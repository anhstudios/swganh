
#include "command_service.h"

#include <cctype>

#include <boost/filesystem.hpp>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "anh/logger.h"

#include "anh/crc.h"
#include "anh/event_dispatcher.h"
#include "anh/database/database_manager_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/command/command_filter.h"

#include "swganh/messages/controllers/combat_action_message.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

#include "python_command.h"
#include "swganh/simulation/simulation_service.h"

#include "swganh/tre/tre_archive.h"
#include "swganh/tre/readers/datatable_reader.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;
using namespace swganh::scripting;
using namespace swganh::simulation;

using boost::asio::deadline_timer;
using boost::posix_time::milliseconds;
using swganh::app::SwganhKernel;
using swganh::tre::readers::DatatableReader;

CommandService::CommandService(SwganhKernel* kernel)
: BaseService(kernel)
{}

ServiceDescription CommandService::GetServiceDescription()
{
    ServiceDescription service_description(
        "CommandService",
        "command",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

void CommandService::AddCommandEnqueueFilter(CommandFilter&& filter)
{
	enqueue_filters_.push_back(move(filter));
}

void CommandService::AddCommandProcessFilter(CommandFilter&& filter)
{
    process_filters_.push_back(move(filter));
}

void CommandService::SetCommandHandler(uint32_t command_crc, CommandHandler&& handler)
{
    handlers_[command_crc] = move(handler);
}

void CommandService::EnqueueCommand(
    const shared_ptr<Creature>& actor,
	const shared_ptr<Tangible>& target,
    CommandQueueEnqueue command)
{
    auto properties_iter = command_properties_map_.find(command.command_crc);
    if (properties_iter == command_properties_map_.end())
    {
        LOG(warning) << "Invalid handler requested: " << hex << command.command_crc;
        return;
    }

    auto handlers_iter = handlers_.find(command.command_crc);
    if (handlers_iter == handlers_.end())
    {
        LOG(warning) << "No handler for command: " << std::hex << command.command_crc;
        return;
    }

    if (!ValidateCommand(actor, target, command, properties_iter->second, enqueue_filters_))
    {
        LOG(warning) << "Command validation failed";
        return;
    }

    if (properties_iter->second.add_to_combat_queue && actor->HasState(COMBAT))
    {
        std::lock_guard<std::mutex> lg(processor_map_mutex_);

        auto find_iter = processor_map_.find(actor->GetObjectId());
        if (find_iter != processor_map_.end() )
        {
            find_iter->second->PushTask(
                milliseconds(properties_iter->second.default_time),
                bind(&CommandService::ProcessCommand,
                    this,
                    actor,
                    target,
                    command,
                    properties_iter->second,
                    handlers_iter->second));
        }
    }
    else
    {
        ProcessCommand(
            actor,
            target,
            command,
            properties_iter->second,
            handlers_iter->second);
    }
}

void CommandService::HandleCommandQueueEnqueue(
    const shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue message)
{
    auto actor = static_pointer_cast<Creature>(controller->GetObject());
	auto target = simulation_service_->GetObjectById<Tangible>(message.target_id);

    EnqueueCommand(actor, target, move(message));
}

void CommandService::HandleCommandQueueRemove(
    const shared_ptr<ObjectController>& controller,
    CommandQueueRemove message)
{}

void CommandService::ProcessCommand(
    const shared_ptr<Creature>& actor,
    const shared_ptr<Tangible>& target,
    const swganh::messages::controllers::CommandQueueEnqueue& command,
    const CommandProperties& properties,
    const CommandHandler& handler
    )
{
    try {
        if (ValidateCommand(actor, target, command, properties, process_filters_))
        {
		    handler(kernel(), actor, target, command);
            // Convert the default time to a float of seconds.
            float default_time = command_properties_map_[command.command_crc].default_time / 1000.0f;

            SendCommandQueueRemove(actor, command.action_counter, default_time, 0, 0);
        }
    } catch(const exception& e) {
        LOG(warning) << "Error Processing Command: " <<  command_properties_map_[command.command_crc].name << "\n" << e.what();
    }

}

void CommandService::onStart()
{
	LoadProperties();
    RegisterCommandScripts();

    simulation_service_ = kernel()->GetServiceManager()->GetService<SimulationService>("SimulationService");

    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueEnqueue, this);
    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueRemove, this);

	auto event_dispatcher = kernel()->GetEventDispatcher();
	event_dispatcher->Dispatch(
        make_shared<anh::ValueEvent<CommandPropertiesMap>>("CommandServiceReady", GetCommandProperties()));

    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        std::lock_guard<std::mutex> lg(processor_map_mutex_);
        processor_map_[object->GetObjectId()].reset(new anh::SimpleDelayedTaskProcessor(kernel()->GetIoService()));
    });

    event_dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        std::lock_guard<std::mutex> lg(processor_map_mutex_);
        processor_map_.erase(object->GetObjectId());
    });
}

void CommandService::LoadProperties()
{
    try {
        auto tre_archive = kernel()->GetTreArchive();
        
        DatatableReader reader(tre_archive->GetResource("datatables/command/command_table.iff"));

        while(reader.Next())
        {
            auto row = reader.GetRow();

            CommandProperties properties;

            properties.name = row["commandName"]->GetValue<string>();

            string tmp = properties.name;
            transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            properties.name_crc = anh::memcrc(tmp);
            
            properties.ability = row["characterAbility"]->GetValue<string>();
            properties.ability_crc = anh::memcrc(properties.ability);

            properties.add_to_combat_queue = row["addToCombatQueue"]->GetValue<int>();

            command_properties_map_.insert(make_pair(properties.name_crc, move(properties)));
        }
        
        LOG(info) << "Loaded (" << command_properties_map_.size() << ") Commands";
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void CommandService::RegisterCommandScripts()
{    
    boost::filesystem::path command_script_dir("scripts/commands");

    try {
        if (!boost::filesystem::exists(command_script_dir) ||
            !boost::filesystem::is_directory(command_script_dir)) {
            throw runtime_error("Invalid script directory [scripts/commands]");
        }

        std::for_each(boost::filesystem::directory_iterator(command_script_dir),
            boost::filesystem::directory_iterator(),
            [this] (const boost::filesystem::directory_entry& entry) 
        {            
            if (entry.path().extension() != ".py") {
                return;
            }

            auto native_path = entry.path().native();
            auto native_name = entry.path().filename().native();
            
            string tmp = string(native_name.begin(), native_name.end()-3);
            transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            
            auto find_iter = command_properties_map_.find(anh::memcrc(tmp));
            if (find_iter != end(command_properties_map_))
            {
                SetCommandHandler(find_iter->second.name_crc, 
                    PythonCommand(find_iter->second, string(begin(native_path), end(native_path))));
            }
        });

    } catch(const std::exception& e) {
        LOG(fatal) << e.what();
    }
}

bool CommandService::ValidateCommand(
    const shared_ptr<Creature>& actor,
	const shared_ptr<Tangible>& target,
    const swganh::messages::controllers::CommandQueueEnqueue& command,
    const CommandProperties& command_properties,
    const std::vector<CommandFilter>& filters)
{
	tuple<bool, uint32_t, uint32_t> result;

    bool all_run = all_of(
        begin(filters),
        end(filters),
        [&result, actor, target, &command, &command_properties] (const CommandFilter& filter)->bool
    {
        result = filter(actor, target, command, command_properties);
		return get<0>(result);
    });

    if (!all_run)
    {
        SendCommandQueueRemove(actor, command.action_counter, 0.0f, get<1>(result), get<2>(result));
    }

    return all_run;
}

void CommandService::SendCommandQueueRemove(
    const shared_ptr<Creature>& actor,
    uint32_t action_counter,
    float default_time_sec,
    uint32_t error,
    uint32_t action)
{
    CommandQueueRemove remove;
    remove.action_counter = action_counter;
    remove.timer = default_time_sec;
    remove.error = error;
    remove.action = action;

	actor->GetController()->Notify(remove);
}

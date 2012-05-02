// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

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
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/command/command_filter.h"

#include "swganh/messages/controllers/combat_action_message.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

#include "python_command.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/scripting/python_event.h"

#include "swganh/tre/tre_archive.h"
#include "swganh/tre/readers/datatable_reader.h"

#include "command_properties_loader_interface.h"

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
: kernel_(kernel)
{
    command_properties_loader_impl_ = kernel->GetPluginManager()->CreateObject<CommandPropertiesLoaderInterface>("Command::PropertiesLoader");
}

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
        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);

        auto find_iter = processor_map_.find(actor->GetObjectId());
        if (find_iter != processor_map_.end() )
        {
            // for combat actions set a default time of 2 seconds if none exist
            uint64_t default_time = 2000;
            if (properties_iter->second.default_time > 0)
                default_time = static_cast<uint64_t>(properties_iter->second.default_time * 1000);
            find_iter->second->PushTask(
                milliseconds(default_time),
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
		    handler(kernel_, actor, target, command);
            
            SendCommandQueueRemove(actor, command.action_counter, command_properties_map_[command.command_crc].default_time, 0, 0);
        }
    } catch(const exception& e) {
        LOG(warning) << "Error Processing Command: " <<  command_properties_map_[command.command_crc].command_name.ident_string() << "\n" << e.what();
    }

}

void CommandService::Start()
{
    script_prefix_ = kernel_->GetAppConfig().script_directory;
    
    command_properties_map_ = command_properties_loader_impl_->LoadCommandPropertiesMap();

    RegisterCommandScripts();

    simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");

    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueEnqueue, this);
    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueRemove, this);

    delayed_task_.reset(new anh::SimpleDelayedTaskProcessor(kernel_->GetIoService()));

	auto event_dispatcher = kernel_->GetEventDispatcher();
	event_dispatcher->Dispatch(
        make_shared<anh::ValueEvent<CommandPropertiesMap>>("CommandServiceReady", GetCommandProperties()));

    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_[object->GetObjectId()].reset(new anh::SimpleDelayedTaskProcessor(kernel_->GetIoService()));
    });

    event_dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_.erase(object->GetObjectId());
    });

    event_dispatcher->Subscribe(
        "PythonEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& python_event = static_pointer_cast<PythonEvent>(incoming_event);
        try {
        // Do we have a timer?
        if (python_event->timer > 0.0f)
        {
            LOG(info) << "triggering Python callback";
            // If so trigger it on another thread as a delayed task processor
            delayed_task_->PushTask(boost::posix_time::milliseconds(static_cast<uint64_t>(python_event->timer * 1000)), [=]() {
                python_event->callback();
            });
        }
        else
        {
            LOG(info) << "triggering Python callback";
            python_event->callback();
        }
        // We can trigger it now
        }
        catch (...)
        {
           LOG(warning) << "exception in handling Python Event";
        }
    });
}

void CommandService::RegisterCommandScripts()
{    
    boost::filesystem::path command_script_dir(script_prefix_ + "/commands");

    try {
        if (!boost::filesystem::exists(command_script_dir) ||
            !boost::filesystem::is_directory(command_script_dir)) {
            throw runtime_error("Invalid script directory [" + script_prefix_ + "/commands]");
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
            
            auto find_iter = command_properties_map_.find(anh::HashString(tmp));
            if (find_iter != end(command_properties_map_)
                && find_iter->second.add_to_combat_queue == 0)
            {
                SetCommandHandler(find_iter->second.command_name.ident(), 
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

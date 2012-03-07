
#include "swganh/command/command_service.h"

#include <cctype>

#include "anh/app/kernel_interface.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <boost/log/trivial.hpp>

#include "anh/crc.h"
#include "anh/event_dispatcher.h"
#include "anh/database/database_manager_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/command/command_filter.h"

#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/messages/controllers/command_queue_remove.h"
#include "swganh/messages/controllers/combat_action_message.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

#include "python_command.h"
#include "swganh/simulation/simulation_service.h"

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

CommandService::CommandService(KernelInterface* kernel)
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
        BOOST_LOG_TRIVIAL(warning) << "Invalid handler requested: " << hex << command.command_crc;
        return;
    }

    auto handlers_iter = handlers_.find(command.command_crc);
    if (handlers_iter == handlers_.end())
    {
        BOOST_LOG_TRIVIAL(warning) << "No handler for command: " << std::hex << command.command_crc;
        return;
    }

    if (!ValidateCommand(actor, target, command, properties_iter->second, enqueue_filters_))
    {
        BOOST_LOG_TRIVIAL(warning) << "Command validation failed";
        return;
    }

    if (properties_iter->second.add_to_combat_queue && actor->HasState(COMBAT))
    {
        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);

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
    const ObjControllerMessage& message)
{
    CommandQueueEnqueue enqueue;
    enqueue.Deserialize(message.data);

    auto actor = static_pointer_cast<Creature>(controller->GetObject());
	auto target = simulation_service_->GetObjectById<Tangible>(enqueue.target_id);

    EnqueueCommand(actor, target, move(enqueue));
}

void CommandService::HandleCommandQueueRemove(
    const shared_ptr<ObjectController>& controller,
    const ObjControllerMessage& message)
{}

void CommandService::ProcessCommand(
    const shared_ptr<Creature>& actor,
    const shared_ptr<Tangible>& target,
    const swganh::messages::controllers::CommandQueueEnqueue& command,
    const CommandProperties& properties,
    const CommandHandler& handler
    )
{
    if (ValidateCommand(actor, target, command, properties, process_filters_))
    {
		handler(kernel(), actor, target, command);
        // Convert the default time to a float of seconds.
        float default_time = command_properties_map_[command.command_crc].default_time / 1000.0f;

        SendCommandQueueRemove(actor, command.action_counter, default_time, 0, 0);
    }
}

void CommandService::onStart()
{
	LoadProperties();

    simulation_service_ = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");

    simulation_service_->RegisterControllerHandler(0x00000116, [this] (
        const std::shared_ptr<ObjectController>& controller,
        const swganh::messages::ObjControllerMessage& message)
    {
        HandleCommandQueueEnqueue(controller, message);
    });

    simulation_service_->RegisterControllerHandler(0x00000117, [this] (
        const std::shared_ptr<ObjectController>& controller,
        const swganh::messages::ObjControllerMessage& message)
    {
        HandleCommandQueueRemove(controller, message);
    });

	auto event_dispatcher = kernel()->GetEventDispatcher();
	event_dispatcher->Dispatch(
        make_shared<anh::ValueEvent<CommandPropertiesMap>>("CommandServiceReady", GetCommandProperties()));

    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_[object->GetObjectId()].reset(new anh::SimpleDelayedTaskProcessor(kernel()->GetIoService()));
    });

    event_dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_.erase(object->GetObjectId());
    });
}

void CommandService::LoadProperties()
{
    try {
        auto db_manager = kernel()->GetDatabaseManager();

        auto conn = db_manager->getConnection("galaxy");
        auto statement =  unique_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_LoadCommandProperties();"));

        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result->next())
        {
            CommandProperties properties;

            properties.name = result->getString("name");

            string tmp = properties.name;
            transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
            properties.name_crc = anh::memcrc(tmp);

            properties.ability = result->getString("ability");
            properties.ability_crc = anh::memcrc(properties.ability);
            properties.deny_in_states = result->getUInt64("deny_in_states");
            properties.script_hook = result->getString("script_hook");
            properties.fail_script_hook = result->getString("fail_script_hook");
            properties.default_time = result->getUInt64("default_time");
            properties.command_group = result->getUInt("command_group");
            properties.max_range_to_target = static_cast<float>(result->getDouble("max_range_to_target"));
            properties.add_to_combat_queue = result->getUInt("add_to_combat_queue");
            properties.health_cost = result->getUInt("health_cost");
            properties.health_cost_multiplier = result->getUInt("health_cost_multiplier");
            properties.action_cost = result->getUInt("action_cost");
            properties.action_cost_multiplier = result->getUInt("action_cost_multiplier");
            properties.mind_cost = result->getUInt("mind_cost");
            properties.mind_cost_multiplier = result->getUInt("mind_cost");
            properties.damage_multiplier = static_cast<float>(result->getDouble("damage_multiplier"));
            properties.delay_multiplier = static_cast<float>(result->getDouble("delay_multiplier"));
            properties.force_cost = result->getUInt("force_cost");
            properties.force_cost_multiplier = result->getUInt("force_cost_multiplier");
            properties.animation_crc = result->getUInt("animation_crc");
            properties.required_weapon_group = result->getUInt("required_weapon_group");
            properties.combat_spam = result->getString("combat_spam");
            properties.trail1 = result->getUInt("trail1");
            properties.trail2 = result->getUInt("trail2");
            properties.allow_in_posture = result->getUInt("allow_in_posture");
            properties.health_hit_chance = static_cast<float>(result->getDouble("health_hit_chance"));
            properties.action_hit_chance = static_cast<float>(result->getDouble("action_hit_chance"));
            properties.mind_hit_chance = static_cast<float>(result->getDouble("mind_hit_chance"));
            properties.knockdown_hit_chance = static_cast<float>(result->getDouble("knockdown_chance"));
            properties.dizzy_hit_chance = static_cast<float>(result->getDouble("dizzy_chance"));
            properties.blind_chance = static_cast<float>(result->getDouble("blind_chance"));
            properties.stun_chance = static_cast<float>(result->getDouble("stun_chance"));
            properties.intimidate_chance = static_cast<float>(result->getDouble("intimidate_chance"));
            properties.posture_down_chance = static_cast<float>(result->getDouble("posture_down_chance"));
            properties.extended_range = static_cast<float>(result->getDouble("extended_range"));
            properties.cone_angle = static_cast<float>(result->getDouble("cone_angle"));
            properties.deny_in_locomotion = result->getUInt64("deny_in_locomotion");

            RegisterCommandScript(properties);

            command_properties_map_.insert(make_pair(properties.name_crc, move(properties)));
        }

        BOOST_LOG_TRIVIAL(warning) << "Loaded (" << command_properties_map_.size() << ") Commands";
    }
    catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void CommandService::RegisterCommandScript(const CommandProperties& properties)
{
    if (properties.script_hook.length() != 0)
    {
        SetCommandHandler(properties.name_crc, PythonCommand(properties));
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

	actor->GetController()->Notify(ObjControllerMessage(0x0000000B, remove));
}

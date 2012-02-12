
#include "swganh/command/command_service.h"

#include <cctype>

#include "anh/app/kernel_interface.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/crc.h"
#include "anh/database/database_manager_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/command/command_filter.h"

#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/messages/controllers/command_queue_remove.h"

#include "swganh/object/creature/creature.h"
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
using namespace swganh::scripting;
using namespace swganh::simulation;

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
	const shared_ptr<Object>& target,
    CommandQueueEnqueue command)
{
    bool is_valid;
    uint32_t error = 0, action = 0;

    std::tie(is_valid, error, action) = ValidateCommand(actor, target, command, command_properties_map_[command.command_crc], enqueue_filters_);
    
    if (!is_valid)
    {
        LOG(WARNING) << "Invalid command";
        SendCommandQueueRemove(actor, command, 0.0f, error, action);
        return;
    }
	auto object_id = actor->GetObjectId();
    command_queues_[object_id].push(command);    

    if (!command_queue_timers_[object_id])
    {
        command_queue_timers_[object_id] = make_shared<boost::asio::deadline_timer>(kernel()->GetIoService());
    }

    /*if (command_queue_timers_[object_id]->expires_at() > boost::posix_time::second_clock::universal_time())
    {*/
        ProcessNextCommand(actor);
    //}
}

void CommandService::HandleCommandQueueEnqueue(
    const shared_ptr<ObjectController>& controller, 
    const ObjControllerMessage& message)
{
    auto actor = dynamic_pointer_cast<Creature>(controller->GetObject());
	auto simulation_service = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");
    
    CommandQueueEnqueue enqueue;
    enqueue.Deserialize(message.data);

	auto target = simulation_service->GetObjectById(enqueue.target_id);

    auto find_iter = command_properties_map_.find(enqueue.command_crc);

    if (find_iter == command_properties_map_.end())
    {
        LOG(WARNING) << "Invalid handler requested: " << hex << enqueue.command_crc;
        return;
    }

    auto cooldown_find_iter = cooldown_timers_[actor->GetObjectId()].find(enqueue.command_crc);

    if (cooldown_find_iter != cooldown_timers_[actor->GetObjectId()].end())
    {
        LOG(WARNING) << "Cooldown timer still running";
        return;
    }
    
    if (find_iter->second.add_to_combat_queue)
    {
        EnqueueCommand(actor, target, enqueue);
    }
    else
    {
        ProcessCommand(actor, target, enqueue);
    }
}

void CommandService::HandleCommandQueueRemove(
    const shared_ptr<ObjectController>& controller, 
    const ObjControllerMessage& message)
{}

void CommandService::ProcessNextCommand(const shared_ptr<Creature>& actor)
{
    auto find_iter = command_queues_.find(actor->GetObjectId());
        
    CommandQueueEnqueue command; 
        
    if (!find_iter->second.try_pop(command))
    {
        return;
    }
	auto simulation_service = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");
    auto target = simulation_service->GetObjectById(command.target_id);

    ProcessCommand(actor, target, command);

    command_queue_timers_[actor->GetObjectId()]->expires_from_now(
        boost::posix_time::milliseconds(command_properties_map_[command.command_crc].default_time));
    
    command_queue_timers_[actor->GetObjectId()]->async_wait(bind(&CommandService::ProcessNextCommand, this, actor));
}

void CommandService::ProcessCommand(const shared_ptr<Creature>& actor, const shared_ptr<Object>& target, const swganh::messages::controllers::CommandQueueEnqueue& command)
{    
	auto object_id = actor->GetObjectId();

    auto find_iter = handlers_.find(command.command_crc);

    if (find_iter == handlers_.end())
    {
        LOG(WARNING) << "No handler for command: " << std::hex << command.command_crc;
        return;
    }
    
    bool is_valid;
    uint32_t error = 0, action = 0;

    std::tie(is_valid, error, action) = ValidateCommand(actor, target, command, command_properties_map_[command.command_crc], enqueue_filters_);
    
    if (is_valid)
    {
		find_iter->second(actor, target, command.command_options);
    
        SendCommandQueueRemove(actor, command, command_properties_map_[command.command_crc].default_time, 0, 0);
         
        if (command_properties_map_[command.command_crc].default_time != 0)
        {
            cooldown_timers_[object_id][command.command_crc] = make_shared<boost::asio::deadline_timer>(kernel()->GetIoService());
            
            cooldown_timers_[object_id][command.command_crc]->expires_from_now(
                boost::posix_time::milliseconds(command_properties_map_[command.command_crc].default_time));
        
            cooldown_timers_[object_id][command.command_crc]->async_wait([this, object_id, command] (const boost::system::error_code& ec) {
                cooldown_timers_[object_id].erase(command.command_crc);
            });
        }
    }     
        
    SendCommandQueueRemove(actor, command, 0.0f, error, action);
}

void CommandService::onStart()
{
    LoadProperties();

    auto simulation_service = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");
    
    simulation_service->RegisterControllerHandler(0x00000116, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        HandleCommandQueueEnqueue(controller, message);
    });
    
    simulation_service->RegisterControllerHandler(0x00000117, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        HandleCommandQueueRemove(controller, message);
    });
}

void CommandService::LoadProperties()
{    
    try {
        auto db_manager = kernel()->GetDatabaseManager();

        auto conn = db_manager->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_LoadCommandProperties();"));

        auto result = shared_ptr<sql::ResultSet>(statement->executeQuery());
               
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
            properties.max_range_to_target = result->getDouble("max_range_to_target");
            properties.add_to_combat_queue = result->getUInt("add_to_combat_queue");
            properties.health_cost = result->getUInt("health_cost");
            properties.health_cost_multiplier = result->getUInt("health_cost_multiplier");
            properties.action_cost = result->getUInt("action_cost");
            properties.action_cost_multiplier = result->getUInt("action_cost_multiplier");
            properties.mind_cost = result->getUInt("mind_cost");
            properties.mind_cost_multiplier = result->getUInt("mind_cost");
            properties.damage_multiplier = result->getDouble("damage_multiplier");
            properties.delay_multiplier = result->getDouble("delay_multiplier");
            properties.force_cost = result->getUInt("force_cost");
            properties.force_cost_multiplier = result->getUInt("force_cost_multiplier");
            properties.animation_crc = result->getUInt("animation_crc");
            properties.required_weapon_group = result->getUInt("required_weapon_group");
            properties.combat_spam = result->getString("combat_spam");
            properties.trail1 = result->getUInt("trail1");
            properties.trail2 = result->getUInt("trail2");
            properties.allow_in_posture = result->getUInt("allow_in_posture");
            properties.health_hit_chance = result->getDouble("health_hit_chance");
            properties.action_hit_chance = result->getDouble("action_hit_chance");
            properties.mind_hit_chance = result->getDouble("mind_hit_chance");
            properties.knockdown_hit_chance = result->getDouble("knockdown_chance");
            properties.dizzy_hit_chance = result->getDouble("dizzy_chance");
            properties.blind_chance = result->getDouble("blind_chance");
            properties.stun_chance = result->getDouble("stun_chance");
            properties.intimidate_chance = result->getDouble("intimidate_chance");
            properties.posture_down_chance = result->getDouble("posture_down_chance");
            properties.extended_range = result->getDouble("extended_range");
            properties.cone_angle = result->getDouble("cone_angle");
            properties.deny_in_locomotion = result->getUInt64("deny_in_locomotion");
            
            command_properties_map_.insert(make_pair(properties.name_crc, move(properties)));

            RegisterCommandScript(properties);
        }

        DLOG(WARNING) << "Loaded (" << command_properties_map_.size() << ") Commands";
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void CommandService::RegisterCommandScript(const CommandProperties& properties)
{
    if (properties.script_hook.length() == 0)
    {
        return;
    }
    
    SetCommandHandler(properties.name_crc, PythonCommand(properties));
}

tuple<bool, uint32_t, uint32_t> CommandService::ValidateCommand(
    const shared_ptr<Creature>& actor, 
	const shared_ptr<Object>& target,
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
        result =  filter(actor, target, command, command_properties);
		return get<0>(result);
    });

    return result;
}

void CommandService::SendCommandQueueRemove(
    const shared_ptr<Creature>& actor,
    const swganh::messages::controllers::CommandQueueEnqueue& command,
    float default_time,
    uint32_t error,
    uint32_t action)
{
    CommandQueueRemove remove;
    remove.action_counter = command.action_counter;
	if (default_time > 1000)
	{
		remove.timer = default_time / 1000;
	}
	else
	{
		remove.timer = 1;
	}
    remove.error = error;
    remove.action = action;

	//actor->GetController()->Notify(remove);
    actor->GetController()->Notify(ObjControllerMessage(0x0000000B, remove));
}

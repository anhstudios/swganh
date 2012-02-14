
#include "swganh/combat/combat_service.h"

#include <cctype>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/app/kernel_interface.h"
#include "anh/crc.h"
#include "anh/event_dispatcher.h"
#include "anh/database/database_manager_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/object_controller.h"

#include "swganh/command/command_service.h"
#include "swganh/command/python_command.h"
#include "swganh/simulation/simulation_service.h"

#include "swganh/messages/controllers/combat_action_message.h"
#include "swganh/messages/controllers/combat_spam_message.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::simulation;
using namespace swganh::combat;
using namespace swganh::command;

CombatService::CombatService(KernelInterface* kernel)
: BaseService(kernel)
{}

ServiceDescription CombatService::GetServiceDescription()
{
    ServiceDescription service_description(
        "CombatService",
        "Combat",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

void CombatService::onStart()
{
	simulation_service_ = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");

	command_service_ = kernel()->GetServiceManager()
		->GetService<CommandService>("CommandService");
    
	kernel()->GetEventDispatcher()->Subscribe(
        "CommandServiceReady", 
        [this] (const shared_ptr<anh::EventInterface>& incoming_event)
    {
        const auto& commandProperties = static_pointer_cast<anh::ValueEvent<swganh::command::CommandPropertiesMap>>(incoming_event)->Get();
        LoadProperties(commandProperties);
        
    });
}

void CombatService::RegisterCombatHandler(uint32_t command_crc, CombatHandler&& handler)
{
    combat_handlers_[command_crc] = move(handler);

	command_service_->SetCommandHandler(command_crc, [=] (const shared_ptr<Creature>& actor,
			const shared_ptr<Object>& target, const CommandQueueEnqueue& command_queue_message){

        ProcessCombatCommand(actor, target, command_queue_message);
    });
}

void CombatService::RegisterCombatScript(const CommandProperties& properties)
{
    if (properties.script_hook.length() == 0)
    {
        return;
    }
    
    RegisterCombatHandler(properties.name_crc, PythonCommand(properties));
}

void CombatService::LoadProperties(swganh::command::CommandPropertiesMap command_properties)
{    
	for_each(begin(command_properties), end(command_properties), [=] (pair<uint32_t, CommandProperties> command){
		// load up all the combat commands into their own map
		if (command.second.add_to_combat_queue || command.second.command_group == 1 || command.second.command_group == 2)
		{
			combat_properties_map_.insert(command);
            RegisterCombatScript(command.second);
		}
	});
    DLOG(WARNING) << "Loaded (" << combat_properties_map_.size() << ") Combat Commands";
}

void CombatService::ProcessCombatCommand(
			const shared_ptr<Creature>& actor,
			const shared_ptr<Object>& target,
			const CommandQueueEnqueue& command_queue_message)
{
    // Get all the data we need to determine what to do with this command
    auto find_iter = combat_handlers_.find(command_queue_message.command_crc);

    if (find_iter == end(combat_handlers_))
    {
        LOG(WARNING) << "No combat handler for command: " << std::hex << command_queue_message.command_crc;
        return;
    }
    /*auto combat_data =*/ find_iter->second(actor, target, command_queue_message);

    SendCombatAction(actor, target, command_queue_message);
}

bool CombatService::InitiateCombat(
    const std::shared_ptr<Creature>& attacker, 
    const shared_ptr<Object>& target, 
    const CommandQueueEnqueue& command_message)
{
    // check to see if we are able to start combat ( are we in peace? )
    if (command_message.command_crc == anh::HashString("peace"))
        return false;
    if (attacker == target)
        return false;

    if (target == nullptr)
        return false;

    shared_ptr<Creature> creature_target = nullptr;
    if (target->GetType() == Creature::type)
        creature_target = static_pointer_cast<Creature>(target);
    // Check if mounted

    // Check if target can be attacked
    if (creature_target != nullptr  && creature_target->GetPosture() == INCAPACITATED)
        return false;

    attacker->AddDefender(target->GetObjectId());
    if (creature_target != nullptr)
        creature_target->AddDefender(attacker->GetObjectId());

    return true;
}

void CombatService::SendCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Object>& target, 
    const CommandQueueEnqueue& command_message)
{
    if (InitiateCombat(attacker, target, command_message))
    {
        auto find_iter = combat_properties_map_.find(command_message.command_crc);
        if (find_iter == end(combat_properties_map_))
            return;
        auto command_property = find_iter->second;

        // Check For Hit

        // Apply Special Attack Cost

        // Check for AOE 

        // Do Animation

        // Apply Damage

        // Combat Spam
        if (command_property.combat_spam.length() > 0)
        {
            CombatSpamMessage spam;
            spam.attacker_id = attacker->GetObjectId();
            spam.defender_id = target->GetObjectId();
            spam.damage = 100;
            spam.file = command_property.combat_spam + "_hit";
        
            attacker->GetController()->Notify(ObjControllerMessage(0x00000134, spam));
        }
        // Send Message
        
        CombatActionMessage cam;
        cam.action_crc = 1136984016;//command_property.ability_crc;
        cam.attacker_id = attacker->GetObjectId();
        cam.weapon_id = attacker->GetWeaponId();
        cam.attacker_end_posture = attacker->GetPosture();
        
        auto defenders = attacker->GetDefenders();
        // build up the defenders
        for_each(defenders.Begin(), defenders.End(), [=, &cam](swganh::object::tangible::Defender defender) {
            CombatDefender def_list;
            def_list.defender_id = defender.object_id;
            def_list.defender_end_posture = simulation_service_->GetObjectById<Creature>(defender.object_id)->GetPosture();
            def_list.hit_type = 0x1;
            def_list.defender_special_move_effect = 0;
            cam.defender_list.push_back(def_list);
        });
        cam.combat_special_move_effect = 0;
        
        attacker->GetController()->Notify(ObjControllerMessage(0x000000CC, cam));

        // If we ended in combat, re-queue this back into the command queue
        // is AutoAttack
        command_service_->EnqueueCommand(attacker, target, command_message);
    }
}
bool CombatService::SingleTargetCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Object>& target, 
    const CommandQueueEnqueue& command_message)
{
    return true;
}
uint16_t CombatService::GetPostureModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker){ return 0; }
uint16_t CombatService::GetAccuracyModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker) { return 0; }
uint16_t CombatService::GetAccuracyBonus(const std::shared_ptr<swganh::object::creature::Creature>& attacker) { return 0; }
void CombatService::ApplyStates(const std::shared_ptr<swganh::object::creature::Creature>& attacker){}
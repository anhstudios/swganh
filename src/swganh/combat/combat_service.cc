
#include "swganh/combat/combat_service.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
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
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/weapon/weapon.h"

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
using namespace swganh::object::weapon;
using namespace swganh::object::tangible;
using namespace swganh::simulation;
using namespace swganh::combat;
using namespace swganh::command;

boost::random::mt19937 gen;

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
			const shared_ptr<Tangible>& target, const CommandQueueEnqueue& command_queue_message){

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
			const shared_ptr<Tangible>& target,
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
    const shared_ptr<Tangible>& target, 
    const CommandQueueEnqueue& command_message)
{
    // check to see if we are able to start combat ( are we in peace? )
    if (command_message.command_crc == anh::HashString("peace"))
        return false;
    if (attacker->GetObjectId() == target->GetObjectId())
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
    const shared_ptr<Tangible>& target, 
    const CommandQueueEnqueue& command_message)
{
    if (InitiateCombat(attacker, target, command_message))
    {
        auto find_iter = combat_properties_map_.find(command_message.command_crc);
        if (find_iter == end(combat_properties_map_))
            return;
        auto command_property = find_iter->second;
        int damage = 0;
        string string_hit = "";
        // Check For Hit

        // Apply Special Attack Cost

        // Check for AOE 

        // Do Animation

        // Apply Damage

        // Combat Spam
        BroadcastCombatSpam(attacker, target, command_property, damage, string_hit);
        // Send Message
        
        CombatActionMessage cam;
        cam.action_crc = 1136984016;//command_property.ability_crc;
        cam.attacker_id = attacker->GetObjectId();
        cam.weapon_id = attacker->GetWeaponId();
        cam.attacker_end_posture = attacker->GetPosture();
        
        auto defenders = attacker->GetDefenders();
        // build up the defenders
        for_each(defenders.Begin(), defenders.End(), [=, &cam](Defender defender) {
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
        if (command_property.name.compare("attack") > 0)
            command_service_->EnqueueCommand(attacker, target, command_message);
    }
}
bool CombatService::SingleTargetCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    const CommandQueueEnqueue& command_message)
{
    return true;
}
uint16_t CombatService::GetPostureModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker){
    uint16_t accuracy = 0;
    uint32_t posture = attacker->GetPosture();
    if (posture == creature::CROUCHED)
        accuracy += 16;
    else if (posture == creature::PRONE)
        accuracy += 50;
    // If Running
    // accuracy -= 50;

    return accuracy; 
}
uint16_t CombatService::GetTargetPostureModifier(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    uint16_t accuracy = 0;
    uint32_t posture = attacker->GetPosture();
    auto weapon = attacker->GetEquipmentItem(attacker->GetWeaponId());
    
    if (posture == creature::CROUCHED)
        
        accuracy += 16;
    else if (posture == creature::PRONE)
        accuracy += 25;

    return accuracy;

}
uint16_t CombatService::GetAccuracyModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker) { 
    // TODO: Get weapon calculation modifiers
    // Get Accuracy Modifiers from weapon and add up the modifiers the creature has
    return 0; 
}
uint16_t CombatService::GetAccuracyBonus(const std::shared_ptr<swganh::object::creature::Creature>& attacker) { 
    // get base attacker accuracy mods

    // give additional mods based on Posture and weapon type
    return 0; 
}
void CombatService::ApplyStates(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target, CommandProperties& properties) {
    auto states = move(properties.getStates());
    for_each(begin(states), end(states),[=](pair<float, string> state){
        boost::random::uniform_int_distribution<> dist(1, 100);
        int generated = dist(gen);
        // Didn't trigger this time
        if (generated > state.first)
        {
            return;
        }
        // Check recovery timers

        // Defender State Modifiers

        // Strength of Modifier

        // GetHitChance(strength, 0.0f, target_defence);

        // Jedi Defences

        // Send Message

        // Check Equilibrium
    });
}
float CombatService::GetHitChance(float attacker_accuracy, float attacker_bonus, float target_defence) 
{
    // TODO: Verify this is the appropriate formula
    return (66.0 + attacker_bonus + (attacker_accuracy - target_defence) / 2.0);
}
int CombatService::GetDamagingPool(int pool_to_damage)
{
    if (pool_to_damage) 
    {
        boost::random::uniform_int_distribution<> dist(1, 100);
        int generated = dist(gen);

        if (generated < 50) {
            pool_to_damage = 0; // HEALTH
        }
        else if (generated < 85) {
            pool_to_damage = 1; // ACTION
        }
        else {
            pool_to_damage = 2; // MIND
        }
    }
    return pool_to_damage;
}

void CombatService::BroadcastCombatSpam(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Tangible>& target, 
    const CommandProperties& properties,
    uint32_t damage, const string& string_file)
{
    if (properties.combat_spam.length() > 0)
        {
            CombatSpamMessage spam;
            spam.attacker_id = attacker->GetObjectId();
            spam.defender_id = target->GetObjectId();
            spam.weapon_id = attacker->GetWeaponId();
            spam.damage = damage;
            spam.file = properties.combat_spam + string_file;
        
            attacker->GetController()->Notify(ObjControllerMessage(0x00000134, spam));
        }
}
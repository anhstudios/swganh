
#include "swganh/combat/combat_service.h"
#include "combat_data.h"
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
#include "swganh/messages/chat_system_message.h"
#include "swganh/messages/out_of_band.h"

using namespace std;
using namespace anh;
using namespace anh::app;
using namespace anh::service;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::weapon;
using namespace swganh::object::tangible;
using namespace swganh::simulation;
using namespace swganh::combat;
using namespace swganh::command;

CombatService::CombatService(KernelInterface* kernel)
: BaseService(kernel)
, generator_(1, 100)
, delayed_task_(new anh::SimpleDelayedTaskProcessor(kernel->GetIoService()))
{
}

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

	command_service_->SetCommandHandler(command_crc, 
        [this, command_crc] (
            const shared_ptr<Creature>& actor,
			const shared_ptr<Tangible>& target, 
            const CommandQueueEnqueue& command_queue_message) {

        combat_handlers_[command_crc](actor, target, command_queue_message);
        SendCombatAction(actor, target, command_queue_message);
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

bool CombatService::InitiateCombat(
    const std::shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    const CommandQueueEnqueue& command_message)
{
    // check to see if we are able to start combat ( are we in peace? )
    if (command_message.command_crc == anh::HashString("peace")) {
        return false;
    }

    if (target == nullptr)
        return false;
    
    if (attacker->GetObjectId() == target->GetObjectId())
    {
        attacker->GetController()->SendSystemMessage(OutOfBand("cbt_spam","shoot_self"));
        return false;
    }

    shared_ptr<Creature> creature_target = nullptr;
    if (target->GetType() == Creature::type)
        creature_target = static_pointer_cast<Creature>(target);
    // Check if mounted

    // Check if target can be attacked
    if (creature_target != nullptr  && creature_target->IsDead() || creature_target->IsIncapacitated())
        return false;
    if (!attacker->CanAttack(creature_target.get()))
        return false;

    // TODO: Base this on weapon range
    if (!attacker->InRange(target->GetPosition(), 25))
        return false;

    // Add Combat
    attacker->ToggleStateOn(COMBAT);
    creature_target->ToggleStateOn(COMBAT);
    if (!creature_target->IsDefending(attacker->GetObjectId()))
    {
        creature_target->AddDefender(attacker->GetObjectId());
    }
    if (!attacker->IsDefending(creature_target->GetObjectId()))
    {
        attacker->AddDefender(creature_target->GetObjectId());
    }
    
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
        string string_hit = "";
        // Check For Hit
        // Combat Spam
        // Check for AOE 
        // if ! AOE
        int damage = SingleTargetCombatAction(attacker, target, command_property);
        // Apply Special Attack Cost

        // Send Message
        SendCombatActionMessage(attacker, target, command_property);
        
        // If we ended in combat, re-queue this back into the command queue
        // is AutoAttack
        // if target is creature, then auto-attack back
        shared_ptr<Creature> creature_target = nullptr;
        if (target->GetType() == Creature::type)
            creature_target = static_pointer_cast<Creature>(target);
        // Apply Damage
        ApplyDamage(attacker, creature_target, damage, 0);

        if (command_property.name == "attack" && attacker->IsAutoAttacking()) {
            command_service_->EnqueueCommand(attacker, target, command_message);
            //command_service_->EnqueueCommand(creature_target, attacker, command_message);
        }
    }
    else
    {
        attacker->GetController()->SendSystemMessage(OutOfBand("error_message","target_not_attackable"));
    }
}
int CombatService::SingleTargetCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    CommandProperties& properties)
{
    int damage = 0;
    if (target->GetType() == Creature::type)
    {
        auto creature = static_pointer_cast<Creature>(target);
        SingleTargetCombatAction(attacker, creature, properties);
    }
    else
    {
        // TODO: Clean this up
        //int pools = GetDamagingPool((int)(properties.health_hit_chance + properties.action_hit_chance + properties.mind_hit_chance));
        //damage = ApplyDamage(attacker, target, damage, HEALTH);

        BroadcastCombatSpam(attacker, target, properties, damage, CombatData::HIT_spam());
    }
    return damage;
}
int CombatService::SingleTargetCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Creature>& defender, 
    CommandProperties& properties)
{
    // Entertaining?

    int hit = 0;
    float damage_multiplier = properties.damage_multiplier;
    int total_damage = 0;

    if (damage_multiplier != 0)
        total_damage = 16; /*CalculateDamage(attacker, defender) * damage_multiplier);*/
    else
    {
        total_damage = 10; // obviously temp
    }
    damage_multiplier = 1.0f;

    hit = GetHitResult(attacker, defender, total_damage, 0); // properties.GetAccuracybonus() + attacker->GetSkillMod(properties.GetAccuracySkillMod()));
    string combat_spam_msg = properties.combat_spam;
    
    switch (hit)
    {
    case HIT:
        total_damage = ApplyDamage(attacker, defender, total_damage, HEALTH);
        BroadcastCombatSpam(attacker, defender, properties, total_damage, CombatData::HIT_spam());
        break;
        // Block
    case BLOCK:
        BroadcastCombatSpam(attacker, defender, properties, total_damage, CombatData::BLOCK_spam());
        damage_multiplier = 0.5f;
        break;
    case DODGE:
        // Dodge
        damage_multiplier = 0.0f;
        BroadcastCombatSpam(attacker, defender, properties, total_damage, CombatData::DODGE_spam());
        break;
    case MISS:
        // Miss
        BroadcastCombatSpam(attacker, defender, properties, total_damage, CombatData::MISS_spam());
        damage_multiplier = 0.0f;
        return 0;
    default:
        return 0;
    }
    ApplyStates(attacker, defender, properties);
    // Apply Dots
    //int pool = GetDamagingPool(
    // Attack Delay?
    return total_damage;
}

uint16_t CombatService::GetHitResult(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Creature>& defender, 
    int damage, int accuracy_bonus)
{
    int hit_outcome = 0;
    // Get Weapon Attack Type
    float weapon_accuracy = 15.0f;
    // Get Weapon Accuracy Mods

    int attacker_accuracy = GetAccuracyModifier(defender);

    accuracy_bonus += GetAccuracyBonus(attacker);

    int target_defense = 15;//GetDefenseModifier(attacker, target);
    float accuracy_total = GetHitChance(attacker_accuracy + weapon_accuracy, accuracy_bonus, target_defense);

    // Scout/Ranger creature hit bonus

    if (accuracy_total > 100.0)
        accuracy_total = 100.0;
    else if(accuracy_total < 0.0f)
        accuracy_total = 0.0f;

    if (generator_.Rand(1,100) > accuracy_total) 
        return MISS;

    // Successful hit
    if (damage > 0)
    {
        // Get Secondary Defense Modifiers

        if (target_defense <= 0)
            return HIT;

        accuracy_total = GetHitChance(attacker_accuracy + weapon_accuracy, accuracy_bonus, target_defense);
        if (accuracy_total > 100.0)
            accuracy_total = 100.0;
        else if(accuracy_total < 0.0f)
            accuracy_total = 0.0f;

        if (generator_.Rand(1,100) > accuracy_total) // Successful secondary defense
        {
            // Get Weapon to see if block makes sense
            // Get Secondary Modifiers
            string def = "counterattack"; // GetWeaponDefenseModifiers

            if (def == "block")
                return BLOCK;
            else if (def == "dodge")
                return DODGE;
            else if (def == "counterattack")
                return COUNTER;
            else if (def == "unarmed_passive_defense")
            {
                generator_.Rand(1,2);
            }
            else
                return HIT;
        }
    }
    return HIT;
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
        int generated = generator_.Rand(1, 100);
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

int CombatService::ApplyDamage(
    const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& defender, int damage, int pool)
{
    // Sanity Check
    if (damage == 0 || pool < 0)
        return 0;

    int generated = generator_.Rand(1, 100);

    float wounds_ratio = 0; /*attacker->GetWeapon()->GetWoundsRatio();*/
    float health_damage = 0.0f, action_damage = 0.0f, mind_damage = 0.0f;
    bool wounded = false;

    // Check For Player && PVP Status

    if (pool == HEALTH) {
        //health_damage = GetArmorReduction(attacker, defender, damage, HEALTH) * damage_multiplier;
        if (defender->GetStatCurrent(HEALTH) - generated <= 0)
        {
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(HEALTH, generated);
        // Will this reduce this pool <= 0 ?
        if (!wounded && generated < wounds_ratio) {
            defender->AddStatWound(HEALTH, generator_.Rand(1,2));
            wounded = true;
        }
    }
    if (pool == ACTION) {
        //action_damage = GetArmorReduction(attacker, defender, damage, ACTION) * damage_multiplier;
        if (defender->GetStatCurrent(ACTION) - generated <= 0)
        {
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(HEALTH, generated);
        if (!wounded && generated < wounds_ratio) {
            defender->AddStatWound(ACTION, generator_.Rand(1,2));
            wounded = true;
        }
    }
    if (pool == MIND) {
        //mind_damage = GetArmorReduction(attacker, defender, damage, MIND) * damage_multiplier;
        if (defender->GetStatCurrent(MIND) - generated <= 0)
        {
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(HEALTH, generated);
        if (!wounded && generated < wounds_ratio) {
            defender->AddStatWound(MIND, generator_.Rand(1,2));
            wounded = true;
        }
    }

    if (wounded)
        defender->AddBattleFatigue(1);

    return generated;

}
int CombatService::GetDamagingPool(int pool)
{
    if (pool) 
    {
        int generated = generator_.Rand(1,100);

        if (generated < 50) {
            pool = HEALTH;
        }
        else if (generated < 85) {
            pool = ACTION;
        }
        else {
            pool = MIND;
        }
    }
    return pool;
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
            spam.file = "cbt_spam";
            spam.text = properties.combat_spam + string_file;
            
            attacker->NotifyObservers(ObjControllerMessage(0x00000134, spam));
        }
}

void CombatService::SendCombatActionMessage(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible> & target, 
    CommandProperties& command_property)
{
        CombatActionMessage cam;
        if (command_property.animation_crc == 0)
            cam.action_crc = CombatData::DefaultAttacks[generator_.Rand(0, 9)];//command_property.ability_crc;
        else
        {
            cam.action_crc = command_property.animation_crc;
        }
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
        
        attacker->NotifyObservers(ObjControllerMessage(0x000000CC, cam));
}

void CombatService::SetIncapacitated(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    target->SetPosture(INCAPACITATED);
    // TODO: Get this from config Default Incap Timer 
    target->SetIncapTimer(15);
    EndCombat(attacker, target);

    attacker->GetController()->SendSystemMessage(OutOfBand("base_player", "prose_target_incap", TT, target->GetObjectId()));
    if (attacker)
    {
        target->GetController()->SendSystemMessage(OutOfBand("base_player", "prose_victim_incap", TT, attacker->GetObjectId()));
    }
    else
        target->GetController()->SendSystemMessage(OutOfBand("base_player", "victim_incapacitated"));

    delayed_task_->PushTask(boost::posix_time::seconds(15), [=](){
        // Incap Recovery
        if (!target || target->IsDead())
            return;
        target->SetPosture(UPRIGHT);
        target->SetIncapTimer(0);
    });
    
}
void CombatService::SetDead(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    target->SetPosture(DEAD);
    // Clear States
    target->SetStateBitmask(NONE);
    EndCombat(attacker, target);
    attacker->GetController()->SendSystemMessage(OutOfBand("base_player", "killer_target_dead", TT, target->GetObjectId()));
    attacker->GetController()->SendSystemMessage(OutOfBand("base_player", "prose_target_dead", TT, target->GetObjectId()));
    if (attacker)
        target->GetController()->SendSystemMessage(OutOfBand("base_player", "prose_victim_dead", TT, attacker->GetObjectId()));
    else
        target->GetController()->SendSystemMessage(OutOfBand("base_player", "victim_dead"));
}

void CombatService::EndDuel(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    if (attacker->InDuelList(target->GetObjectId()))
    {
        attacker->RemoveFromDuelList(target->GetObjectId());
        attacker->SetPvPStatus(PvPStatus_Player);
        attacker->ToggleStateOff(COMBAT);
        // End the Duel for the target as well
        EndDuel(target, attacker);
    }
}
void CombatService::EndCombat(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    attacker->ClearAutoAttack();
    attacker->ToggleStateOff(COMBAT);
    // For Some Reason This Crashes...
    //attacker->RemoveDefender(target->GetObjectId());
    target->RemoveDefender(attacker->GetObjectId());
    attacker->SetTargetId(0);
    // End Duel
    EndDuel(attacker, target);
}
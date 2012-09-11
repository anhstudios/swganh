// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "combat_service.h"
#include "combat_data.h"
#include <cctype>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/crc.h"
#include "swganh/event_dispatcher.h"
#include "swganh/database/database_manager_interface.h"
#include "swganh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh_core/object/creature/creature.h"
#include "swganh/observer/observer_interface.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh_core/object/weapon/weapon.h"

#include "swganh/command/command_service_interface.h"
#include "swganh/command/python_command_creator.h"
#include "swganh/command/base_combat_command.h"
#include "swganh/simulation/simulation_service_interface.h"

#include "swganh_core/messages/controllers/combat_action_message.h"
#include "swganh_core/messages/controllers/combat_spam_message.h"
#include "swganh_core/messages/controllers/show_fly_text.h"
#include "swganh_core/messages/out_of_band.h"
#include "swganh_core/messages/chat_system_message.h"
#include "swganh_core/messages/system_message.h"

using namespace std;
using namespace swganh;
using namespace swganh::app;
using namespace swganh::service;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::combat;
using namespace swganh::command;

using swganh::app::SwganhKernel;

CombatService::CombatService(SwganhKernel* kernel)
: generator_(1, 100)
, active_(kernel->GetIoService())
, kernel_(kernel)
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

void CombatService::Startup()
{
	simulation_service_ = kernel_->GetServiceManager()
        ->GetService<SimulationServiceInterface>("SimulationService");

	command_service_ = kernel_->GetServiceManager()
		->GetService<CommandServiceInterface>("CommandService");
    
    command_service_->AddCommandCreator("attack", swganh::command::PythonCommandCreator("commands.attack", "AttackCommand"));
    command_service_->AddCommandCreator("deathblow", swganh::command::PythonCommandCreator("commands.deathblow", "DeathBlowCommand")); 
    command_service_->AddCommandCreator("defaultattack", swganh::command::PythonCommandCreator("commands.defaultattack", "DefaultAttackCommand"));
    command_service_->AddCommandCreator("duel", swganh::command::PythonCommandCreator("commands.duel", "DuelCommand"));
    command_service_->AddCommandCreator("endduel", swganh::command::PythonCommandCreator("commands.endduel", "EndDuelCommand"));
    command_service_->AddCommandCreator("kneel", swganh::command::PythonCommandCreator("commands.kneel", "KneelCommand"));
    command_service_->AddCommandCreator("berserk1", swganh::command::PythonCommandCreator("commands.berserk1", "Berserk1Command"));
    command_service_->AddCommandCreator("overchargeshot1", swganh::command::PythonCommandCreator("commands.overchargeshot1", "OverchargeShot1Command"));
    command_service_->AddCommandCreator("peace", swganh::command::PythonCommandCreator("commands.peace", "PeaceCommand"));
    command_service_->AddCommandCreator("prone", swganh::command::PythonCommandCreator("commands.prone", "ProneCommand"));
    command_service_->AddCommandCreator("sitserver", swganh::command::PythonCommandCreator("commands.sitserver", "SitServerCommand"));
    command_service_->AddCommandCreator("stand", swganh::command::PythonCommandCreator("commands.stand", "StandCommand"));
}

void CombatService::SendCombatAction(BaseCombatCommand* command)
{    
    CombatData combat_data(command);
    auto actor = std::static_pointer_cast<Creature>(command->GetActor());
    auto target = std::static_pointer_cast<Tangible>(command->GetTarget());

    if (actor != nullptr && target != nullptr && InitiateCombat(actor, target, command->GetCommandName()))
    {
        SingleTargetCombatAction(actor, target, combat_data);
        SendCombatActionMessage(actor, target, combat_data);
    }
}

bool CombatService::InitiateCombat(
    const std::shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    const swganh::HashString& command)
{
    // check to see if we are able to start combat ( are we in peace? )
    if (command == swganh::HashString("peace")) {
        return false;
    }

    if (target == nullptr)
        return false;
    
    if (attacker->GetObjectId() == target->GetObjectId())
    {
        SystemMessage::Send(attacker, OutOfBand("cbt_spam", "shoot_self"));
        return false;
    }

    shared_ptr<Creature> creature_target = nullptr;
    if (target->GetType() == Creature::type)
        creature_target = static_pointer_cast<Creature>(target);
    // Check if mounted

    // Check if target can be attacked
    if (creature_target != nullptr  && (creature_target->IsDead() || creature_target->IsIncapacitated()))
        return false;
    if (!attacker->CanAttack(creature_target.get()))
        return false;

    //@TODO: Base this on weapon range
    if (!attacker->InRange(target->GetPosition(), 25))
        return false;

    // Add Combat
    attacker->ToggleStateOn(COMBAT);
    attacker->ToggleStateOff(PEACE);
    creature_target->ToggleStateOn(COMBAT);
    creature_target->ToggleStateOff(PEACE);

    if (!attacker->IsDefending(creature_target->GetObjectId()))
    {
        attacker->AddDefender(creature_target->GetObjectId());
        
        auto auto_command = command_service_->CreateCommand("defaultattack");
        auto swg_command = std::static_pointer_cast<BaseSwgCommand>(auto_command);
        
        CommandQueueEnqueue request;
        request.observable_id = attacker->GetObjectId();
        request.target_id = creature_target->GetObjectId();

        swg_command->SetCommandRequest(request);
        
        command_service_->SetDefaultCommand(attacker->GetObjectId(), swg_command);
    }
    
    if (!creature_target->IsDefending(attacker->GetObjectId()))
    {
        creature_target->AddDefender(attacker->GetObjectId());
        
        auto auto_command = command_service_->CreateCommand("defaultattack");
        auto swg_command = std::static_pointer_cast<BaseSwgCommand>(auto_command);
        
        CommandQueueEnqueue request;
        request.observable_id = creature_target->GetObjectId();
        request.target_id = attacker->GetObjectId();

        swg_command->SetCommandRequest(request);

        command_service_->SetDefaultCommand(creature_target->GetObjectId(), swg_command);
    }

    return true;
}

bool CombatService::InitiateCombat(
    const std::shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    const CommandQueueEnqueue& command_message)
{
    return InitiateCombat(attacker, target, command_message.command_crc);
}

void CombatService::SendCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    const CommandQueueEnqueue& command_message,
    boost::python::object p_object)
{
    // Grab data from python object
    auto find_iter = combat_properties_map_.find(command_message.command_crc);
        if (find_iter == end(combat_properties_map_))
            return;
    auto command_property = find_iter->second;
    CombatData combat_data(p_object, command_property);
    if (InitiateCombat(attacker, target, command_message))
    {
        string string_hit = "";
        // Check For Hit
        // Combat Spam
        // Check for AOE 
        // if ! AOE
        /*int damage = */SingleTargetCombatAction(attacker, target, combat_data);
        // Apply Special Attack Cost

        // Send Message
        SendCombatActionMessage(attacker, target, combat_data);
        
        // If we ended in combat, re-queue this back into the command queue
        // is AutoAttack
        // if target is creature, then auto-attack back
        shared_ptr<Creature> creature_target = nullptr;
        if (target->GetType() == Creature::type)
            creature_target = static_pointer_cast<Creature>(target);
        // Apply Damage
        //ApplyDamage(attacker, creature_target, combat_data, damage, GetDamagingPool(combat_data));
        //if (command_property.command_name.ident_string() == "attack" && attacker->IsAutoAttacking()) {
        //    command_service_->EnqueueCommand(attacker, target, command_message);
        //    //command_service_->EnqueueCommand(creature_target, attacker, command_message);
        //}
    }
}
int CombatService::SingleTargetCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target, 
    CombatData& properties)
{
    int damage = 0;
    if (target->GetType() == Creature::type)
    {
        auto creature = static_pointer_cast<Creature>(target);
        SingleTargetCombatAction(attacker, creature, properties);
    }
    else
    {
        int pool = GetDamagingPool(properties);
        damage = ApplyDamage(attacker, target, properties, damage, pool);

        BroadcastCombatSpam(attacker, target, properties, damage, CombatData::HIT_spam());
    }
    return damage;
}
int CombatService::SingleTargetCombatAction(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Creature>& defender, 
    CombatData& properties)
{
    // Entertaining?

    int hit = 0;
    float damage_multiplier = properties.damage_multiplier;
    int total_damage = 0;
    int damage = 0;

    if (damage_multiplier != 0)
        damage = 16; /*CalculateDamage(attacker, defender) * damage_multiplier);*/
    else
    {
        damage = 10; // obviously temp
    }
    if (damage_multiplier < 1.0f)
        damage_multiplier = 1.0f;

    hit = GetHitResult(attacker, defender, damage, properties.accuracy_bonus + GetAccuracyModifier(attacker)); 
        
    switch (hit)
    {
    case HIT:
        BroadcastCombatSpam(attacker, defender, properties, damage, CombatData::HIT_spam());
        break;
        // Block
    case BLOCK:
        SendCombatActionMessage(defender, attacker, properties, "block");
        SystemMessage::FlyText(defender, "@combat_effects:block", FlyTextColor::GREEN); 
        BroadcastCombatSpam(attacker, defender, properties, damage, CombatData::BLOCK_spam());
        damage_multiplier = 0.5f;
        break;
    case DODGE:
        // Dodge
        SendCombatActionMessage(defender, attacker, properties, "dodge");
        SystemMessage::FlyText(defender, "@combat_effects:dodge", FlyTextColor::GREEN); 
        damage_multiplier = 0.0f;
        BroadcastCombatSpam(attacker, defender, properties, damage, CombatData::DODGE_spam());
        break;
    case COUNTER:
        SystemMessage::FlyText(defender, "@combat_effects:counterattack", FlyTextColor::GREEN); 
        BroadcastCombatSpam(attacker, defender, properties, damage, CombatData::COUNTER_spam());
        damage_multiplier = 0.0f;
    case MISS:
        // Miss
        SystemMessage::FlyText(defender, "@combat_effects:miss", FlyTextColor::WHITE); 
        BroadcastCombatSpam(attacker, defender, properties, damage, CombatData::MISS_spam());
        damage_multiplier = 0.0f;
        return 0;
    default:
        return 0;
    }
    int pool = GetDamagingPool(properties);
    total_damage = ApplyDamage(attacker, defender, properties, damage, pool);

    // If they aren't auto-attacking they should
    if (!defender->IsAutoAttacking())
        defender->ActivateAutoAttack();

    ApplyStates(attacker, defender, properties);
    
    // Apply Dots
    // Attack Delay?
    return total_damage;
}

uint16_t CombatService::GetHitResult(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Creature>& defender, 
    int damage, int accuracy_bonus)
{
//    int hit_outcome = 0;
    // Get Weapon Attack Type
    float weapon_accuracy = 15.0f;
    // Get Weapon Accuracy Mods

    int attacker_accuracy = GetAccuracyModifier(defender);

    accuracy_bonus += GetAccuracyBonus(attacker);

    float target_defense = 15;//GetDefenseModifier(attacker, target);
    float accuracy_total = GetHitChance(attacker_accuracy + weapon_accuracy, static_cast<float>(accuracy_bonus), target_defense);

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

        accuracy_total = GetHitChance(attacker_accuracy + weapon_accuracy, static_cast<float>(accuracy_bonus), target_defense);
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
uint16_t CombatService::GetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker){
    uint16_t accuracy = 0;
    uint32_t posture = attacker->GetPosture();
    if (posture == CROUCHED)
        accuracy += 16;
    else if (posture == PRONE)
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
    
    if (posture == CROUCHED)
        
        accuracy += 16;
    else if (posture == PRONE)
        accuracy += 25;

    return accuracy;

}
uint16_t CombatService::GetAccuracyModifier(const std::shared_ptr<swganh::object::Tangible>& attacker) { 
    //@TODO: Get weapon calculation modifiers
    // Get Accuracy Modifiers from weapon and add up the modifiers the creature has
    return 0; 
}
uint16_t CombatService::GetAccuracyBonus(const std::shared_ptr<swganh::object::Tangible>& attacker) { 
    // get base attacker accuracy mods

    // give additional mods based on Posture and weapon type
    return 0; 
}
void CombatService::ApplyStates(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target, CombatData& properties) {
    //auto states = move(properties.getStates());
    //for_each(begin(states), end(states),[=](pair<float, string> state){
    //    int generated = generator_.Rand(1, 100);
    //    // Didn't trigger this time
    //    if (generated > state.first)
    //    {
    //        return;
    //    }
    //    // Check recovery timers
    //
    //    // Defender State Modifiers
    //
    //    // Strength of Modifier
    //
    //    // GetHitChance(strength, 0.0f, target_defence);
    //
    //    // Jedi Defences
    //
    //    // Send Message
    //
    //    // Check Equilibrium
    //});
}
float CombatService::GetHitChance(float attacker_accuracy, float attacker_bonus, float target_defence) 
{
    //@TODO: Verify this is the appropriate formula
    return (66.0f + attacker_bonus + (attacker_accuracy - target_defence) / 2.0f);
}
int CombatService::ApplyDamage(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Tangible>& target, 
    CombatData& properties,
    int damage, int pool)
{
    // Sanity Check
    if (damage == 0 || pool < 0)
        return 0;

    //@TODO: Tangible apply damage
    return 0;
}

int CombatService::ApplyDamage(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Creature>& defender,
    CombatData& properties,
    int damage, int pool)
{
    // Sanity Check
    if (damage == 0 || pool < 0)
        return 0;

    damage = generator_.Rand(1, 100);

    float wounds_ratio = 0; /*attacker->GetWeapon()->GetWoundsRatio();*/
    float health_damage = 0.0f, action_damage = 0.0f, mind_damage = 0.0f;
    bool wounded = false;

    // Check For Player && PVP Status
    if (!attacker->CanAttack(defender.get()))
        return 0;


    if (pool == HEALTH) {
        //health_damage = GetArmorReduction(attacker, defender, damage, HEALTH) * damage_multiplier;
        health_damage = damage * properties.damage_multiplier;
        if (defender->GetStatCurrent(HEALTH) - health_damage <= 0)
        {
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(HEALTH, damage);
        // Will this reduce this pool <= 0 ?
        if (!wounded && damage < wounds_ratio) {
            defender->AddStatWound(HEALTH, generator_.Rand(1,2));
            wounded = true;
        }
    }
    if (pool == ACTION) {
        //action_damage = GetArmorReduction(attacker, defender, damage, ACTION) * damage_multiplier;
        action_damage = damage * properties.damage_multiplier;
        if (defender->GetStatCurrent(ACTION) - action_damage <= 0)
        {
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(HEALTH, damage);
        if (!wounded && damage < wounds_ratio) {
            defender->AddStatWound(ACTION, generator_.Rand(1,2));
            wounded = true;
        }
    }
    if (pool == MIND) {
        //mind_damage = GetArmorReduction(attacker, defender, damage, MIND) * damage_multiplier;
        mind_damage = damage * properties.damage_multiplier;
        if (defender->GetStatCurrent(MIND) - mind_damage <= 0)
        {
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(HEALTH, damage);
        if (!wounded && damage < wounds_ratio) {
            defender->AddStatWound(MIND, generator_.Rand(1,2));
            wounded = true;
        }
    }

    if (wounded)
        defender->AddBattleFatigue(1);

    return damage;

}
int CombatService::GetDamagingPool(CombatData& properties)
{
    int pool = 0;
    if (properties.IsRandomPool())
    {
        int randPool = generator_.Rand(0, 2);
        switch (randPool)
        {
            case 0:
                return HEALTH;
            case 1:
                return ACTION;
            case 2:
                return MIND;
        }
    }
    else // Get specific hit chance
    {
        int generated = generator_.Rand(1, 100);
        if (generated < properties.health_hit_chance) {
            LOG(info) << "Damaging Pool picked HEALTH with " << generated << " number and " << properties.health_hit_chance;
            pool = HEALTH;
        }
        else if (generated < properties.action_hit_chance) {
            LOG(info)  << "Damaging Pool picked ACTION with " << generated << " number and " << properties.action_hit_chance;
            pool = ACTION;
        }
        else {
            LOG(info)  << "Damaging Pool picked MIND with " << generated << " number and " << properties.mind_hit_chance;
            pool = MIND;
        }
    }
    return pool;
}

void CombatService::BroadcastCombatSpam(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Tangible>& target, 
    const CombatData& properties,
    uint32_t damage, const string& string_file)
{
    CombatSpamMessage spam;
    spam.attacker_id = attacker->GetObjectId();
    spam.defender_id = target->GetObjectId();
    spam.weapon_id = attacker->GetWeaponId();
    spam.damage = damage;
    spam.file = "cbt_spam";
    if (properties.combat_spam.length() > 0)
        spam.text = properties.combat_spam + string_file;
    attacker->NotifyObservers(&spam);
}

void CombatService::SendCombatActionMessage(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible> & target, 
    CombatData& command_property,
    string animation)
{
        CombatActionMessage cam;
        if ((uint32_t)command_property.animation_crc == 0 && animation.length() == 0)
            cam.action_crc = CombatData::DefaultAttacks[generator_.Rand(0, 9)];
        else
        {
            if (animation.length() > 0)
            {
                cam.action_crc = swganh::HashString(animation);
            }
            else
                cam.action_crc = command_property.animation_crc;
        }
        cam.attacker_id = attacker->GetObjectId();
        cam.weapon_id = attacker->GetWeaponId();
        cam.attacker_end_posture = attacker->GetPosture();
        
        auto defenders = attacker->GetDefenders();
        // build up the defenders
        for(auto& defender : defenders)
        {
            CombatDefender def_list;
            def_list.defender_id = defender.object_id;
            def_list.defender_end_posture = simulation_service_->GetObjectById<Creature>(defender.object_id)->GetPosture();
            def_list.hit_type = 0x1;
            def_list.defender_special_move_effect = 0;
            cam.defender_list.push_back(def_list);
        }
        cam.combat_special_move_effect = 0;
        
        attacker->NotifyObservers(&cam);
}

void CombatService::SetIncapacitated(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    command_service_->ClearDefaultCommand(target->GetObjectId());
    target->SetPosture(INCAPACITATED);
    //@TODO: Get this from config Default Incap Timer 
    target->SetIncapTimer(15);
    EndCombat(attacker, target);

    SystemMessage::Send(attacker, OutOfBand("base_player", "prose_target_incap", TT, target->GetObjectId()));
    if (attacker)
    {
        SystemMessage::Send(target, OutOfBand("base_player", "prose_victim_incap", TT, attacker->GetObjectId()));
    }
    else
	{
        SystemMessage::Send(target, OutOfBand("base_player", "victim_incapacitated"));
	}

    active_.AsyncDelayed(boost::posix_time::seconds(15), [target](){
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
    SystemMessage::Send(attacker, OutOfBand("base_player", "killer_target_dead", TT, target->GetObjectId()));
    SystemMessage::Send(attacker, OutOfBand("base_player", "prose_target_dead", TT, target->GetObjectId()));
    if (attacker)
	{
        SystemMessage::Send(target, OutOfBand("base_player", "prose_victim_dead", TT, attacker->GetObjectId()));
	}
    else
	{
        SystemMessage::Send(target, OutOfBand("base_player", "victim_dead"));
	}
}

void CombatService::EndDuel(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    if (attacker->InDuelList(target->GetObjectId()))
    {
        attacker->ClearAutoAttack();
        attacker->RemoveFromDuelList(target->GetObjectId());
        attacker->RemoveDefender(target->GetObjectId());
        attacker->SetPvPStatus(PvPStatus_Player);
        attacker->ToggleStateOff(COMBAT);
        attacker->SetTargetId(0);
        SystemMessage::Send(attacker, OutOfBand("duel", "end_self", TT, target->GetObjectId()));
        SystemMessage::Send(target, OutOfBand("duel", "end_target", TT, attacker->GetObjectId()));
        // End the Duel for the target as well
        target->ClearAutoAttack();
        target->RemoveFromDuelList(attacker->GetObjectId());
        target->RemoveDefender(attacker->GetObjectId());
        target->SetPvPStatus(PvPStatus_Player);
        target->ToggleStateOff(COMBAT);
        target->SetTargetId(0);
    }
}
void CombatService::EndCombat(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    attacker->ClearAutoAttack();
    attacker->ToggleStateOff(COMBAT);
    // For Some Reason This Crashes...
    attacker->RemoveDefender(target->GetObjectId());    
    command_service_->ClearDefaultCommand(attacker->GetObjectId());

    target->RemoveDefender(attacker->GetObjectId());
    command_service_->ClearDefaultCommand(target->GetObjectId());
    attacker->SetTargetId(0);
    // End Duel
    EndDuel(attacker, target);
}
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
#include "swganh/database/database_manager.h"
#include "swganh/service/service_manager.h"
#include "swganh/scripting/python_instance_creator.h"
#include "swganh/app/swganh_kernel.h"

#include "swganh_core/object/creature/creature.h"
#include "swganh/observer/observer_interface.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh_core/object/weapon/weapon.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/command/base_combat_command.h"

#include "swganh_core/command/command_service_interface.h"
#include "swganh_core/command/base_swg_command.h"
#include "swganh_core/command/command_properties.h"
#include "swganh_core/simulation/simulation_service_interface.h"

#include "swganh_core/equipment/equipment_service.h"
#include "swganh_core/static/static_service.h"
#include "swganh_core/static/skill_manager.h"

#include "swganh_core/messages/controllers/combat_action_message.h"
#include "swganh_core/messages/controllers/combat_spam_message.h"
#include "swganh_core/messages/controllers/show_fly_text.h"
#include "swganh_core/messages/out_of_band.h"
#include "swganh_core/messages/chat/chat_system_message.h"
#include "swganh_core/messages/system_message.h"

using namespace std;
using namespace swganh;
using namespace swganh::app;
using namespace swganh::service;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::equipment;
using namespace swganh::simulation;
using namespace swganh::combat;
using namespace swganh::command;

using swganh::app::SwganhKernel;
using swganh::scripting::PythonInstanceCreator;

CombatService::CombatService(SwganhKernel* kernel)
: generator_(1, 100)
, buff_manager_(kernel)
, active_(kernel->GetCpuThreadPool())
, kernel_(kernel)
{    
    SetServiceDescription(ServiceDescription(
        "CombatService",
        "Combat",
        "0.1",
        "127.0.0.1", 
        0, 
        0,
        0));
}

CombatService::~CombatService()
{}

void CombatService::Initialize()
{
	simulation_service_ = kernel_->GetServiceManager()
        ->GetService<SimulationServiceInterface>("SimulationService");

	command_service_ = kernel_->GetServiceManager()
		->GetService<CommandServiceInterface>("CommandService");
	
	equipment_service_ = simulation_service_->GetEquipmentService();

	static_service_ = kernel_->GetServiceManager()->GetService<swganh::statics::StaticServiceInterface>("StaticService");
}

void CombatService::Startup()
{
	buff_manager_.Start();
}

void CombatService::SendCombatAction(BaseCombatCommand* command)
{    
    auto actor = std::static_pointer_cast<Creature>(command->GetActor());
    auto target = std::static_pointer_cast<Tangible>(command->GetTarget());
	auto weapon = equipment_service_->GetEquippedObject<Weapon>(actor, "hold_r");
    if (actor != nullptr && InitiateCombat(actor, target, weapon, command->combat_data))
    {
		auto attacker_mods = static_service_->GetSkillMods(actor);
		command->combat_data->attacker_skill_mods = attacker_mods;
		if (target->GetType() == Creature::type)
		{
			auto target_mods = static_service_->GetSkillMods(static_pointer_cast<Creature>(target));
			command->combat_data->target_skill_mods = target_mods;
		}
		
		std::vector<CombatDefender> defender_data;
        for (auto& target_ : GetCombatTargets(actor, target, weapon, command->combat_data))
		{
			auto defender = DoCombat(actor, target_, weapon, command->combat_data);
			if (defender.defender_id > 0)
				defender_data.push_back(defender);	
		}
		// Send Combat message
		SendCombatActionMessage(actor, target, weapon, command->combat_data, defender_data);
    }
}

CombatDefender CombatService::DoCombat(
	const shared_ptr<Creature>& attacker,
	const shared_ptr<Tangible>& target,
	const shared_ptr<Weapon>& weapon,
	std::shared_ptr<CombatData> combat_data)
{
	CombatDefender defender;
	string combat_spam, effect;
	FlyTextColor color = GREEN;
	HIT_TYPE hit_type = HIT;
	HIT_LOCATION hit_location;
	int total_damage = 0;

	if (!ApplySpecialCost(attacker, weapon, combat_data))
		return defender;
	if (target->GetType() == Creature::type)
	{
		auto creature_target = static_pointer_cast<Creature>(target);
		hit_type = GetHitResult(attacker, creature_target, weapon, combat_data);
		hit_location = GetHitLocation(combat_data);
		switch (hit_type)
		{
			case HIT:
				combat_spam = CombatData::HIT_spam();
				break;
			case BLOCK:
				combat_spam = CombatData::BLOCK_spam();
				combat_data->damage_multiplier *= 0.5f;
				break;
			case DODGE:
				combat_spam = CombatData::DODGE_spam();
				combat_data->damage_multiplier = 0.0f;
				break;
			case COUNTER:
				effect = "counterattack";
				combat_spam = CombatData::COUNTER_spam();
				combat_data->damage_multiplier = 0.0f;
			case MISS:
				combat_spam = CombatData::MISS_spam();
				combat_data->damage_multiplier = 0.0f;			
				color = WHITE;
                                
			case RICHOCHET:
                        case REFLECT:
				combat_spam = CombatData::BLOCK_spam();
				combat_data->damage_multiplier *= 0.5f;
				break;
                                
		}
	}
	else
	{
		// we always hit tangibles
		combat_spam = CombatData::HIT_spam();
	}
	if (hit_type == HIT)
	{
		float initial_damage = CalculateDamage(attacker, target, combat_data);
		total_damage = ApplyDamage(attacker, target, weapon, combat_data, (int)(initial_damage), hit_location);		
	}
	else
		SystemMessage::FlyText(target,  CombatData::COMBAT_EFFECTS() + combat_spam, color);

	BroadcastCombatSpam(attacker, target, weapon, combat_data, total_damage, combat_spam);
	CombatSpecialMoveEffect csme = ApplyStates(attacker, target, combat_data);
	
	

	// Set Defender values
	defender.defender_id = target->GetObjectId();
	defender.defender_special_move_effect = csme;
	defender.hit_type = hit_type;
	defender.defender_end_posture = simulation_service_->GetObjectById<Creature>(defender.defender_id)->GetPosture();

	return defender;
}

bool CombatService::InitiateCombat(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible>& target,
	const shared_ptr<Weapon>& weapon,
    const shared_ptr<CombatData> combat_data)
{    
    if (attacker->GetObjectId() == target->GetObjectId())
    {
        SystemMessage::Send(attacker, OutOfBand("cbt_spam", "shoot_self"));
        return false;
    }

    shared_ptr<Creature> creature_target = nullptr;
    if (target->GetType() == Creature::type)
        creature_target = static_pointer_cast<Creature>(target);
	// check for too far out of range to disengage combat...
	if (!attacker->InRange(target->GetPosition(), 128.0f))
	{
		EndCombat(attacker, creature_target);
		return false;
	}
	int range = combat_data->range > 0 ? combat_data->range : 7;
	if (weapon)
	{
		int range_ok = weapon->GetAttribute<int32_t>("range");
		if (range_ok > 0 && attacker->InRange(target->GetPosition(), (float)(range_ok)))
		{
			SystemMessage::Send(attacker, OutOfBand("cbt_spam", "out_of_range_single"), false, false);
			return false;
		}
	}
	else
	{
		if (!attacker->InRange(target->GetPosition(), (float)(range)))
		{
			SystemMessage::Send(attacker, OutOfBand("cbt_spam", "out_of_range_single"), false, false);
			return false;
		}
	}
	
    if (!attacker->CanAttack(creature_target.get()))
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
        auto swg_command = std::static_pointer_cast<BaseCombatCommand>(auto_command);
        
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
        auto swg_command = std::static_pointer_cast<BaseCombatCommand>(auto_command);
        
        CommandQueueEnqueue request;
        request.observable_id = creature_target->GetObjectId();
        request.target_id = attacker->GetObjectId();
		
        swg_command->SetCommandRequest(request);

        command_service_->SetDefaultCommand(creature_target->GetObjectId(), swg_command);		
    }
    return true;
}

vector<shared_ptr<Tangible>> CombatService::GetCombatTargets(
	const shared_ptr<Creature>& attacker, 
	const shared_ptr<Tangible> & target, 
	const shared_ptr<Weapon>& weapon,
	shared_ptr<CombatData> combat_data)
{
	std::vector<std::shared_ptr<Tangible>> targets_in_range;

	// area range
	if (combat_data->area_range > 0)
	{
		target->ViewAwareObjects([&targets_in_range, target, combat_data](shared_ptr<Object> obj){
			if (target->InRange(obj->GetPosition(), (float)(combat_data->range)))
			{
				auto tano = static_pointer_cast<Tangible>(obj);
				if (tano)
				{
					targets_in_range.push_back(tano);
				}
			}
		});
	}
	// cone range
	// TODO: Do later
	// Most Attacks are just single target actions
	else
	{
		targets_in_range.push_back(target);
	}

	return targets_in_range;	
}
float CombatService::CalculateDamage(
	const shared_ptr<Creature>& attacker, 
	const shared_ptr<Tangible>& target, 
	shared_ptr<CombatData> combat_data)
{
	return 15.0f;
}

HIT_TYPE CombatService::GetHitResult(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Creature>& defender, 
	const shared_ptr<Weapon>& weapon,
    std::shared_ptr<CombatData> combat_data)
{
	// Accuracy Mods
	int weapon_accuracy = combat_data->weapon_accuracy + (int)(GetWeaponRangeModifier(weapon, attacker->RangeTo(defender->GetPosition())));
	int attacker_accuracy = combat_data->accuracy_bonus;
	attacker_accuracy += weapon_accuracy;
    combat_data->accuracy_bonus += GetAccuracyBonus(attacker, weapon, combat_data);

	int target_defense = 0;
	if (weapon && weapon->GetWeaponType() == WeaponType::MELEE)
	{
		//@TODO EquipmentList : target_defense += defender->GetAttributeRecursive<int>("melee_defense");
	}
	else
	{
		//@TODO EquipmentList : target_defense += defender->GetAttributeRecursive<int>("ranged_defense");
	}
    
    float accuracy_total = GetHitChance(attacker_accuracy, combat_data->accuracy_bonus, target_defense);

    // Scout/Ranger creature hit bonus
	//@TODO EquipmentList : accuracy_total += attacker->GetAttributeRecursive<int>("creature_hit_bonus");

	//@TODO EquipmentList : accuracy_total -= defender->GetAttributeRecursive<int>("dodge_attack");
	//@TODO EquipmentList : accuracy_total -= defender->GetAttributeRecursive<int>("private_center_of_being");

	LOG(warning) << "Final Hit chance is " << accuracy_total << " for attacker " << attacker->GetObjectId() << std::endl;

    if (accuracy_total > 100.0)
        accuracy_total = 100.0;
    else if(accuracy_total < 0.0f)
        accuracy_total = 0.0f;

    if (generator_.Rand(1,100) > accuracy_total) 
        return MISS;

    if (target_defense <= 0)
        return HIT;

    accuracy_total = GetHitChance(attacker_accuracy, combat_data->accuracy_bonus, target_defense);
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
    return HIT;
}
HIT_LOCATION CombatService::GetHitLocation(std::shared_ptr<CombatData> combat_data)
{
	// Determine if it's a scatter attack, default, or targetted pool attack
	HIT_LOCATION location;
	if (combat_data->pool > 0)
	{
		return (HIT_LOCATION)combat_data->pool;
	}
	else
	{
		int generated = generator_.Rand(1, 100);
		int health_hit_chance = combat_data->health_hit_chance == 0 ? 55 : (int)(combat_data->health_hit_chance);
		int action_hit_chance = combat_data->action_hit_chance == 0 ? 32 : (int)(combat_data->action_hit_chance);
		int mind_hit_chance = combat_data->mind_hit_chance == 0 ? 13 : (int)(combat_data->mind_hit_chance);
		
		if (generated < mind_hit_chance){
            LOG(info)  << "Damaging Pool picked MIND with " << generated << " number and " << combat_data->mind_hit_chance;
            location = MIND;
        }
		else if (generated < (action_hit_chance + mind_hit_chance)) {
            LOG(info)  << "Damaging Pool picked ACTION with " << generated << " number and " << action_hit_chance;
            location = ACTION;
        }
        else {
            LOG(info) << "Damaging Pool picked HEALTH with " << generated << " number and " << health_hit_chance;
            location = HEALTH;
        }
        
        
	}
	return location;
}
// http://wiki.swganh.org/index.php/Accuracy_%28Game_Mechanics%29
float CombatService::GetWeaponRangeModifier(const shared_ptr<Weapon>& weapon, float range_to_target)
{
	float min_range = 0;
	float ideal_range = 2;
	float max_range = 5;
	float small_modifier = 6.5;
	float big_modifier = 7;

	if (weapon)
	{
		//@TODO EquipmentList : min_range = weapon->GetAttributeRecursive<float>("wpn_range_zero");
		//@TODO EquipmentList : ideal_range = weapon->GetAttributeRecursive<float>("wpn_range_mid");
		//@TODO EquipmentList : max_range = weapon->GetAttributeRecursive<float>("wpn_range_max");
	}
	float small_range = min_range;
	float big_range = ideal_range;
	if (range_to_target > ideal_range)
	{
		if (weapon)
		{
			//@TODO EquipmentList : small_modifier = weapon->GetAttributeRecursive<float>("wpn_accuracy_mid");	// Get Ideal Accuracy
			//@TODO EquipmentList : big_modifier = weapon->GetAttributeRecursive<float>("wpn_accuracy_max");	// Get Max Range Accuracy
		}
		small_range = ideal_range;
		big_range = max_range;
	} 
	else if (range_to_target <= min_range)
		return small_modifier;
	float overall_modifier = small_modifier + ((range_to_target - small_range) / (big_range - small_range) * (big_modifier - small_modifier));
	if (overall_modifier > 0.0f)
		return overall_modifier;
	else 
		return 0.0f;
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
    
    if (posture == CROUCHED)
        accuracy -= 16;
    else if (posture == PRONE)
        accuracy -= 25;

	// if running +25

    return accuracy;

}

uint16_t CombatService::GetAccuracyBonus(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<Weapon>& weapon, std::shared_ptr<CombatData> combat_data) { 
    uint16_t bonus = 0;
	auto acc_bonus_pair = combat_data->attacker_skill_mods["attack_accuracy"];
	bonus += acc_bonus_pair.first + acc_bonus_pair.second;
	auto priv_acc_bonus = combat_data->attacker_skill_mods["attack_accuracy"];
	bonus += priv_acc_bonus.first + priv_acc_bonus.second;	

	if (weapon)
	{
		if (weapon->GetWeaponType() == WeaponType::RANGED)
		{
			//@TODO EquipmentList : bonus += attacker->GetAttributeRecursive<int>("private_ranged_accuracy_bonus");
		}
		else
		{
			//@TODO EquipmentList : bonus += attacker->GetAttributeRecursive<int>("private_melee_accuracy_bonus");			
		}
	}
	bonus += GetPostureModifier(attacker);

    return bonus; 
}
bool CombatService::ApplySpecialCost(
	const shared_ptr<Creature>& attacker, 
	const shared_ptr<Weapon>& weapon, 
	shared_ptr<CombatData> combat_data)
{
	if (weapon)
	{
		int health_cost = 0; //@TODO EquipmentList : (int)(weapon->GetAttributeRecursive<float>("wpn_attack_cost_health") * combat_data->health_cost_multiplier);
		int action_cost = 0; //@TODO EquipmentList : (int)(weapon->GetAttributeRecursive<float>("wpn_attack_cost_action") * combat_data->action_cost_multiplier);
		int mind_cost = 0; //@TODO EquipmentList : (int)(weapon->GetAttributeRecursive<float>("wpn_attack_cost_mind") * combat_data->mind_cost_multiplier);
		int force_cost = 0; //@TODO EquipmentList : (int)(weapon->GetAttributeRecursive<float>("wpn_attack_cost_force") * combat_data->force_cost_multiplier);

		// Force is in player
		if (force_cost > 0)
		{
			auto player = equipment_service_->GetEquippedObject<Player>(attacker, "ghost");
			int32_t current_force = player->GetCurrentForcePower();
			if (current_force > force_cost)
			{
				player->SetCurrentForcePower(current_force - force_cost);
			}
			else
			{
				SystemMessage::Send(attacker, "jedi_spam", "no_force_power");
				return false;
			}
		}
		if (health_cost > 0 && attacker->GetStatCurrent(StatIndex::HEALTH) <= health_cost)
			return false;
		if (action_cost > 0 && attacker->GetStatCurrent(StatIndex::ACTION) <= action_cost)
			return false;
		if (mind_cost > 0 && attacker->GetStatCurrent(StatIndex::MIND) <= mind_cost)
			return false;
		
		if (health_cost > 0)
			attacker->DeductStatCurrent(StatIndex::HEALTH, health_cost);
		if (action_cost > 0)
			attacker->DeductStatCurrent(StatIndex::ACTION, action_cost);
		if (mind_cost > 0)
			attacker->DeductStatCurrent(StatIndex::MIND, mind_cost);

		// TODO: Weapon decay
	}
	return true;
}
CombatSpecialMoveEffect CombatService::ApplyStates(const shared_ptr<Creature>& attacker, const shared_ptr<Tangible>& target, std::shared_ptr<CombatData> combat_data) {
	return NONE;
    //auto states = move(combat_data->getStates());
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
float CombatService::GetHitChance(int attacker_accuracy, int attacker_bonus, int target_defence) 
{
    return (66.0f + (float)(attacker_bonus) + (float)((attacker_accuracy - target_defence)) / 2.0f);
}
int CombatService::ApplyDamage(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Tangible>& target, 
	const shared_ptr<Weapon>& weapon,
    std::shared_ptr<CombatData> combat_data,
    int damage, HIT_LOCATION pool)
{
	if (target->GetType() == Creature::type)
		return ApplyDamage(attacker, static_pointer_cast<Creature>(target), weapon, combat_data, damage, pool);
    // Sanity Check
    if (damage == 0 || pool < 0)
        return 0;

    //@TODO: Tangible apply damage
    return 0;
}

int CombatService::ApplyDamage(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Creature>& defender,
	const shared_ptr<Weapon>& weapon,
    std::shared_ptr<CombatData> combat_data,
    int damage, HIT_LOCATION pool)
{
    // Sanity Check
    if (damage == 0 || pool < 0)
        return 0;

    damage = generator_.Rand(1, 100);

    float wounds_ratio = 0; /*attacker->GetWeapon()->GetWoundsRatio();*/
    float health_damage = 0.0f, action_damage = 0.0f, mind_damage = 0.0f;
	float damage_multiplier = combat_data->damage_multiplier == 0 ? 1.0f : combat_data->damage_multiplier;
    bool wounded = false;

    // Check For Player && PVP Status
    if (!attacker->CanAttack(defender.get()))
        return 0;


    if (pool == HEALTH) {
        //health_damage = GetArmorReduction(attacker, defender, damage, HEALTH) * damage_multiplier;
        health_damage = damage * damage_multiplier;
        if (defender->GetStatCurrent(StatIndex::HEALTH) - health_damage <= 0)
        {
			defender->SetStatCurrent(StatIndex::HEALTH, 0);
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(StatIndex::HEALTH, (int)(health_damage));
        // Will this reduce this pool <= 0 ?
        if (!wounded && health_damage < wounds_ratio) {
            defender->AddStatWound(StatIndex::HEALTH, generator_.Rand(1,2));
            wounded = true;
        }
    }
    if (pool == ACTION) {
        //action_damage = GetArmorReduction(attacker, defender, damage, ACTION) * damage_multiplier;
        action_damage = damage * damage_multiplier;
        if (defender->GetStatCurrent(StatIndex::ACTION) - action_damage <= 0)
        {
			defender->SetStatCurrent(StatIndex::ACTION, 0);
            SetIncapacitated(attacker, defender);			
        }
        else
            defender->DeductStatCurrent(StatIndex::ACTION, (int)(action_damage));
        if (!wounded && action_damage < wounds_ratio) {
            defender->AddStatWound(StatIndex::ACTION, generator_.Rand(1,2));
            wounded = true;
        }
    }
    if (pool == MIND) {
        //mind_damage = GetArmorReduction(attacker, defender, damage, MIND) * damage_multiplier;
        mind_damage = damage * damage_multiplier;
        if (defender->GetStatCurrent(StatIndex::MIND) - mind_damage <= 0)
        {
			defender->SetStatCurrent(StatIndex::MIND, 0);
            SetIncapacitated(attacker, defender);
        }
        else
            defender->DeductStatCurrent(StatIndex::MIND, (int)(mind_damage));
        if (!wounded && mind_damage < wounds_ratio) {
            defender->AddStatWound(StatIndex::MIND, generator_.Rand(1,2));
            wounded = true;
        }
    }

    if (wounded)
        defender->AddBattleFatigue(1);

    return (int)(health_damage + action_damage + mind_damage);

}

void CombatService::BroadcastCombatSpam(
    const shared_ptr<Creature>& attacker,
    const shared_ptr<Tangible>& target, 
	const shared_ptr<Weapon>& weapon,
    const std::shared_ptr<CombatData> combat_data,
    uint32_t damage, const string& string_file)
{
    CombatSpamMessage spam;
    spam.attacker_id = attacker->GetObjectId();
    spam.defender_id = target->GetObjectId();
	if (weapon)
	{
		if (weapon->GetWeaponType() == RANGED)
			spam.text = "shoot_" + string_file;
		else
			spam.text = "melee_" + string_file;
		spam.weapon_id = weapon->GetObjectId();
	}
	else
	{
		spam.weapon_id = attacker->GetWeaponId();
		spam.text ="attack_" + string_file;
	}
    spam.damage = damage;
    spam.file = "cbt_spam";
    if (combat_data->combat_spam.length() > 0)
        spam.text = combat_data->combat_spam + "_"  + string_file;
	
    attacker->NotifyObservers(&spam);
}

void CombatService::SendCombatActionMessage(
    const shared_ptr<Creature>& attacker, 
    const shared_ptr<Tangible> & target, 
	const shared_ptr<Weapon>& weapon,
    std::shared_ptr<CombatData> command_property,
	std::vector<CombatDefender> defenders)
{
        CombatActionMessage cam;		
        cam.attacker_id = attacker->GetObjectId();
        cam.weapon_id = attacker->GetWeaponId();
        cam.attacker_end_posture = attacker->GetPosture();

        if (weapon)
		{
			if (weapon->GetWeaponType() == WeaponType::RANGED)
			{
				// Temp
				if ((uint32_t)command_property->animation_crc == 0)
					cam.action_crc = 1349426508;					
				else
					cam.action_crc = command_property->animation_crc;				
			}
			cam.weapon_id = weapon->GetObjectId();
			cam.trails_bit_flag = WEAPON_TRAIL;
		}
		else
		{
			if ((uint32_t)command_property->animation_crc == 0)
				cam.action_crc = CombatData::DefaultAttacks[generator_.Rand(0, 9)];
			else
			{
				cam.action_crc = command_property->animation_crc;
			}
		}
        // build up the defenders
        for(auto& defender : defenders)
        {
            CombatDefender def_list;
            def_list.defender_id = defender.defender_id;
            def_list.defender_end_posture = defender.defender_end_posture;
            def_list.hit_type = defender.hit_type;
			def_list.defender_special_move_effect = defender.defender_special_move_effect;
            cam.defender_list.push_back(def_list);
        }        
        attacker->NotifyObservers(&cam);
}

void CombatService::SetIncapacitated(const shared_ptr<Creature>& attacker, const shared_ptr<Creature>& target)
{
    command_service_->ClearDefaultCommand(target->GetObjectId());
    target->SetPosture(INCAPACITATED);
    //@TODO: Get this from config Default Incap Timer 
	// Incap Timer
    target->SetCounter(15);
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
		// Incap Timer
        target->SetCounter(0);
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
	target->ToggleStateOff(COMBAT);
    command_service_->ClearDefaultCommand(target->GetObjectId());
    
    // End Duel
    EndDuel(attacker, target);
}
/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_
#define SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;


struct CreatureWrapper : Creature,  wrapper<Creature>
{
};

void exportCreature()
{
	// ENUMS
	enum_<Posture>("POSTURE", "The Possible postures a creature can be in")
		.value("UPRIGHT", UPRIGHT)
		.value("CROUCHED", CROUCHED)
		.value("PRONE", PRONE)
		.value("SNEAKING", SNEAKING)
		.value("BLOCKING", BLOCKING)
		.value("CLIMBING", CLIMBING)
		.value("LYING_DOWN", LYING_DOWN)
		.value("SITTING", SITTING)
		.value("SKILL_ANIMATING", SKILL_ANIMATING)
		.value("DRIVING_VEHICLE", DRIVING_VEHICLE)
		.value("RIDING_CREATURE", RIDING_CREATURE)
		.value("KNOCKED_DOWN", KNOCKED_DOWN)
		.value("INCAPACITATED", INCAPACITATED)
		.value("DEAD", DEAD)
		;

	enum_<State>("ACTION", "State Bitmask, the creature can be in multiple of these")
		.value("NONE", NONE)
		.value("COVER", COVER)
		.value("COMBAT", COMBAT)
		.value("PEACE", PEACE)
		.value("AIMING", AIMING)
		.value("ALERT", ALERT)
		.value("BERSERK", BERSERK)
		.value("FEIGN_DEATH", FEIGN_DEATH)
		.value("COMBAT_ATTITUDE_EVASIVE", COMBAT_ATTITUDE_EVASIVE)
		.value("COMBAT_ATTITUDE_NORMAL", COMBAT_ATTITUDE_NORMAL)
		.value("COMBAT_ATTITUDE_AGGRESSIVE", COMBAT_ATTITUDE_AGGRESSIVE)
		.value("TUMBLING", TUMBLING)
		.value("RALLIED", RALLIED)
		.value("STUNNED", STUNNED)
		.value("BLINDED", BLINDED)
		.value("DIZZY", DIZZY)
		.value("INTIMIDATED", INTIMIDATED)
		.value("IMMOBILIZED", IMMOBILIZED)
		.value("FROZEN", FROZEN)
		.value("SWIMMING", SWIMMING)
		.value("SITTING_ON_CHAIR", SITTING_ON_CHAIR)
		.value("CRAFTING", CRAFTING)
		.value("GLOWING_JEDI", GLOWING_JEDI)
		.value("MASK_SCENT", MASK_SCENT)
		.value("POISONED", POISONED)
		.value("BLEEDING", BLEEDING)
		.value("DISEASED", DISEASED)
		.value("ON_FIRE", ON_FIRE)
		.value("RIDING_MOUNT", RIDING_MOUNT)
		.value("MOUNTED_CREATURE", MOUNTED_CREATURE)

		//@TODO: NEED TO FIND OUT HOW TO GET BOOST PYTHON TO ALLOW UINT64_T VALUES
		/*.value("PILOTING_SHIP", PILOTING_SHIP)
		.value("SHIP_OPERATIONS", SHIP_OPERATIONS)
		.value("SHIP_GUNNER", SHIP_GUNNER)
		.value("SHIP_INTERIOR", SHIP_INTERIOR)
		.value("PILOTING_POB_SHIP", PILOTING_POB_SHIP)*/
		;
	enum_<PvpStatus>("PVPSTATUS", "The PvPStatus of the creature, this controls if a creature is attackable.")
		.value("PvPStatus_None", PvPStatus_None)
		.value("PvPStatus_Attackable", PvPStatus_Attackable)
		.value("PvPStatus_Aggressive", PvPStatus_Aggressive)
		.value("PvPStatus_Overt", PvPStatus_Overt)
		.value("PvPStatus_Tef", PvPStatus_Tef)
		.value("PvPStatus_Player", PvPStatus_Player)
		.value("PvPStatus_Enemy", PvPStatus_Enemy)
		.value("PvPStatus_Duel", PvPStatus_Duel)
		;
	enum_<StatIndex>("STATS", "The different stats a creature can have")
		.value("HEALTH", HEALTH)
		.value("STRENGTH", STRENGTH)
		.value("CONSTITUTION", CONSTITUTION)
		.value("ACTION", ACTION)
		.value("QUICKNESS", QUICKNESS)
		.value("STAMINA", STAMINA)
		.value("MIND", MIND)
		.value("FOCUS", FOCUS)
		.value("WILLPOWER", WILLPOWER)
		;

	class_<EquipmentItem>("EquipmentItem", "The Equipment Object")
		.def(init<uint64_t, uint32_t, std::string, uint32_t>())
		.def_readwrite("object_id", &EquipmentItem::object_id)
		.def_readwrite("template_crc", &EquipmentItem::template_crc)
		.def_readwrite("customization", &EquipmentItem::customization)
		.def_readwrite("containment_type", &EquipmentItem::containment_type)
		;
	class_<SkillMod>("SkillMod", "The Skill Mod Object")
		.def(init<std::string, uint32_t, uint32_t>())
		.def_readwrite("identifier", &SkillMod::identifier)
		.def_readwrite("base", &SkillMod::base)
		.def_readwrite("modifier", &SkillMod::modifier)
		.def("__eq__", &SkillMod::operator==)
		;
	class_<CreatureWrapper, bases<Tangible>, std::shared_ptr<Creature>, boost::noncopyable>("Creature", "Object that describes the creature object, child of Tangible")
		// STATS
		.def("set_stat_base", &CreatureWrapper::SetStatBase, "sets the base :class:`.STATS` value")
		.def("add_stat_base", &CreatureWrapper::AddStatBase, "adds to the given :class:`.STATS`")
		.def("deduct_stat_base", &CreatureWrapper::DeductStatBase, "deducts from the :class:`.STATS`")
		.def("get_stat_base", &CreatureWrapper::GetStatBase, "gets the value of the :class:`.STATS`")
		.def("set_stat_current", &CreatureWrapper::SetStatCurrent, "sets the current :class:`.STATS`")
		.def("add_stat_current", &CreatureWrapper::AddStatCurrent, "adds to the given :class:`.STATS`")
		.def("deduct_stat_current", &CreatureWrapper::DeductStatCurrent, "deducts from the current :class:`.STATS`")
		.def("get_stat_current", &CreatureWrapper::GetStatCurrent, "gets the value of the current :class:`.STATS`")
		.def("add_stat_wound", &CreatureWrapper::AddStatWound, "adds to the given wound :class:`.STATS`")
		.def("deduct_stat_wound", &CreatureWrapper::DeductStatWound, "deducts from the given wound :class:`.STATS`")
		.def("get_stat_wound", &CreatureWrapper::GetStatWound, "gets the value of the current wound :class:`.STATS`")
		.def("add_stat_encumberance", &CreatureWrapper::AddStatEncumberance, "adds to the given encumberance :class:`.STATS`")
		.def("deduct_stat_encumberance", &CreatureWrapper::DeductStatEncumberance, "deducts from the given encumberance :class:`.STATS`")
		.def("get_stat_encumberance", &CreatureWrapper::GetStatEncumberance, "gets the value of the current encumberance :class:`.STATS`")
		.def("set_stat_max", &CreatureWrapper::SetStatMax, "sets the given MAX :class:`.STATS`")
		.def("add_stat_max", &CreatureWrapper::AddStatMax, "adds to the given MAX :class:`.STATS`")
		.def("deduct_stat_max", &CreatureWrapper::DeductStatMax, "deducts from the given MAX :class:`.STATS`")
		.def("get_stat_max", &CreatureWrapper::GetStatMax, "gets the value of the current MAX :class:`.STATS`")
		//
		.def("add_skill", &CreatureWrapper::AddSkill, "adds skill to the creature")
		.def("remove_skill", &CreatureWrapper::RemoveSkill, "removes skill from the creature")
		.def("has_skill", &CreatureWrapper::HasSkill, "returns true if the creature has the skill")
		.def("add_skill_mod", &CreatureWrapper::AddSkillMod, "adds skill mod to the creature")
		.def("remove_skill_mod", &CreatureWrapper::RemoveSkillMod, "removes skill mod from the creature")
		.def("get_skill_mod", &CreatureWrapper::GetSkillMod, "gets the current value of the skill mod")
		.add_property("bank_credits", &CreatureWrapper::GetBankCredits, &CreatureWrapper::SetBankCredits, "Gets and Sets the credits that a creature has in their bank")
		.add_property("cash_credits", &CreatureWrapper::GetCashCredits, &CreatureWrapper::SetCashCredits, "Gets and Sets the credits that a creature has on themselves")
		.add_property("posture", &CreatureWrapper::GetPosture, &CreatureWrapper::SetPosture, "Gets and Sets the posture of the creature, :class:`.POSTURE`")
        .def("is_dead", &CreatureWrapper::IsDead, "returns true if this creature is dead")
        .def("is_incap", &CreatureWrapper::IsIncapacitated, "returns true if this creature is incapacitated")
        .add_property("faction_rank", &CreatureWrapper::GetFactionRank, &CreatureWrapper::SetFactionRank, "Gets and Sets the faction rank for this creature")
		.add_property("owner_id", &CreatureWrapper::GetOwnerId, &CreatureWrapper::SetOwnerId, "Gets and Sets the owner id for the creature")
		.add_property("scale", &CreatureWrapper::GetScale, &CreatureWrapper::SetScale, "Gets and Sets the scale of the creature 1-10")
		.add_property("battle_fatigue", &CreatureWrapper::GetBattleFatigue, &CreatureWrapper::SetBattleFatigue, "Gets and Sets the battle fatigue of the creature")
		.add_property("state_bitmask", &CreatureWrapper::GetStateBitmask, &CreatureWrapper::SetStateBitmask, "Gets and Sets the state bitmask see :class:`.ACTION`")
        .def("has_state", &CreatureWrapper::HasState, "Checks to see if the state :class:`.ACTION` exists in the bitmask")
        .def("toggle_state", &CreatureWrapper::ToggleStateBitmask, "Toggles the provided :class:`.ACTION` State")
        .def("toggle_state_on", &CreatureWrapper::ToggleStateOn, "Toggles the provided :class:`.ACTION` State On")
        .def("toggle_state_off", &CreatureWrapper::ToggleStateOff, "Toggles the provided :class:`.ACTION` State Off")
		.add_property("acceleration_multiplayer_base", &CreatureWrapper::GetAccelerationMultiplierBase, &CreatureWrapper::SetAccelerationMultiplierBase, "Gets and Sets the base acceleration multiplier")
		.add_property("acceleration_multiplayer_modifier", &CreatureWrapper::GetAccelerationMultiplierModifier, &CreatureWrapper::SetAccelerationMultiplierModifier, "Gets and Sets the acceleration multiplier modifier")
		.add_property("listen_to_id", &CreatureWrapper::GetListenToId, &CreatureWrapper::SetListenToId, "Gets and Sets who this creature is listening to, see Entertainer")
		.add_property("run_speed", &CreatureWrapper::GetRunSpeed, &CreatureWrapper::SetRunSpeed, "Gets and Sets the run speed of the creature")
		.add_property("slope_modifier_angle", &CreatureWrapper::GetSlopeModifierAngle, &CreatureWrapper::SetSlopeModifierAngle, "Gets and Sets the slope modifier angle, used to determine terrain negotiation")
		.add_property("slope_modifier_percent", &CreatureWrapper::GetSlopeModifierPercent, &CreatureWrapper::SetSlopeModifierPercent, "Gets and Sets the slope modifier percent, used to determine terrain negotiation")
		.add_property("turn_radius", &CreatureWrapper::GetTurnRadius, &CreatureWrapper::SetTurnRadius, "Gets and Sets the turn radius, when using vehicles")
		.add_property("walking_speed", &CreatureWrapper::GetWalkingSpeed, &CreatureWrapper::SetWalkingSpeed, "Gets and Sets the default walking speed of the creature")
		.add_property("water_modifier_percent", &CreatureWrapper::GetWaterModifierPercent, &CreatureWrapper::SetWaterModifierPercent, "Gets and Sets the water modifier percent")
		.add_property("combat_level", &CreatureWrapper::GetCombatLevel, &CreatureWrapper::SetCombatLevel, "Gets and Sets the combat level of the creature")
		.add_property("animation", &CreatureWrapper::GetAnimation, &CreatureWrapper::SetAnimation, "Gets and Sets the current animation of the player")
		.add_property("mood_animation", &CreatureWrapper::GetMoodAnimation, &CreatureWrapper::SetMoodAnimation, "Gets and Sets the current mood animation of the player")
		.add_property("weapon_id", &CreatureWrapper::GetWeaponId, &CreatureWrapper::SetWeaponId, "Gets and Sets the weapon id of the creature")
		.add_property("group_id", &CreatureWrapper::GetGroupId, &CreatureWrapper::SetGroupId, "Gets and Sets the current group id of the creature")
		.add_property("invite_sender_id", &CreatureWrapper::GetInviteSenderId, &CreatureWrapper::SetInviteSenderId, "Gets and Sets the invite sender id of the last person to send a group invite to the creature")
		.add_property("guild_id", &CreatureWrapper::GetGuildId, &CreatureWrapper::SetGuildId, "Gets and Sets the guild id of the creature")
		.add_property("target_id", &CreatureWrapper::GetTargetId, &CreatureWrapper::SetTargetId, "Gets and Sets the target of the current creature by id")
		.add_property("mood_id", &CreatureWrapper::GetMoodId, &CreatureWrapper::SetMoodId, "Gets and Sets the current mood of the creature")
		.add_property("disguise", &CreatureWrapper::GetDisguise, &CreatureWrapper::SetDisguise, "Gets and Sets the disguise of the current creature, this makes the creature look like the given iff file")
		.add_property("stationary", &CreatureWrapper::SetStationary, &CreatureWrapper::SetStationary, "Gets and Sets if the creature can move or not")
        .add_property("pvp_status", &CreatureWrapper::GetPvpStatus, &CreatureWrapper::SetPvPStatus, "Gets and Sets the :class:`.PVPSTATUS` of the creature")
        .def("in_duel_list", &CreatureWrapper::InDuelList, "Returns a boolean based on if the creature is currently dueling the target")
        .def("add_duel_list", &CreatureWrapper::AddToDuelList, "Adds the creature id to the duel list")
        .def("remove_duel_list", &CreatureWrapper::RemoveFromDuelList, "Removes the creature from the duel list")
        ;
}

#endif  //SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_
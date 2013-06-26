// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_
#define SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_

#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

boost::python::tuple AddBuff(std::string buff_name, uint32_t duration=0)
{
	return boost::python::make_tuple(buff_name, duration);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(addBuffOverload, AddBuff, 1, 2)

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
	class_<Creature, bases<Tangible>, std::shared_ptr<Creature>, boost::noncopyable> c("Creature", "Object that describes the creature object, child of Tangible");
		// STATS
		c.def("setStatBase", (void(Creature::*)(uint16_t, int32_t)) &Creature::SetStatBase, "sets the base :class:`.STATS` value")
		;c.def("addStatBase", (void(Creature::*)(uint16_t, int32_t)) &Creature::AddStatBase, "adds to the given :class:`.STATS`")
		;c.def("deductStatBase", (void(Creature::*)(uint16_t, int32_t)) &Creature::DeductStatBase, "deducts from the :class:`.STATS`")
		;c.def("getStatBase", (int32_t(Creature::*)(uint16_t)) &Creature::GetStatBase, "gets the value of the :class:`.STATS`")
		;c.def("setStatCurrent", (void(Creature::*)(uint16_t, int32_t)) &Creature::SetStatCurrent, "sets the current :class:`.STATS`")
		;c.def("addStatCurrent", (void(Creature::*)(uint16_t, int32_t)) &Creature::AddStatCurrent, "adds to the given :class:`.STATS`")
		;c.def("deductStatCurrent", (void(Creature::*)(uint16_t, int32_t)) &Creature::DeductStatCurrent, "deducts from the current :class:`.STATS`")
		;c.def("getStatCurrent", (int32_t(Creature::*)(uint16_t)) &Creature::GetStatCurrent, "gets the value of the current :class:`.STATS`")
		;c.def("setStatWound", (void(Creature::*)(uint16_t, int32_t)) &Creature::SetStatWound, "sets the wound :class:`STATS`")
		;c.def("addStatWound", (void(Creature::*)(uint16_t, int32_t)) &Creature::AddStatWound, "adds to the given wound :class:`.STATS`")
		;c.def("deductStatWound", (void(Creature::*)(uint16_t, int32_t)) &Creature::DeductStatWound, "deducts from the given wound :class:`.STATS`")
		;c.def("getStatWound", (int32_t(Creature::*)(uint16_t)) &Creature::GetStatWound, "gets the value of the current wound :class:`.STATS`")
		;c.def("setStatEncumberance", (void(Creature::*)(uint16_t, int32_t)) &Creature::SetStatEncumberance, "sets the encumberance :class:`STATS`")
		;c.def("addStatEncumberance", (void(Creature::*)(uint16_t, int32_t)) &Creature::AddStatEncumberance, "adds to the given encumberance :class:`.STATS`")
		;c.def("deductStatEncumberance", (void(Creature::*)(uint16_t, int32_t)) &Creature::DeductStatEncumberance, "deducts from the given encumberance :class:`.STATS`")
		;c.def("getStatEncumberance", (int32_t(Creature::*)(uint16_t)) &Creature::GetStatEncumberance, "gets the value of the current encumberance :class:`.STATS`")
		;c.def("setStatMax", (void(Creature::*)(uint16_t, int32_t)) &Creature::SetStatMax, "sets the given MAX :class:`.STATS`")
		;c.def("addStatMax", (void(Creature::*)(uint16_t, int32_t)) &Creature::AddStatMax, "adds to the given MAX :class:`.STATS`")
		;c.def("deductStatMax", (void(Creature::*)(uint16_t, int32_t)) &Creature::DeductStatMax, "deducts from the given MAX :class:`.STATS`")
		;c.def("getStatMax", (int32_t(Creature::*)(uint16_t)) &Creature::GetStatMax, "gets the value of the current MAX :class:`.STATS`")
		;c.def("setAllStats", (void(Creature::*)(int32_t)) &Creature::SetAllStats, "Sets HAM of base/current/max to a given value")
		;c.def("addSkill", (void(Creature::*)(std::string)) &Creature::AddSkill, "adds skill to the creature")
		;c.def("removeSkill", (void(Creature::*)(std::string)) &Creature::RemoveSkill, "removes skill from the creature")
		;c.def("hasSkill", (bool(Creature::*)(std::string)) &Creature::HasSkill, "returns true if the creature has the skill")
		;c.def("addSkillMod", (void(Creature::*)(SkillMod)) &Creature::AddSkillMod, "adds skill mod to the creature")
		;c.def("removeSkillMod", (void(Creature::*)(std::string)) &Creature::RemoveSkillMod, "removes skill mod from the creature")
		;c.def("setSkillMod", (void(Creature::*)(SkillMod)) &Creature::SetSkillMod, "sets skill mod to the creature")
		;c.def("getSkillMod", (SkillMod(Creature::*)(std::string)) &Creature::GetSkillMod, "gets the current value of the skill mod")
		;c.add_property("bank_credits", (uint32_t(Creature::*)()) &Creature::GetBankCredits, (void(Creature::*)(uint32_t)) &Creature::SetBankCredits, "Gets and Sets the credits that a creature has in their bank")
		;c.add_property("cash_credits", (uint32_t(Creature::*)()) &Creature::GetCashCredits, (void(Creature::*)(uint32_t)) &Creature::SetCashCredits, "Gets and Sets the credits that a creature has on themselves")
		;c.add_property("posture", (Posture(Creature::*)()) &Creature::GetPosture, (void(Creature::*)(Posture)) &Creature::SetPosture, "Gets and Sets the posture of the creature, :class:`.POSTURE`")
		;c.def("isDead", (bool(Creature::*)()) &Creature::IsDead, "returns true if this creature is dead")
        ;c.def("isIncap", (bool(Creature::*)()) &Creature::IsIncapacitated, "returns true if this creature is incapacitated")
		;c.add_property("faction_rank", (uint8_t(Creature::*)()) &Creature::GetFactionRank, (void(Creature::*)(uint8_t)) &Creature::SetFactionRank, "Gets and Sets the faction rank for this creature")
		;c.add_property("owner_id", (uint64_t(Creature::*)()) &Creature::GetOwnerId, (void(Creature::*)(uint64_t)) &Creature::SetOwnerId, "Gets and Sets the owner id for the creature")
		;c.add_property("scale", (float(Creature::*)()) &Creature::GetScale, (void(Creature::*)(float)) &Creature::SetScale, "Gets and Sets the scale of the creature 1-10")
		;c.add_property("battle_fatigue", (uint32_t(Creature::*)()) &Creature::GetBattleFatigue, (void(Creature::*)(uint32_t)) &Creature::SetBattleFatigue, "Gets and Sets the battle fatigue of the creature")
		;c.add_property("state_bitmask", (uint64_t(Creature::*)()) &Creature::GetStateBitmask, (void(Creature::*)(uint64_t)) &Creature::SetStateBitmask, "Gets and Sets the state bitmask see :class:`.ACTION`")
        ;c.def("hasState", (bool(Creature::*)(uint64_t)) &Creature::HasState, "Checks to see if the state :class:`.ACTION` exists in the bitmask")
        ;c.def("toggleState", (void(Creature::*)(uint64_t)) &Creature::ToggleStateBitmask, "Toggles the provided :class:`.ACTION` State")
        ;c.def("toggleStateOn", (void(Creature::*)(uint64_t)) &Creature::ToggleStateOn, "Toggles the provided :class:`.ACTION` State On")
        ;c.def("toggleStateOff", (void(Creature::*)(uint64_t)) &Creature::ToggleStateOff, "Toggles the provided :class:`.ACTION` State Off")
		;c.add_property("acceleration_multiplayer_base", (float(Creature::*)()) &Creature::GetAccelerationMultiplierBase, (void(Creature::*)(float)) &Creature::SetAccelerationMultiplierBase, "Gets and Sets the base acceleration multiplier")
		;c.add_property("acceleration_multiplayer_modifier", (float(Creature::*)()) &Creature::GetAccelerationMultiplierModifier, (void(Creature::*)(float)) &Creature::SetAccelerationMultiplierModifier, "Gets and Sets the acceleration multiplier modifier")
		;c.add_property("listen_to_id", (uint64_t(Creature::*)()) &Creature::GetListenToId, (void(Creature::*)(uint64_t)) &Creature::SetListenToId, "Gets and Sets who this creature is listening to, see Entertainer")
		;c.add_property("run_speed", (float(Creature::*)()) &Creature::GetRunSpeed, (void(Creature::*)(float)) &Creature::SetRunSpeed, "Gets and Sets the run speed of the creature")
		;c.add_property("slope_modifier_angle", (float(Creature::*)()) &Creature::GetSlopeModifierAngle, (void(Creature::*)(float)) &Creature::SetSlopeModifierAngle, "Gets and Sets the slope modifier angle, used to determine terrain negotiation")
		;c.add_property("slope_modifier_percent", (float(Creature::*)()) &Creature::GetSlopeModifierPercent, (void(Creature::*)(float)) &Creature::SetSlopeModifierPercent, "Gets and Sets the slope modifier percent, used to determine terrain negotiation")
		;c.add_property("turn_radius", (float(Creature::*)()) &Creature::GetTurnRadius, (void(Creature::*)(float)) &Creature::SetTurnRadius, "Gets and Sets the turn radius, when using vehicles")
		;c.add_property("walking_speed", (float(Creature::*)()) &Creature::GetWalkingSpeed, (void(Creature::*)(float)) &Creature::SetWalkingSpeed, "Gets and Sets the default walking speed of the creature")
		;c.add_property("water_modifier_percent", (float(Creature::*)()) &Creature::GetWaterModifierPercent, (void(Creature::*)(float)) &Creature::SetWaterModifierPercent, "Gets and Sets the water modifier percent")
		;c.add_property("combat_level", (uint16_t(Creature::*)()) &Creature::GetCombatLevel, (void(Creature::*)(uint16_t)) &Creature::SetCombatLevel, "Gets and Sets the combat level of the creature")
		;c.add_property("animation", (std::string(Creature::*)()) &Creature::GetAnimation, (void(Creature::*)(std::string)) &Creature::SetAnimation, "Gets and Sets the current animation of the player")
		;c.add_property("mood_animation", (std::string(Creature::*)()) &Creature::GetMoodAnimation, (void(Creature::*)(std::string)) &Creature::SetMoodAnimation, "Gets and Sets the current mood animation of the player")
		;c.add_property("weapon_id", (uint64_t(Creature::*)()) &Creature::GetWeaponId, (void(Creature::*)(uint64_t)) &Creature::SetWeaponId, "Gets and Sets the weapon id of the creature")
		;c.add_property("group_id", (uint64_t(Creature::*)()) &Creature::GetGroupId, (void(Creature::*)(uint64_t)) &Creature::SetGroupId, "Gets and Sets the current group id of the creature")
		;c.add_property("invite_sender_id", (uint64_t(Creature::*)()) &Creature::GetInviteSenderId, (void(Creature::*)(uint64_t)) &Creature::SetInviteSenderId, "Gets and Sets the invite sender id of the last person to send a group invite to the creature")
		;c.add_property("guild_id",(uint32_t(Creature::*)())  &Creature::GetGuildId, (void(Creature::*)(uint32_t)) &Creature::SetGuildId, "Gets and Sets the guild id of the creature")
		;c.add_property("target_id", (uint64_t(Creature::*)()) &Creature::GetTargetId, (void(Creature::*)(uint64_t)) &Creature::SetTargetId, "Gets and Sets the target of the current creature by id")
		;c.add_property("mood_id", (uint8_t(Creature::*)()) &Creature::GetMoodId, (void(Creature::*)(uint8_t)) &Creature::SetMoodId, "Gets and Sets the current mood of the creature")
		;c.add_property("disguise", (std::string(Creature::*)()) &Creature::GetDisguise, (void(Creature::*)(std::string)) &Creature::SetDisguise, "Gets and Sets the disguise of the current creature, this makes the creature look like the given iff file")
		;c.add_property("stationary", (bool(Creature::*)()) &Creature::IsStationary, (void(Creature::*)(bool)) &Creature::SetStationary, "Gets and Sets if the creature can move or not")
        ;c.add_property("pvp_status", (PvpStatus(Creature::*)() const) &Creature::GetPvpStatus, (void(Creature::*)(PvpStatus)) &Creature::SetPvPStatus, "Gets and Sets the :class:`.PVPSTATUS` of the creature")
        ;c.def("inDuelList", (bool(Creature::*)(uint64_t)) &Creature::InDuelList, "Returns a boolean based on if the creature is currently dueling the target")
        ;c.def("addDuelList", (void(Creature::*)(uint64_t)) &Creature::AddToDuelList, "Adds the creature id to the duel list")
        ;c.def("removeDuelList", (void(Creature::*)(uint64_t)) &Creature::RemoveFromDuelList, "Removes the creature from the duel list")
		;c.def("addBuff", (void(Creature::*)(std::string, uint32_t)) &Creature::AddBuff, addBuffOverload(args("buff_name", "duration"), "Adds a new buff to the creature"))
		;c.def("removeBuff", (void(Creature::*)(std::string)) &Creature::RemoveBuff, "Instantly removes a buff from the creature")
		;c.def("setCustomization", (void(Creature::*)(const std::string&)) &Creature::SetCustomization, "Sets customization string")
        ;

	implicitly_convertible<std::shared_ptr<Creature>, std::shared_ptr<Tangible>>();
	implicitly_convertible<std::shared_ptr<Creature>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Creature>, std::shared_ptr<ContainerInterface>>();
}

#endif  //SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_
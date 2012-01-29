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
#include "swganh/object/object.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object::creature;


struct CreatureWrapper : Creature,  wrapper<Creature>
{
};

void exportCreature()
{
	// ENUMS
	enum_<Posture>("POSTURE")
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

	class_<CreatureWrapper, bases<Object>, boost::noncopyable>("Creature")
		.add_property("bank_credits", &CreatureWrapper::GetBankCredits, &CreatureWrapper::SetBankCredits, "Gets and Sets the credits that a creature has in their bank")
		.add_property("cash_credits", &CreatureWrapper::GetCashCredits, &CreatureWrapper::SetCashCredits, "Gets and Sets the credits that a creature has on themselves")
		.add_property("posture", &CreatureWrapper::GetPosture, &CreatureWrapper::SetPosture, "Gets and Sets the posture of the creature, see POSTURE enum")
		.add_property("faction_rank", &CreatureWrapper::GetFactionRank, &CreatureWrapper::SetFactionRank, "Gets and Sets the faction rank for this creature")
		.add_property("owner_id", &CreatureWrapper::GetOwnerId, &CreatureWrapper::SetOwnerId, "Gets and Sets the owner id for the creature")
		.add_property("scale", &CreatureWrapper::GetScale, &CreatureWrapper::SetScale, "Gets and Sets the scale of the creature 1-10")
		.add_property("battle_fatigue", &CreatureWrapper::GetBattleFatigue, &CreatureWrapper::SetBattleFatigue, "Gets and Sets the battle fatigue of the creature")
		.add_property("state_bitmask", &CreatureWrapper::GetStateBitmask, &CreatureWrapper::SetStateBitmask, "Gets and Sets the state bitmask see enum STATES")
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

		;
}

#endif  //SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_
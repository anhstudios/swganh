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
		.add_property("posture", &CreatureWrapper::GetPosture, &CreatureWrapper::SetPosture, "Gets and Sets the posture of the creature, see POSTURE enum")
		;
}

#endif  //SWGANH_OBJECT_CREATURE_CREATURE_BINDING_H_
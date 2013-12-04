// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif


#include "swganh/scripting/python_shared_ptr.h"
#include "gamesystems_service_interface.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/object/creature/creature.h"

#include <boost/python.hpp>

using namespace swganh::object;
using namespace swganh::gamesystems;
using namespace boost::python;
using namespace std;

void exportGameSystemsService()
{
	//typedef bool	(HamInterface::*ApplyHamCostsBinding)(std::shared_ptr<swganh::object::Creature> creature, uint32_t healthCost, uint32_t actionCost, uint32_t mindCost);
	//typedef void	(GameSystemsServiceInterface::*DropSkillBinding)(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_name);
	//typedef void	(GameSystemsServiceInterface::*GrantSkillBinding)(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_name);

    class_<GameSystemsServiceInterface, shared_ptr<GameSystemsServiceInterface>, boost::noncopyable>("GameSytemsService", "The Gamesystem service is a utility class for helping with game services like skills.", no_init)
	.def("dropSkill", &GameSystemsServiceInterface::DropSkill,"Drops a skill together with associated commands and Mods and schematics")
	.def("grantSkill", &GameSystemsServiceInterface::GrantSkill,"Grants a skill together with relevant Mods, Commands and schematics")
	;
    //.def("getSlotNameById", &EquipmentServiceInterface::GetSlotNameById,"Returns the slot name by the slot_id. This can be used to 'undo' GetSlotIdByName")
    //.def("clearSlot", ClearFunc,"Utility method that gets the slot id for a slot name and then clears that slot from an object.")
    //.def("getEquippedObject", GetFunc,"Utility method that gets the slot id for a slot name and then returns the object in that slot for a given object.")
    //.def("getPlayerObject", make_function(
    //         std::bind(&EquipmentServiceInterface::GetEquippedObject<Player>, std::placeholders::_1, std::placeholders::_2, "ghost"),
    //         return_value_policy<return_by_value>(),
    //         boost::mpl::vector<shared_ptr<Player>, EquipmentServiceInterface*, shared_ptr<Object>>()),
    //     "returns the :class:`.Player` equipped by this creature object")
    //;
}

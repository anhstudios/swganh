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

#ifndef SWGANH_OBJECT_PLAYER_PLAYER_BINDING_H_
#define SWGANH_OBJECT_PLAYER_PLAYER_BINDING_H_

#include "swganh/object/player/player.h"
#include "swganh/object/creature/creature_binding.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object::player;


struct PlayerWrapper : Player,  wrapper<Player>
{
};

void exportPlayer()
{
	enum_<Gender>("GENDER")
		.value("FEMALE", FEMALE)
		.value("MALE", MALE)
		;
	enum_<StatusFlags>("STATUS_FLAGS")
		.value("CLEAR", CLEAR)
		.value("LFG", LFG)
		.value("HELPER", HELPER)
		.value("ROLEPLAYER", ROLEPLAYER)
		.value("AFK", AFK)
		.value("LD", LD)
		.value("FACTION_RANK", FACTION_RANK)
		.value("ANONYMOUS", ANONYMOUS)
		;
	enum_<AdminTag>("ADMIN_TAG")
		.value("CSR", CSR)
		.value("DEVELOPER", DEVELOPER)
		;
	class_<XpData>("XpData")
		//.def_readwrite("type", &XpData.type)
		//.def_readwrite("value", &XpData.value)
		//.def("__eq__", &XpData.operator==)
		;
	class_<QuestJournalData>("QuestJournalData")
		.def_readwrite("quest_crc", &QuestJournalData::quest_crc)
		.def_readwrite("owner_id", &QuestJournalData::owner_id)
		.def_readwrite("active_step_bitmask", &QuestJournalData::active_step_bitmask)
		.def_readwrite("completed_step_bitmask", &QuestJournalData::completed_step_bitmask)
		.def_readwrite("completed_flag", &QuestJournalData::completed_flag)
		.def_readwrite("counter", &QuestJournalData::counter)
		;
	class_<DraftSchematicData>("DraftSchematicData", init<uint32_t, uint32_t>())
		.def_readwrite("schematic_id", &DraftSchematicData::schematic_id)
		.def_readwrite("schematic_crc", &DraftSchematicData::schematic_crc)
		;
	class_<WaypointData>("WaypointData", init<uint64_t, uint32_t, glm::vec3, uint64_t, std::string, std::wstring, uint8_t, uint8_t>())
		.def_readwrite("object_id", &WaypointData::object_id_)
		.def_readwrite("cell_id", &WaypointData::cell_id_)
		.def_readwrite("coodinates", &WaypointData::coordinates_)
		.def_readwrite("location_network_id", &WaypointData::location_network_id_)
		.def_readwrite("planet_name", &WaypointData::planet_name_)
		.def_readwrite("name", &WaypointData::name_)
		.def_readwrite("color", &WaypointData::color_)
		.def_readwrite("activated_flag", &WaypointData::activated_flag_)
		;

	class_<PlayerWrapper, bases<Creature>, boost::noncopyable>("Player")
		.def("add_status_flag", &PlayerWrapper::AddStatusFlag, "adds a status flag to the existing bitmask")
		.def("remove_status_flag", &PlayerWrapper::RemoveStatusFlag, "remove status flag from existing bitmask")
		.def("clear_status_flags", &PlayerWrapper::ClearStatusFlags, "clears all status flags")
		.def("add_profile_flag", &PlayerWrapper::AddProfileFlag, "adds a profile flag to the existing bitmask")
		.def("remove_profile_flag", &PlayerWrapper::RemoveProfileFlag, "remove profile flag from existing bitmask")
		.def("clear_profile_flags", &PlayerWrapper::ClearProfileFlags, "clears all profile flags")
		.add_property("profession_tag", &PlayerWrapper::GetProfessionTag, &PlayerWrapper::SetProfessionTag, "Gets and Sets the profession tag")
		.add_property("born_date", &PlayerWrapper::GetBornDate, &PlayerWrapper::SetBornDate, "Gets and Sets the born date")
		.add_property("play_time", &PlayerWrapper::GetTotalPlayTime, &PlayerWrapper::SetTotalPlayTime, "Gets and Sets Total Play Time")
		.add_property("admin_flag", &PlayerWrapper::GetAdminTag, &PlayerWrapper::SetAdminTag, "Gets and Sets the admin tag of the player")
		.def("add_xp", &PlayerWrapper::AddExperience, "adds experience via XpData")
		.def("deduct_xp", &PlayerWrapper::DeductXp, "deducts experience via XpData")
		.def("clear_xp", &PlayerWrapper::ClearXpType, "clears all experience of the type given")
		.def("clear_all_xp", &PlayerWrapper::ClearAllXp, "clears all experience of all types")
		.def("add_waypoint", &PlayerWrapper::AddWaypoint, "adds a waypoint to the player")
		.def("remove_waypoint", &PlayerWrapper::RemoveWaypoint, "removes a waypoint from the player")
		.def("modify_waypoint", &PlayerWrapper::ModifyWaypoint, "modifys an existing waypoint")
		.def("clear_all_waypoints", &PlayerWrapper::ClearAllWaypoints, "clears all waypoints")
		;
}

#endif //SWGANH_OBJECT_PLAYER_PLAYER_BINDING_H_
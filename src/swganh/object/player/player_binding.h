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
	enum_<Gender>("GENDER", "The gender of the :class:`.Player`")
		.value("FEMALE", FEMALE)
		.value("MALE", MALE)
		;
	enum_<StatusFlags>("STATUS_FLAGS", "Status of the :class:`.Player` they can be in multiple of these")
		.value("CLEAR", CLEAR)
		.value("LFG", LFG)
		.value("HELPER", HELPER)
		.value("ROLEPLAYER", ROLEPLAYER)
		.value("AFK", AFK)
		.value("LD", LD)
		.value("FACTION_RANK", FACTION_RANK)
		//.value("ANONYMOUS", ANONYMOUS)
		;
    
    //TODO: Discover Profile Flags
    enum_<ProfileFlags>("PROFILE_FLAGS", "Flags that describe a :class:`.Player` profile.")
        ;
	
    enum_<AdminTag>("ADMIN_TAG", "Admin tag that the :class:`.Player` has.")
		.value("CSR", CSR)
		.value("DEVELOPER", DEVELOPER)
		;

	class_<XpData>("XpData")
		//.def_readwrite("type", &XpData.type)
		//.def_readwrite("value", &XpData.value)
		//.def("__eq__", &XpData.operator==)
		;
	class_<QuestJournalData>("QuestJournalData", "Describes the Quest Journal of the :class:`.Player`")
		.def_readwrite("quest_crc", &QuestJournalData::quest_crc)
		.def_readwrite("owner_id", &QuestJournalData::owner_id)
		.def_readwrite("active_step_bitmask", &QuestJournalData::active_step_bitmask)
		.def_readwrite("completed_step_bitmask", &QuestJournalData::completed_step_bitmask)
		.def_readwrite("completed_flag", &QuestJournalData::completed_flag)
		.def_readwrite("counter", &QuestJournalData::counter)
		;
	class_<DraftSchematicData>("DraftSchematicData", "Describes a single Draft Schematic", init<uint32_t, uint32_t>())
		.def_readwrite("schematic_id", &DraftSchematicData::schematic_id)
		.def_readwrite("schematic_crc", &DraftSchematicData::schematic_crc)
		;
	class_<WaypointData>("WaypointData", "Describes a single Waypoint", init<uint64_t, uint32_t, glm::vec3, uint64_t, std::string, std::wstring, uint8_t, uint8_t>())
		.def_readwrite("object_id", &WaypointData::object_id_)
		.def_readwrite("cell_id", &WaypointData::cell_id_)
		.def_readwrite("coodinates", &WaypointData::coordinates_)
		.def_readwrite("location_network_id", &WaypointData::location_network_id_)
		.def_readwrite("planet_name", &WaypointData::planet_name_)
		.def_readwrite("name", &WaypointData::name_)
		.def_readwrite("color", &WaypointData::color_)
		.def_readwrite("activated_flag", &WaypointData::activated_flag_)
		;

	class_<PlayerWrapper, bases<Creature>, boost::noncopyable>("Player", "The :class:`.Player` is a child of :class:`.Object` and describes actions specific to a Player Character.")
		.def("add_status_flag", &PlayerWrapper::AddStatusFlag, "adds a status flag to the existing :class:`.STATUS_FLAGS`")
		.def("remove_status_flag", &PlayerWrapper::RemoveStatusFlag, "remove status flag from existing :class:`.STATUS_FLAGS`")
		.def("clear_status_flags", &PlayerWrapper::ClearStatusFlags, "clears all status :class:`.STATUS_FLAGS`")
		.def("add_profile_flag", &PlayerWrapper::AddProfileFlag, "adds a profile flag to the existing :class:`.PROFILE_FLAGS`")
		.def("remove_profile_flag", &PlayerWrapper::RemoveProfileFlag, "remove profile flag from existing :class:`.PROFILE_FLAGS`")
		.def("clear_profile_flags", &PlayerWrapper::ClearProfileFlags, "clears all :class:`.PROFILE_FLAGS`")
		.add_property("profession_tag", &PlayerWrapper::GetProfessionTag, &PlayerWrapper::SetProfessionTag, "Gets and Sets the profession tag")
		.add_property("born_date", &PlayerWrapper::GetBornDate, &PlayerWrapper::SetBornDate, "Gets and Sets the born date")
		.add_property("play_time", &PlayerWrapper::GetTotalPlayTime, &PlayerWrapper::SetTotalPlayTime, "Gets and Sets Total Play Time")
		.add_property("admin_flag", &PlayerWrapper::GetAdminTag, &PlayerWrapper::SetAdminTag, "Gets and Sets the admin tag of the player")
		.def("add_xp", &PlayerWrapper::AddExperience, "adds experience via :class:`.XpData`")
		.def("deduct_xp", &PlayerWrapper::DeductXp, "deducts experience via :class:`.XpData`")
		.def("clear_xp", &PlayerWrapper::ClearXpType, "clears all experience of the type given")
		.def("clear_all_xp", &PlayerWrapper::ClearAllXp, "clears all experience of all types")
		.def("add_waypoint", &PlayerWrapper::AddWaypoint, "adds a :class:`.Waypoint` to the player")
		.def("remove_waypoint", &PlayerWrapper::RemoveWaypoint, "removes a :class:`.Waypoint` from the player")
		.def("modify_waypoint", &PlayerWrapper::ModifyWaypoint, "modifys an existing :class:`.Waypoint`")
		.def("clear_all_waypoints", &PlayerWrapper::ClearAllWaypoints, "clears all :class:`.Waypoint`")
		.add_property("force_power", &PlayerWrapper::GetCurrentForcePower, &PlayerWrapper::SetCurrentForcePower, "Gets and Sets the current force power")
		.def("add_force_power", &PlayerWrapper::IncrementForcePower, "increments the current force power by x amount")
		.add_property("max_force_power", &PlayerWrapper::GetMaxForcePower, &PlayerWrapper::SetMaxForcePower, "Gets and set the max force power")
		.def("get_current_fs_quests", &PlayerWrapper::GetCurrentForceSensitiveQuests, "Gets the players current force sensitive quests in crc form")
		.def("add_fs_quest", &PlayerWrapper::AddCurrentForceSensitiveQuest, "Adds a fs quest to the players current force sensitive quests")
		.def("clear_fs_quests", &PlayerWrapper::ClearCurrentForceSensitiveQuests, "Clear current force sensitive quests")
		.def("get_completed_fs_quests", &PlayerWrapper::GetCompletedForceSensitiveQuests, "Gets the players completed force sensitive quests in crc form")
		.def("complete_fs_quest", &PlayerWrapper::AddCompletedForceSensitiveQuest, "Adds a fs quest to the players completed force sensitive quests")
		.def("clear_completed_fs_quests", &PlayerWrapper::ClearCompletedForceSensitiveQuests, "Clear completed force sensitive quests")
		.def("add_quest", &PlayerWrapper::AddQuest, "Adds a quest to the players :class:`.QuestJournal`")
		.def("remove_quest", &PlayerWrapper::RemoveQuest, "Removes quest from the players :class:`.QuestJournal`")
		.def("update_quest", &PlayerWrapper::UpdateQuest, "Updates an existing quest in the players :class:`.QuestJournal`")
		.def("clear_quests", &PlayerWrapper::ClearAllQuests, "Clears all quests from a players :class:`.QuestJournal`")
		/*.def("add_ability", &PlayerWrapper::AddAbility, "Adds an ability to the player")
		.def("remove_ability", &PlayerWrapper::RemoveAbility, "Removes an ability from the player")
		.def("clear_abilities", &PlayerWrapper::ClearAllAbilities, "Clears all abilities from the player")*/
		.add_property("experimentation_flag", &PlayerWrapper::GetExperimentationFlag, &PlayerWrapper::SetExperimentationFlag, "Gets and Sets the player experimentation flag")
		.add_property("crafting_stage", &PlayerWrapper::GetCraftingStage, &PlayerWrapper::SetCraftingStage, "Gets and Sets the players current crafting stage")
		.add_property("nearest_crafting_station", &PlayerWrapper::GetNearestCraftingStation, &PlayerWrapper::SetNearestCraftingStation, "Gets and Sets the nearest crafting station for the player")
		.def("add_draft_schematic", &PlayerWrapper::AddDraftSchematic, "Adds a :class:`.DraftSchematicData` to the player")
		.def("remove_draft_schematic", &PlayerWrapper::RemoveDraftSchematic, "Removes a :class:`.DraftSchematicData` from the player")
		.def("clear_draft_schematics", &PlayerWrapper::ClearDraftSchematics, "Clears all :class:`.DraftSchematicData`  from player")
		.add_property("experimentation_points", &PlayerWrapper::GetExperimentationPoints, &PlayerWrapper::ResetExperimentationPoints, "Gets and Resets the experimentation points of the player")
		.def("add_experimentation_points", &PlayerWrapper::AddExperimentationPoints, "Adds experimentations points to the player")
		.def("remove_experimentation_points", &PlayerWrapper::RemoveExperimentationPoints, "Removes experimentation points from the player")
		.def("increment_accomplishments", &PlayerWrapper::IncrementAccomplishmentCounter, "increments the accomplishment counter of the player")
		.add_property("accomplishment_counter", &PlayerWrapper::GetAccomplishmentCounter, &PlayerWrapper::ResetAccomplishmentCounter, "Gets and Resets the accomplishment counter of the player")
		.def("add_friend", &PlayerWrapper::AddFriend, "Adds a friend to the players friends list")
		.def("remove_friend", &PlayerWrapper::RemoveFriend, "Removes a friend from the players friends list")
		.def("clear_friends", &PlayerWrapper::ClearFriends, "Clears all friends from the players friends list")
		.def("ignore_player", &PlayerWrapper::IgnorePlayer, "Adds a player to the ignore list")
		.def("stop_ignore_player", &PlayerWrapper::StopIgnoringPlayer, "Stops ignoring a player, removes them from the ignore list")
		.def("clear_ignore_players", &PlayerWrapper::ClearIgnored, "Clears the ignore player list")
		.add_property("language", &PlayerWrapper::GetLanguage, &PlayerWrapper::SetLanguage, "Gets and Sets the players spoken language")
		.add_property("stomach", &PlayerWrapper::GetCurrentStomach, &PlayerWrapper::ResetCurrentStomach, "Gets and resets the players stomach level")
		.def("increase_stomach", &PlayerWrapper::IncreaseCurrentStomach, "adds to the players current stomach level")
		.def("decrease_stomach", &PlayerWrapper::DecreaseCurrentStomach, "decreases from the players current stomach level")
		.add_property("max_stomach", &PlayerWrapper::GetMaxStomach, &PlayerWrapper::ResetMaxStomach, "Gets and resets the players MAX stomach level")
		.add_property("drink", &PlayerWrapper::GetCurrentDrink, &PlayerWrapper::ResetCurrentDrink, "Gets and resets the players drink level")
		.def("increase_drink", &PlayerWrapper::IncreaseCurrentDrink, "adds to the players current drink level")
		.def("decrease_drink", &PlayerWrapper::DecreaseCurrentDrink, "decreases from the players current drink level")
		.add_property("max_drink", &PlayerWrapper::GetMaxDrink, &PlayerWrapper::ResetMaxDrink, "Gets and resets the players MAX drink level")
		.add_property("jedi_state", &PlayerWrapper::GetJediState, &PlayerWrapper::SetJediState, "Gets and set the jedi state")
		.add_property("gender", &PlayerWrapper::GetGender, &PlayerWrapper::SetGender, "Gets and Sets the players :class:`.GENDER` ")
		;
}

#endif //SWGANH_OBJECT_PLAYER_PLAYER_BINDING_H_
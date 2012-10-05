// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/object/player/player.h"
#include "swganh/object/creature/creature_binding.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;


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
    
    //@TODO: Discover Profile Flags
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
	class_<PlayerWaypointSerializer>("PlayerWaypointSerializer", "Describes a single Waypoint", init<std::shared_ptr<Waypoint>>())
		.def_readwrite("waypoint", &PlayerWaypointSerializer::waypoint)
		;

	class_<PlayerWrapper, bases<Creature>, std::shared_ptr<Player>, boost::noncopyable>("Player", "The :class:`.Player` is a child of :class:`.Object` and describes actions specific to a Player Character.")
		.def("addStatusFlag", &PlayerWrapper::AddStatusFlag, "adds a status flag to the existing :class:`.STATUS_FLAGS`")
		.def("removeStatusFlag", &PlayerWrapper::RemoveStatusFlag, "remove status flag from existing :class:`.STATUS_FLAGS`")
		.def("clearStatusFlags", &PlayerWrapper::ClearStatusFlags, "clears all status :class:`.STATUS_FLAGS`")
		.def("addProfileFlag", &PlayerWrapper::AddProfileFlag, "adds a profile flag to the existing :class:`.PROFILE_FLAGS`")
		.def("removeProfileFlag", &PlayerWrapper::RemoveProfileFlag, "remove profile flag from existing :class:`.PROFILE_FLAGS`")
		.def("clearProfileFlags", &PlayerWrapper::ClearProfileFlags, "clears all :class:`.PROFILE_FLAGS`")
		.add_property("profession_tag", &PlayerWrapper::GetProfessionTag, &PlayerWrapper::SetProfessionTag, "Gets and Sets the profession tag")
		.add_property("born_date", &PlayerWrapper::GetBornDate, &PlayerWrapper::SetBornDate, "Gets and Sets the born date")
		.add_property("play_time", &PlayerWrapper::GetTotalPlayTime, &PlayerWrapper::SetTotalPlayTime, "Gets and Sets Total Play Time")
		.add_property("admin_flag", &PlayerWrapper::GetAdminTag, &PlayerWrapper::SetAdminTag, "Gets and Sets the admin tag of the player")
		.def("addXp", &PlayerWrapper::AddExperience, "adds experience via :class:`.XpData`")
		.def("deductXp", &PlayerWrapper::DeductXp, "deducts experience via :class:`.XpData`")
		.def("clearXp", &PlayerWrapper::ClearXpType, "clears all experience of the type given")
		.def("clearAllXp", &PlayerWrapper::ClearAllXp, "clears all experience of all types")
		.def("addWaypoint", &PlayerWrapper::AddWaypoint, "adds a :class:`.Waypoint` to the player")
		.def("removeWaypoint", &PlayerWrapper::RemoveWaypoint, "removes a :class:`.Waypoint` from the player")
		.def("modifyWaypoint", &PlayerWrapper::ModifyWaypoint, "modifys an existing :class:`.Waypoint`")
		.def("clearAllWaypoints", &PlayerWrapper::ClearAllWaypoints, "clears all :class:`.Waypoint`")
		.add_property("forcePower", &PlayerWrapper::GetCurrentForcePower, &PlayerWrapper::SetCurrentForcePower, "Gets and Sets the current force power")
		.def("addForcePower", &PlayerWrapper::IncrementForcePower, "increments the current force power by x amount")
		.add_property("maxForcePower", &PlayerWrapper::GetMaxForcePower, &PlayerWrapper::SetMaxForcePower, "Gets and set the max force power")
		.def("getCurrentFsQuests", &PlayerWrapper::GetCurrentForceSensitiveQuests, "Gets the players current force sensitive quests in crc form")
		.def("addFsQuest", &PlayerWrapper::AddCurrentForceSensitiveQuest, "Adds a fs quest to the players current force sensitive quests")
		.def("clearFsQuests", &PlayerWrapper::ClearCurrentForceSensitiveQuests, "Clear current force sensitive quests")
		.def("getCompletedFsQuests", &PlayerWrapper::GetCompletedForceSensitiveQuests, "Gets the players completed force sensitive quests in crc form")
		.def("completeFsQuest", &PlayerWrapper::AddCompletedForceSensitiveQuest, "Adds a fs quest to the players completed force sensitive quests")
		.def("clearCompletedFsQuests", &PlayerWrapper::ClearCompletedForceSensitiveQuests, "Clear completed force sensitive quests")
		.def("addQuest", &PlayerWrapper::AddQuest, "Adds a quest to the players :class:`.QuestJournal`")
		.def("removeQuest", &PlayerWrapper::RemoveQuest, "Removes quest from the players :class:`.QuestJournal`")
		.def("updateQuest", &PlayerWrapper::UpdateQuest, "Updates an existing quest in the players :class:`.QuestJournal`")
		.def("clearQuests", &PlayerWrapper::ClearAllQuests, "Clears all quests from a players :class:`.QuestJournal`")
		/*.def("add_ability", &PlayerWrapper::AddAbility, "Adds an ability to the player")
		.def("remove_ability", &PlayerWrapper::RemoveAbility, "Removes an ability from the player")
		.def("clear_abilities", &PlayerWrapper::ClearAllAbilities, "Clears all abilities from the player")*/
		.add_property("experimentation_flag", &PlayerWrapper::GetExperimentationFlag, &PlayerWrapper::SetExperimentationFlag, "Gets and Sets the player experimentation flag")
		.add_property("crafting_stage", &PlayerWrapper::GetCraftingStage, &PlayerWrapper::SetCraftingStage, "Gets and Sets the players current crafting stage")
		.add_property("nearest_crafting_station", &PlayerWrapper::GetNearestCraftingStation, &PlayerWrapper::SetNearestCraftingStation, "Gets and Sets the nearest crafting station for the player")
		.def("addDraftSchematic", &PlayerWrapper::AddDraftSchematic, "Adds a :class:`.DraftSchematicData` to the player")
		.def("removeDraftSchematic", &PlayerWrapper::RemoveDraftSchematic, "Removes a :class:`.DraftSchematicData` from the player")
		.def("clearDraftSchematics", &PlayerWrapper::ClearDraftSchematics, "Clears all :class:`.DraftSchematicData`  from player")
		.add_property("experimentation_points", &PlayerWrapper::GetExperimentationPoints, &PlayerWrapper::ResetExperimentationPoints, "Gets and Resets the experimentation points of the player")
		.def("addExperimentationPoints", &PlayerWrapper::AddExperimentationPoints, "Adds experimentations points to the player")
		.def("removeExperimentationPoints", &PlayerWrapper::RemoveExperimentationPoints, "Removes experimentation points from the player")
		.def("incrementAccomplishments", &PlayerWrapper::IncrementAccomplishmentCounter, "increments the accomplishment counter of the player")
		.add_property("accomplishment_counter", &PlayerWrapper::GetAccomplishmentCounter, &PlayerWrapper::ResetAccomplishmentCounter, "Gets and Resets the accomplishment counter of the player")
		.def("addFriend", &PlayerWrapper::AddFriend, "Adds a friend to the players friends list")
		.def("removeFriend", &PlayerWrapper::RemoveFriend, "Removes a friend from the players friends list")
		.def("clearFriends", &PlayerWrapper::ClearFriends, "Clears all friends from the players friends list")
        .def("isFriend", &PlayerWrapper::IsFriend, "Checks to see if the name is currently a friend of the player")
        .def("isIgnored", &PlayerWrapper::IsIgnored, "Checks to see if the name is currently being ignored by the player")
		.def("ignorePlayer", &PlayerWrapper::IgnorePlayer, "Adds a player to the ignore list")
		.def("removeIgnore", &PlayerWrapper::StopIgnoringPlayer, "Stops ignoring a player, removes them from the ignore list")
		.def("clearIgnorePlayers", &PlayerWrapper::ClearIgnored, "Clears the ignore player list")
		.add_property("language", &PlayerWrapper::GetLanguage, &PlayerWrapper::SetLanguage, "Gets and Sets the players spoken language")
		.add_property("stomach", &PlayerWrapper::GetCurrentStomach, &PlayerWrapper::ResetCurrentStomach, "Gets and resets the players stomach level")
		.def("increaseStomach", &PlayerWrapper::IncreaseCurrentStomach, "adds to the players current stomach level")
		.def("decreaseStomach", &PlayerWrapper::DecreaseCurrentStomach, "decreases from the players current stomach level")
		.add_property("max_stomach", &PlayerWrapper::GetMaxStomach, &PlayerWrapper::ResetMaxStomach, "Gets and resets the players MAX stomach level")
		.add_property("drink", &PlayerWrapper::GetCurrentDrink, &PlayerWrapper::ResetCurrentDrink, "Gets and resets the players drink level")
		.def("increaseDrink", &PlayerWrapper::IncreaseCurrentDrink, "adds to the players current drink level")
		.def("decreaseDrink", &PlayerWrapper::DecreaseCurrentDrink, "decreases from the players current drink level")
		.add_property("max_drink", &PlayerWrapper::GetMaxDrink, &PlayerWrapper::ResetMaxDrink, "Gets and resets the players MAX drink level")
		.add_property("jedi_state", &PlayerWrapper::GetJediState, &PlayerWrapper::SetJediState, "Gets and set the jedi state")
		.add_property("gender", &PlayerWrapper::GetGender, &PlayerWrapper::SetGender, "Gets and Sets the players :class:`.GENDER` ")
		;

	implicitly_convertible<std::shared_ptr<Player>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Player>, std::shared_ptr<ContainerInterface>>();
}
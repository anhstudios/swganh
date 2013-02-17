// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/object/player/player.h"
#include "swganh_core/object/creature/creature_binding.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

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

	class_<Player, bases<Intangible>, std::shared_ptr<Player>, boost::noncopyable>("Player", "The :class:`.Player` is a child of :class:`.Object` and describes actions specific to a Player Character.")
		.def("addStatusFlag", &Player::AddStatusFlag, "adds a status flag to the existing :class:`.STATUS_FLAGS`")
		.def("removeStatusFlag", &Player::RemoveStatusFlag, "remove status flag from existing :class:`.STATUS_FLAGS`")
		.def("clearStatusFlags", &Player::ClearStatusFlags, "clears all status :class:`.STATUS_FLAGS`")
		.def("addProfileFlag", &Player::AddProfileFlag, "adds a profile flag to the existing :class:`.PROFILE_FLAGS`")
		.def("removeProfileFlag", &Player::RemoveProfileFlag, "remove profile flag from existing :class:`.PROFILE_FLAGS`")
		.def("clearProfileFlags", &Player::ClearProfileFlags, "clears all :class:`.PROFILE_FLAGS`")
		.add_property("profession_tag", &Player::GetProfessionTag, &Player::SetProfessionTag, "Gets and Sets the profession tag")
		.add_property("born_date", &Player::GetBornDate, &Player::SetBornDate, "Gets and Sets the born date")
		.add_property("play_time", &Player::GetTotalPlayTime, &Player::SetTotalPlayTime, "Gets and Sets Total Play Time")
		.add_property("admin_flag", &Player::GetAdminTag, &Player::SetAdminTag, "Gets and Sets the admin tag of the player")
		.def("addXp", &Player::AddExperience, "adds experience via :class:`.XpData`")
		.def("deductXp", &Player::DeductXp, "deducts experience via :class:`.XpData`")
		.def("clearXp", &Player::ClearXpType, "clears all experience of the type given")
		.def("clearAllXp", &Player::ClearAllXp, "clears all experience of all types")
		.def("addWaypoint", &Player::AddWaypoint, "adds a :class:`.Waypoint` to the player")
		.def("removeWaypoint", &Player::RemoveWaypoint, "removes a :class:`.Waypoint` from the player")
		.def("modifyWaypoint", &Player::ModifyWaypoint, "modifys an existing :class:`.Waypoint`")
		.def("clearAllWaypoints", &Player::ClearAllWaypoints, "clears all :class:`.Waypoint`")
		.add_property("forcePower", &Player::GetCurrentForcePower, &Player::SetCurrentForcePower, "Gets and Sets the current force power")
		.def("addForcePower", &Player::IncrementForcePower, "increments the current force power by x amount")
		.add_property("maxForcePower", &Player::GetMaxForcePower, &Player::SetMaxForcePower, "Gets and set the max force power")
		.def("getCurrentFsQuests", &Player::GetCurrentForceSensitiveQuests, "Gets the players current force sensitive quests in crc form")
		.def("addFsQuest", &Player::AddCurrentForceSensitiveQuest, "Adds a fs quest to the players current force sensitive quests")
		.def("clearFsQuests", &Player::ClearCurrentForceSensitiveQuests, "Clear current force sensitive quests")
		.def("getCompletedFsQuests", &Player::GetCompletedForceSensitiveQuests, "Gets the players completed force sensitive quests in crc form")
		.def("completeFsQuest", &Player::AddCompletedForceSensitiveQuest, "Adds a fs quest to the players completed force sensitive quests")
		.def("clearCompletedFsQuests", &Player::ClearCompletedForceSensitiveQuests, "Clear completed force sensitive quests")
		.def("addQuest", &Player::AddQuest, "Adds a quest to the players :class:`.QuestJournal`")
		.def("removeQuest", &Player::RemoveQuest, "Removes quest from the players :class:`.QuestJournal`")
		.def("updateQuest", &Player::UpdateQuest, "Updates an existing quest in the players :class:`.QuestJournal`")
		.def("clearQuests", &Player::ClearAllQuests, "Clears all quests from a players :class:`.QuestJournal`")
		/*.def("add_ability", &Player::AddAbility, "Adds an ability to the player")
		.def("remove_ability", &Player::RemoveAbility, "Removes an ability from the player")
		.def("clear_abilities", &Player::ClearAllAbilities, "Clears all abilities from the player")*/
		.add_property("experimentation_flag", &Player::GetExperimentationFlag, &Player::SetExperimentationFlag, "Gets and Sets the player experimentation flag")
		.add_property("crafting_stage", &Player::GetCraftingStage, &Player::SetCraftingStage, "Gets and Sets the players current crafting stage")
		.add_property("nearest_crafting_station", &Player::GetNearestCraftingStation, &Player::SetNearestCraftingStation, "Gets and Sets the nearest crafting station for the player")
		.def("addDraftSchematic", &Player::AddDraftSchematic, "Adds a :class:`.DraftSchematicData` to the player")
		.def("removeDraftSchematic", &Player::RemoveDraftSchematic, "Removes a :class:`.DraftSchematicData` from the player")
		.def("clearDraftSchematics", &Player::ClearDraftSchematics, "Clears all :class:`.DraftSchematicData`  from player")
		.add_property("experimentation_points", &Player::GetExperimentationPoints, &Player::ResetExperimentationPoints, "Gets and Resets the experimentation points of the player")
		.def("addExperimentationPoints", &Player::AddExperimentationPoints, "Adds experimentations points to the player")
		.def("removeExperimentationPoints", &Player::RemoveExperimentationPoints, "Removes experimentation points from the player")
		.def("incrementAccomplishments", &Player::IncrementAccomplishmentCounter, "increments the accomplishment counter of the player")
		.add_property("accomplishment_counter", &Player::GetAccomplishmentCounter, &Player::ResetAccomplishmentCounter, "Gets and Resets the accomplishment counter of the player")
		.def("addFriend", &Player::AddFriend, "Adds a friend to the players friends list")
		.def("removeFriend", &Player::RemoveFriend, "Removes a friend from the players friends list")
		.def("clearFriends", &Player::ClearFriends, "Clears all friends from the players friends list")
        .def("isFriend", &Player::IsFriend, "Checks to see if the name is currently a friend of the player")
        .def("isIgnored", &Player::IsIgnored, "Checks to see if the name is currently being ignored by the player")
		.def("ignorePlayer", &Player::IgnorePlayer, "Adds a player to the ignore list")
		.def("removeIgnore", &Player::StopIgnoringPlayer, "Stops ignoring a player, removes them from the ignore list")
		.def("clearIgnorePlayers", &Player::ClearIgnored, "Clears the ignore player list")
		.add_property("language", &Player::GetLanguage, &Player::SetLanguage, "Gets and Sets the players spoken language")
		.add_property("stomach", &Player::GetCurrentStomach, &Player::ResetCurrentStomach, "Gets and resets the players stomach level")
		.def("increaseStomach", &Player::IncreaseCurrentStomach, "adds to the players current stomach level")
		.def("decreaseStomach", &Player::DecreaseCurrentStomach, "decreases from the players current stomach level")
		.add_property("max_stomach", &Player::GetMaxStomach, &Player::ResetMaxStomach, "Gets and resets the players MAX stomach level")
		.add_property("drink", &Player::GetCurrentDrink, &Player::ResetCurrentDrink, "Gets and resets the players drink level")
		.def("increaseDrink", &Player::IncreaseCurrentDrink, "adds to the players current drink level")
		.def("decreaseDrink", &Player::DecreaseCurrentDrink, "decreases from the players current drink level")
		.add_property("max_drink", &Player::GetMaxDrink, &Player::ResetMaxDrink, "Gets and resets the players MAX drink level")
		.add_property("jedi_state", &Player::GetJediState, &Player::SetJediState, "Gets and set the jedi state")
		.add_property("gender", &Player::GetGender, &Player::SetGender, "Gets and Sets the players :class:`.GENDER` ")
		;

	implicitly_convertible<std::shared_ptr<Player>, std::shared_ptr<Object>>();
	implicitly_convertible<std::shared_ptr<Player>, std::shared_ptr<ContainerInterface>>();
}
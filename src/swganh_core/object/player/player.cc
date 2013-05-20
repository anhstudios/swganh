// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "player.h"

#include "swganh/logger.h"

#include "swganh/crc.h"

#include "player_events.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/object/creature/creature.h"

#include "swganh_core/object/waypoint/waypoint.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

Player::Player()
: Intangible()
, status_flags_(array<uint32_t, 4>())
, profile_flags_(array<uint32_t, 4>())
, profession_tag_("")
, born_date_(0)
, total_playtime_(0)
, admin_tag_(0)
, region_(0)
, current_force_power_(0)
, max_force_power_(0)
, current_force_sensitive_quests_(0)
, completed_force_sensitive_quests_(0)
, experimentation_flag_(0)
, crafting_stage_(0)
, nearest_crafting_station_(0)
, experimentation_points_(0)
, accomplishment_counter_(0)
, language_(0)
, current_stomach_(0)
, max_stomach_(0)
, current_drink_(0)
, max_drink_(0)
, jedi_state_(0)
, gender_(MALE)
{}

std::array<uint32_t, 4> Player::GetStatusFlags() { return GetStatusFlags(AcquireLock()); }
std::array<uint32_t, 4> Player::GetStatusFlags(boost::unique_lock<boost::mutex>& lock) 
{
    return status_flags_;
}

void Player::AddStatusFlag(StatusFlags flag, StatusIndex index) { AddStatusFlag(flag, AcquireLock(), index); }
void Player::AddStatusFlag(StatusFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
    status_flags_[index] = status_flags_[index] | flag;
	DISPATCH(Player, StatusBitmask);
}

void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index) { RemoveStatusFlag(flag, AcquireLock(), index); }
void Player::RemoveStatusFlag(StatusFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
	status_flags_[index] = status_flags_[index] & ~flag;
    DISPATCH(Player, StatusBitmask);
}

void Player::ClearStatusFlags() { ClearStatusFlags(AcquireLock()); }
void Player::ClearStatusFlags(boost::unique_lock<boost::mutex>& lock)
{
    for_each(begin(status_flags_), end(status_flags_), [] (uint32_t& value) {
		value = 0;
	});
    DISPATCH(Player, StatusBitmask);
}

std::array<uint32_t, 4> Player::GetProfileFlags() { return GetProfileFlags(AcquireLock()); }
std::array<uint32_t, 4> Player::GetProfileFlags(boost::unique_lock<boost::mutex>& lock) 
{
    return profile_flags_;
}

void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index) { AddProfileFlag(flag, AcquireLock(), index); }
void Player::AddProfileFlag(ProfileFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
	profile_flags_[index] = profile_flags_[index] | flag;
	DISPATCH(Player, ProfileFlag);
}

void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index) { RemoveProfileFlag(flag, AcquireLock(), index); }
void Player::RemoveProfileFlag(ProfileFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
	profile_flags_[index] = profile_flags_[index] & ~flag;
	DISPATCH(Player, ProfileFlag);
}

void Player::ClearProfileFlags() { ClearProfileFlags(AcquireLock()); }
void Player::ClearProfileFlags(boost::unique_lock<boost::mutex>& lock)
{
    for_each(begin(profile_flags_), end(profile_flags_), [] (uint32_t& value)  {
		value = 0;
	});
	DISPATCH(Player, ProfileFlag);
}

std::string Player::GetProfessionTag() { return GetProfessionTag(AcquireLock()); }
std::string Player::GetProfessionTag(boost::unique_lock<boost::mutex>& lock) 
{
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag) { SetProfessionTag(profession_tag, AcquireLock()); }
void Player::SetProfessionTag(string profession_tag, boost::unique_lock<boost::mutex>& lock)
{
    profession_tag_ = profession_tag;
	DISPATCH(Player, ProfessionTag);
}

uint32_t Player::GetBornDate() { return GetBornDate(AcquireLock()); }
uint32_t Player::GetBornDate(boost::unique_lock<boost::mutex>& lock) 
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date) { SetBornDate(born_date, AcquireLock()); }
void Player::SetBornDate(uint32_t born_date, boost::unique_lock<boost::mutex>& lock)
{
    born_date_ = born_date;
	DISPATCH(Player, BornDate);
}

uint32_t Player::GetTotalPlayTime() { return GetTotalPlayTime(AcquireLock()); }
uint32_t Player::GetTotalPlayTime(boost::unique_lock<boost::mutex>& lock) 
{
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time) { SetTotalPlayTime(play_time, AcquireLock()); }
void Player::SetTotalPlayTime(uint32_t play_time, boost::unique_lock<boost::mutex>& lock)
{
    total_playtime_ = play_time;
	DISPATCH(Player, TotalPlayTime);
}

void Player::IncrementTotalPlayTime(uint32_t increment) { IncrementTotalPlayTime(increment, AcquireLock()); }
void Player::IncrementTotalPlayTime(uint32_t increment, boost::unique_lock<boost::mutex>& lock)
{
    total_playtime_ += increment;
	DISPATCH(Player, TotalPlayTime);
}

uint8_t Player::GetAdminTag() { return GetAdminTag(AcquireLock()); }
uint8_t Player::GetAdminTag(boost::unique_lock<boost::mutex>& lock) 
{
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag) { SetAdminTag(tag, AcquireLock()); }
void Player::SetAdminTag(uint8_t tag, boost::unique_lock<boost::mutex>& lock)
{
    admin_tag_ = tag;
	DISPATCH(Player, AdminTag);
}

std::map<string, XpData> Player::GetXp() { return GetXp(AcquireLock()); }
std::map<string, XpData> Player::GetXp(boost::unique_lock<boost::mutex>& lock) 
{
    return experience_.raw();
}

void Player::AddExperience(std::string type, uint32_t value) { AddExperience(type, value, AcquireLock()); }
void Player::AddExperience(std::string type, uint32_t value, boost::unique_lock<boost::mutex>& lock)
{
	if(experience_.contains(type))
	{
		experience_[type].value += value;
		experience_.update(type);
	}
	else
	{
		experience_.add(type, XpData(type, value));
	}

	DISPATCH(Player, Experience);
}

void Player::DeductXp(std::string type, uint32_t value) { DeductXp(type, value, AcquireLock()); }
void Player::DeductXp(std::string type, uint32_t value, boost::unique_lock<boost::mutex>& lock)
{
    if(experience_.contains(type))
	{
		experience_[type].value -= value;
		experience_.update(type);
	}
	else
	{
		experience_.add(type, XpData(type, 0));
	}

	DISPATCH(Player, Experience);
}

void Player::ClearXpType(string type) { ClearXpType(type, AcquireLock()); }
void Player::ClearXpType(string type, boost::unique_lock<boost::mutex>& lock)
{
    experience_.remove(experience_.find(type));
	DISPATCH(Player, Experience);
}

std::map<uint64_t, PlayerWaypointSerializer> Player::GetWaypoints()  { return GetWaypoints(AcquireLock()); }
std::map<uint64_t, PlayerWaypointSerializer> Player::GetWaypoints(boost::unique_lock<boost::mutex>& lock) 
{
    return waypoints_.raw();
}

void Player::AddWaypoint(PlayerWaypointSerializer waypoint) { AddWaypoint(waypoint.waypoint->GetObjectId(), waypoint, AcquireLock()); }
void Player::AddWaypoint(uint64_t way_object_id, PlayerWaypointSerializer waypoint,boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.add(way_object_id, waypoint);
	DISPATCH(Player, Waypoint);
}

void Player::RemoveWaypoint(uint64_t waypoint_id) { RemoveWaypoint(waypoint_id, AcquireLock()); }
void Player::RemoveWaypoint(uint64_t waypoint_id, boost::unique_lock<boost::mutex>& lock)
{
	auto find_iter = find_if(begin(waypoints_), end(waypoints_), [waypoint_id] (pair<uint64_t, PlayerWaypointSerializer> stored_waypoint)
    {
        return waypoint_id == stored_waypoint.first;
    });

    if (find_iter == end(waypoints_))
    {
        return;
    }

    waypoints_.remove(find_iter);
	DISPATCH(Player, Waypoint);
}

void Player::ModifyWaypoint(PlayerWaypointSerializer waypoint) { ModifyWaypoint(waypoint.waypoint->GetObjectId(), AcquireLock()); }
void Player::ModifyWaypoint(uint64_t way_object_id, boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.update(way_object_id);
	DISPATCH(Player, Waypoint);
}

int32_t Player::GetCurrentForcePower() { return GetCurrentForcePower(AcquireLock()); }
int32_t Player::GetCurrentForcePower(boost::unique_lock<boost::mutex>& lock) 
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(int32_t force_power) { SetCurrentForcePower(force_power, AcquireLock()); }
void Player::SetCurrentForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock)
{
    current_force_power_ = force_power;
	DISPATCH(Player, ForcePower);
}

void Player::IncrementForcePower(int32_t force_power) { IncrementForcePower(force_power, AcquireLock()); }
void Player::IncrementForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    DISPATCH(Player, ForcePower);
}

int32_t Player::GetMaxForcePower() { return GetMaxForcePower(AcquireLock()); }
int32_t Player::GetMaxForcePower(boost::unique_lock<boost::mutex>& lock) 
{
    return max_force_power_;
}

void Player::SetMaxForcePower(int32_t force_power) { SetMaxForcePower(force_power, AcquireLock()); }
void Player::SetMaxForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock)
{
    max_force_power_ = force_power;
	DISPATCH(Player, MaxForcePower);
}

uint32_t Player::GetCurrentForceSensitiveQuests() { return GetCurrentForceSensitiveQuests(AcquireLock()); }
uint32_t Player::GetCurrentForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask) { AddCurrentForceSensitiveQuest(quest_mask, AcquireLock()); }
void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ | quest_mask;
    DISPATCH(Player, ForceSensitiveQuests);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask) { RemoveCurrentForceSensitiveQuest(quest_mask, AcquireLock()); }
void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ & ~quest_mask;
	DISPATCH(Player, ForceSensitiveQuests);
}

void Player::ClearCurrentForceSensitiveQuests() { ClearCurrentForceSensitiveQuests(AcquireLock()); }
void Player::ClearCurrentForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    current_force_sensitive_quests_ = 0;
	DISPATCH(Player, ForceSensitiveQuests);
}

uint32_t Player::GetCompletedForceSensitiveQuests() { return GetCompletedForceSensitiveQuests(AcquireLock()); }
uint32_t Player::GetCompletedForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask) { AddCompletedForceSensitiveQuest(quest_mask, AcquireLock()); }
void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ | quest_mask;
    DISPATCH(Player, CompletedForceSensitiveQuests);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask) { RemoveCompletedForceSensitiveQuest(quest_mask, AcquireLock()); }
void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ & ~quest_mask;
	DISPATCH(Player, CompletedForceSensitiveQuests);
}

void Player::ClearCompletedForceSensitiveQuests() { ClearCompletedForceSensitiveQuests(AcquireLock()); }
void Player::ClearCompletedForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    completed_force_sensitive_quests_ = 0;
	DISPATCH(Player, CompletedForceSensitiveQuests);
}

std::map<uint32_t, QuestJournalData> Player::GetQuests() { return GetQuests(AcquireLock()); }
std::map<uint32_t, QuestJournalData> Player::GetQuests(boost::unique_lock<boost::mutex>& lock) 
{
    return quest_journal_.raw();
}

void Player::AddQuest(QuestJournalData quest) { AddQuest(quest, AcquireLock()); }
void Player::AddQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
    quest_journal_.add(quest.quest_crc, quest);
	DISPATCH(Player, QuestJournal);
}

void Player::RemoveQuest(QuestJournalData quest) { RemoveQuest(quest, AcquireLock()); }
void Player::RemoveQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
    auto find_iter = find_if(begin(quest_journal_), end(quest_journal_), 
	[&quest] ( pair<uint32_t, QuestJournalData> stored_quest){
        return quest.quest_crc == stored_quest.first;
    });

    if (find_iter == end(quest_journal_))
    {
        return;
    }

    quest_journal_.remove(find_iter);
    DISPATCH(Player, QuestJournal);
}

void Player::UpdateQuest(QuestJournalData quest) { UpdateQuest(quest, AcquireLock()); }
void Player::UpdateQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
	quest_journal_.update(quest.quest_crc);
    DISPATCH(Player, QuestJournal);
}

uint32_t Player::GetExperimentationFlag() { return GetExperimentationFlag(AcquireLock()); }
uint32_t Player::GetExperimentationFlag(boost::unique_lock<boost::mutex>& lock) 
{
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag) { SetExperimentationFlag(experimentation_flag, AcquireLock()); }
void Player::SetExperimentationFlag(uint32_t experimentation_flag, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_flag_ = experimentation_flag;
	DISPATCH(Player, ExperimentationFlag);
}

uint32_t Player::GetCraftingStage() { return GetCraftingStage(AcquireLock()); }
uint32_t Player::GetCraftingStage(boost::unique_lock<boost::mutex>& lock) 
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage) { SetCraftingStage(crafting_stage, AcquireLock()); }
void Player::SetCraftingStage(uint32_t crafting_stage, boost::unique_lock<boost::mutex>& lock)
{
    crafting_stage_ = crafting_stage;
    DISPATCH(Player, CraftingStage);
}

uint64_t Player::GetNearestCraftingStation() { return GetNearestCraftingStation(AcquireLock()); }
uint64_t Player::GetNearestCraftingStation(boost::unique_lock<boost::mutex>& lock) 
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id) { SetNearestCraftingStation(crafting_station_id, AcquireLock()); }
void Player::SetNearestCraftingStation(uint64_t crafting_station_id, boost::unique_lock<boost::mutex>& lock)
{
    nearest_crafting_station_ = crafting_station_id;
	DISPATCH(Player, NearestCraftingStation);
}

std::vector<DraftSchematicData> Player::GetDraftSchematics() { return GetDraftSchematics(AcquireLock()); }
std::vector<DraftSchematicData> Player::GetDraftSchematics(boost::unique_lock<boost::mutex>& lock) 
{
	return draft_schematics_.raw();
}

void Player::AddDraftSchematic(DraftSchematicData schematic) { AddDraftSchematic(schematic, AcquireLock()); }
void Player::AddDraftSchematic(DraftSchematicData schematic, boost::unique_lock<boost::mutex>& lock)
{ 
	draft_schematics_.add(schematic);
	DISPATCH(Player, DraftSchematic);
}

void Player::RemoveDraftSchematic(uint32_t schematic_id) { RemoveDraftSchematic(schematic_id, AcquireLock()); }
void Player::RemoveDraftSchematic(uint32_t id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(draft_schematics_.begin(), draft_schematics_.end(), [=] (const DraftSchematicData& data) {
		return (data.schematic_id) == id;
	});

    if(iter != end(draft_schematics_))
    {
		draft_schematics_.remove(iter);
		DISPATCH(Player, DraftSchematic);
	}
}

void Player::ClearDraftSchematics() { ClearDraftSchematics(AcquireLock()); }
void Player::ClearDraftSchematics(boost::unique_lock<boost::mutex>& lock)
{
	draft_schematics_.clear();
    DISPATCH(Player, DraftSchematic);
}

uint32_t Player::GetExperimentationPoints()  { return GetExperimentationPoints(AcquireLock()); }
uint32_t Player::GetExperimentationPoints(boost::unique_lock<boost::mutex>& lock) 
{
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points) { AddExperimentationPoints(points, AcquireLock()); }
void Player::AddExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_points_ += points;
    DISPATCH(Player, ExperimentationPoints);
}

void Player::RemoveExperimentationPoints(uint32_t points) { RemoveExperimentationPoints(points, AcquireLock()); }
void Player::RemoveExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_points_ -= points;
	DISPATCH(Player, ExperimentationPoints);
}

void Player::ResetExperimentationPoints(uint32_t points) { ResetExperimentationPoints(points, AcquireLock()); }
void Player::ResetExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_points_ = points;
	DISPATCH(Player, ExperimentationPoints);
}

uint32_t Player::GetAccomplishmentCounter() { return GetAccomplishmentCounter(AcquireLock()); }
uint32_t Player::GetAccomplishmentCounter(boost::unique_lock<boost::mutex>& lock) 
{
    return accomplishment_counter_;
}

void Player::ResetAccomplishmentCounter(uint32_t counter) { ResetAccomplishmentCounter(counter, AcquireLock()); }
void Player::ResetAccomplishmentCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock)
{
    accomplishment_counter_ = counter;
	DISPATCH(Player, AccomplishmentCounter);
}

void Player::IncrementAccomplishmentCounter() { IncrementAccomplishmentCounter(AcquireLock()); }
void Player::IncrementAccomplishmentCounter(boost::unique_lock<boost::mutex>& lock)
{
    ++accomplishment_counter_;
	DISPATCH(Player, AccomplishmentCounter);
}

std::vector<std::string> Player::GetFriends() { return GetFriends(AcquireLock()); }
std::vector<std::string> Player::GetFriends(boost::unique_lock<boost::mutex>& lock)
{
    return friends_.raw();
}

bool Player::IsFriend(std::string friend_name) { return IsFriend(friend_name, AcquireLock()); }
bool Player::IsFriend(std::string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(friends_), end(friends_), [=](const std::string& x)->bool {
        return x == friend_name;
    });
    
    return iter != end(friends_);
}

void Player::AddFriend(string friend_name) { AddFriend(friend_name, AcquireLock()); }
void Player::AddFriend(string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    friends_.add(friend_name);
	DISPATCH(Player, Friend);
}

void Player::RemoveFriend(string friend_name) { RemoveFriend(friend_name, AcquireLock()); }
void Player::RemoveFriend(string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(friends_), end(friends_), [=](const std::string& x)->bool {
        return x == friend_name;
    });
        
    if (iter != end(friends_))
    {
		friends_.remove(iter);
		DISPATCH(Player, RemoveFriend);
	}
}

void Player::ClearFriends() { ClearFriends(AcquireLock()); }
void Player::ClearFriends(boost::unique_lock<boost::mutex>& lock)
{
    friends_.clear();
	DISPATCH(Player, Friend);
}

std::vector<std::string> Player::GetIgnoredPlayers() { return GetIgnoredPlayers(AcquireLock()); }
std::vector<std::string> Player::GetIgnoredPlayers(boost::unique_lock<boost::mutex>& lock)
{
    return ignored_players_.raw();
}

bool Player::IsIgnored(string player_name) { return IsIgnored(player_name, AcquireLock()); }
bool Player::IsIgnored(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const std::string& x)->bool {
        return x == player_name;
    });
    
	return iter != end(ignored_players_);
}

void Player::IgnorePlayer(string player_name) { IgnorePlayer(player_name, AcquireLock()); }
void Player::IgnorePlayer(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    ignored_players_.add(player_name);
	DISPATCH(Player, IgnorePlayer);
}

void Player::StopIgnoringPlayer(string player_name) { StopIgnoringPlayer(player_name, AcquireLock()); }
void Player::StopIgnoringPlayer(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const std::string& x)->bool {
        return  x == player_name;
    });

    if (iter != end(ignored_players_))
    {    
		ignored_players_.remove(iter); 
		DISPATCH(Player, RemoveIgnoredPlayer);
	}
}

void Player::ClearIgnored() { ClearIgnored(AcquireLock()); }
void Player::ClearIgnored(boost::unique_lock<boost::mutex>& lock)
{
    ignored_players_.clear();
    DISPATCH(Player, IgnorePlayer);
}

uint32_t Player::GetLanguage() { return GetLanguage(AcquireLock()); }
uint32_t Player::GetLanguage(boost::unique_lock<boost::mutex>& lock) 
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id) { SetLanguage(language_id, AcquireLock()); }
void Player::SetLanguage(uint32_t language_id, boost::unique_lock<boost::mutex>& lock)
{
    language_ = language_id;
	DISPATCH(Player, Language);
}

uint32_t Player::GetCurrentStomach() { return GetCurrentStomach(AcquireLock()); }
uint32_t Player::GetCurrentStomach(boost::unique_lock<boost::mutex>& lock) 
{
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach) { IncreaseCurrentStomach(stomach, AcquireLock()); }
void Player::IncreaseCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;
    DISPATCH(Player, CurrentStomach);
}

void Player::DecreaseCurrentStomach(uint32_t stomach) { DecreaseCurrentStomach(stomach, AcquireLock()); }
void Player::DecreaseCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    current_stomach_ -= stomach;
    DISPATCH(Player, CurrentStomach);
}

void Player::ResetCurrentStomach(uint32_t stomach) { ResetCurrentStomach(stomach, AcquireLock()); }
void Player::ResetCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    current_stomach_ = stomach;
	DISPATCH(Player, CurrentStomach);
}

uint32_t Player::GetMaxStomach() { return GetMaxStomach(AcquireLock()); }
uint32_t Player::GetMaxStomach(boost::unique_lock<boost::mutex>& lock) 
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach) { ResetMaxStomach(stomach, AcquireLock()); }
void Player::ResetMaxStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    max_stomach_ = stomach;
	DISPATCH(Player, MaxStomach);
}

uint32_t Player::GetCurrentDrink() { return GetCurrentDrink(AcquireLock()); }
uint32_t Player::GetCurrentDrink(boost::unique_lock<boost::mutex>& lock) 
{
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink) { IncreaseCurrentDrink(drink, AcquireLock()); }
void Player::IncreaseCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
    DISPATCH(Player, CurrentDrink);
}

void Player::DecreaseCurrentDrink(uint32_t drink) { DecreaseCurrentDrink(drink, AcquireLock()); }
void Player::DecreaseCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    current_drink_ -= drink;
    DISPATCH(Player, CurrentDrink);
}

void Player::ResetCurrentDrink(uint32_t drink) { ResetCurrentDrink(drink, AcquireLock()); }
void Player::ResetCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    current_drink_ = drink;
	DISPATCH(Player, CurrentDrink);
}

uint32_t Player::GetMaxDrink()  { return GetMaxDrink(AcquireLock()); }
uint32_t Player::GetMaxDrink(boost::unique_lock<boost::mutex>& lock) 
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink) { ResetMaxDrink(drink, AcquireLock()); }
void Player::ResetMaxDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    max_drink_ = drink;
	DISPATCH(Player, MaxDrink);
}

uint32_t Player::GetJediState() { return GetJediState(AcquireLock()); }
uint32_t Player::GetJediState(boost::unique_lock<boost::mutex>& lock) 
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state) { SetJediState(jedi_state, AcquireLock()); }
void Player::SetJediState(uint32_t jedi_state, boost::unique_lock<boost::mutex>& lock)
{
    jedi_state_ = jedi_state;
	DISPATCH(Player, JediState);
}

Gender Player::GetGender() { return GetGender(AcquireLock()); }
Gender Player::GetGender(boost::unique_lock<boost::mutex>& lock) 
{
    return gender_;
}

void Player::SetGender(Gender value) { return SetGender(value, AcquireLock()); }
void Player::SetGender(Gender value, boost::unique_lock<boost::mutex>& lock)
{
    gender_ = value;
}

void Player::RemoveBadge(uint32_t id) { RemoveBadge(id, AcquireLock()); }
void Player::RemoveBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock)
{
	auto iter = std::find(badges_.begin(), badges_.end(), id);

	if(iter == badges_.end())
		return;

	badges_sync_queue_.push(std::pair<uint8_t, uint32_t>(0, id));
	badges_.erase(iter);
}

void Player::AddBadge(uint32_t id) { AddBadge(id, AcquireLock()); }
void Player::AddBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock)
{
	
	badges_sync_queue_.push(std::pair<uint8_t, uint32_t>(1, id));
	badges_.push_back(id);
	
	DISPATCH(Player, BadgeFlags);
}

bool Player::HasBadge(uint32_t id) { return HasBadge(id, AcquireLock()); }
bool Player::HasBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock)
{
	auto i = std::find(badges_.begin(), badges_.end(), id);
	if(i != badges_.end())
		return true;
	else
		return false;
}

void Player::CreateBaselines(shared_ptr<swganh::observer::ObserverInterface> observer)
{
    if (auto dispatch = GetEventDispatcher())
	{
		dispatch->Dispatch(make_shared<ObserverEvent>
			("Player::Baselines", shared_from_this(), observer));
	}
}

void Player::SerializeXp(swganh::messages::BaseSwgMessage* message) { SerializeXp(message, AcquireLock()); }
void Player::SerializeXp(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	experience_.Serialize(message);
}

void Player::SerializeWaypoints(swganh::messages::BaseSwgMessage* message) { SerializeWaypoints(message, AcquireLock()); }
void Player::SerializeWaypoints(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.Serialize(message);
}

void Player::SerializeQuests(swganh::messages::BaseSwgMessage* message) { SerializeQuests(message, AcquireLock()); }
void Player::SerializeQuests(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	quest_journal_.Serialize(message);
}

void Player::SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message) { SerializeDraftSchematics(message, AcquireLock()); }
void Player::SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	draft_schematics_.Serialize(message);
}

void Player::SerializeFriends(swganh::messages::BaseSwgMessage* message) { SerializeFriends(message, AcquireLock()); }
void Player::SerializeFriends(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	friends_.Serialize(message);
}

void Player::SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message) { SerializeIgnoredPlayers(message, AcquireLock()); }
void Player::SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	ignored_players_.Serialize(message);
}

std::list<uint32_t> Player::GetBadges() { return GetBadges(AcquireLock()); }
std::list<uint32_t> Player::GetBadges(boost::unique_lock<boost::mutex>& lock)
{
	return badges_;
}

std::queue<std::pair<uint8_t, uint32_t>> Player::GetBadgesSyncQueue() { return GetBadgesSyncQueue(AcquireLock()); }
std::queue<std::pair<uint8_t, uint32_t>> Player::GetBadgesSyncQueue(boost::unique_lock<boost::mutex>& lock)
{
	return badges_sync_queue_;
}
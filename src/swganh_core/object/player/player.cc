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

std::array<uint32_t, 4> Player::GetStatusFlags() {
    auto lock = AcquireLock();
    return GetStatusFlags(lock);
}

std::array<uint32_t, 4> Player::GetStatusFlags(boost::unique_lock<boost::mutex>& lock) 
{
    return status_flags_;
}

void Player::AddStatusFlag(StatusFlags flag, StatusIndex index) {
    auto lock = AcquireLock();
    AddStatusFlag(flag, lock, index);
}

void Player::AddStatusFlag(StatusFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
    status_flags_[index] = status_flags_[index] | flag;
	DISPATCH(Player, StatusBitmask);
}

void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index) {
    auto lock = AcquireLock();
    RemoveStatusFlag(flag, lock, index);
}

void Player::RemoveStatusFlag(StatusFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
	status_flags_[index] = status_flags_[index] & ~flag;
    DISPATCH(Player, StatusBitmask);
}

void Player::ClearStatusFlags() {
    auto lock = AcquireLock();
    ClearStatusFlags(lock);
}

void Player::ClearStatusFlags(boost::unique_lock<boost::mutex>& lock)
{
    for_each(begin(status_flags_), end(status_flags_), [] (uint32_t& value) {
		value = 0;
	});
    DISPATCH(Player, StatusBitmask);
}

std::array<uint32_t, 4> Player::GetProfileFlags() {
    auto lock = AcquireLock();
    return GetProfileFlags(lock);
}

std::array<uint32_t, 4> Player::GetProfileFlags(boost::unique_lock<boost::mutex>& lock) 
{
    return profile_flags_;
}

void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index) {
    auto lock = AcquireLock();
    AddProfileFlag(flag, lock, index);
}

void Player::AddProfileFlag(ProfileFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
	profile_flags_[index] = profile_flags_[index] | flag;
	DISPATCH(Player, ProfileFlag);
}

void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index) {
    auto lock = AcquireLock();
    RemoveProfileFlag(flag, lock, index);
}

void Player::RemoveProfileFlag(ProfileFlags flag, boost::unique_lock<boost::mutex>& lock, StatusIndex index)
{
	profile_flags_[index] = profile_flags_[index] & ~flag;
	DISPATCH(Player, ProfileFlag);
}

void Player::ClearProfileFlags() {
    auto lock = AcquireLock();
    ClearProfileFlags(lock);
}

void Player::ClearProfileFlags(boost::unique_lock<boost::mutex>& lock)
{
    for_each(begin(profile_flags_), end(profile_flags_), [] (uint32_t& value)  {
		value = 0;
	});
	DISPATCH(Player, ProfileFlag);
}

std::string Player::GetProfessionTag() {
    auto lock = AcquireLock();
    return GetProfessionTag(lock);
}

std::string Player::GetProfessionTag(boost::unique_lock<boost::mutex>& lock) 
{
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag) {
    auto lock = AcquireLock();
    SetProfessionTag(profession_tag, lock);
}
void Player::SetProfessionTag(string profession_tag, boost::unique_lock<boost::mutex>& lock)
{
    profession_tag_ = profession_tag;
	DISPATCH(Player, ProfessionTag);
}

uint32_t Player::GetBornDate() {
    auto lock = AcquireLock();
    return GetBornDate(lock);
}

uint32_t Player::GetBornDate(boost::unique_lock<boost::mutex>& lock) 
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date) {
    auto lock = AcquireLock();
    SetBornDate(born_date, lock);
}

void Player::SetBornDate(uint32_t born_date, boost::unique_lock<boost::mutex>& lock)
{
    born_date_ = born_date;
	DISPATCH(Player, BornDate);
}

uint32_t Player::GetTotalPlayTime() {
    auto lock = AcquireLock();
    return GetTotalPlayTime(lock);
}

uint32_t Player::GetTotalPlayTime(boost::unique_lock<boost::mutex>& lock) 
{
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time) {
    auto lock = AcquireLock();
    SetTotalPlayTime(play_time, lock);
}

void Player::SetTotalPlayTime(uint32_t play_time, boost::unique_lock<boost::mutex>& lock)
{
    total_playtime_ = play_time;
	DISPATCH(Player, TotalPlayTime);
}

void Player::IncrementTotalPlayTime(uint32_t increment) {
    auto lock = AcquireLock();
    IncrementTotalPlayTime(increment, lock);
}

void Player::IncrementTotalPlayTime(uint32_t increment, boost::unique_lock<boost::mutex>& lock)
{
    total_playtime_ += increment;
	DISPATCH(Player, TotalPlayTime);
}

uint8_t Player::GetAdminTag() {
    auto lock = AcquireLock();
    return GetAdminTag(lock);
}

uint8_t Player::GetAdminTag(boost::unique_lock<boost::mutex>& lock) 
{
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag) {
    auto lock = AcquireLock();
    SetAdminTag(tag, lock);
}

void Player::SetAdminTag(uint8_t tag, boost::unique_lock<boost::mutex>& lock)
{
    admin_tag_ = tag;
	DISPATCH(Player, AdminTag);
}

std::map<string, XpData> Player::GetXp() {
    auto lock = AcquireLock();
    return GetXp(lock);
}

std::map<string, XpData> Player::GetXp(boost::unique_lock<boost::mutex>& lock) 
{
    return experience_.raw();
}

void Player::AddExperience(std::string type, uint32_t value) {
    auto lock = AcquireLock();
    AddExperience(type, value, lock);
}

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

void Player::DeductXp(std::string type, uint32_t value) {
    auto lock = AcquireLock();
    DeductXp(type, value, lock);
}

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

void Player::ClearXpType(string type) {
    auto lock = AcquireLock();
    ClearXpType(type, lock);
}

void Player::ClearXpType(string type, boost::unique_lock<boost::mutex>& lock)
{
    experience_.remove(experience_.find(type));
	DISPATCH(Player, Experience);
}

std::vector<std::shared_ptr<Waypoint>> Player::GetWaypoints() {
    auto lock = AcquireLock();
    return GetWaypoints(lock);
}

std::vector<std::shared_ptr<Waypoint>> Player::GetWaypoints(boost::unique_lock<boost::mutex>& lock) 
{
    std::vector<std::shared_ptr<Waypoint>> waypoints;    
    for(const auto& i : waypoints_)
    {
        waypoints.push_back(i.second.waypoint);
    }
    return waypoints;
}

void Player::AddWaypoint(const std::shared_ptr<Waypoint>& waypoint) {
    auto lock = AcquireLock();
    AddWaypoint(waypoint, lock);
}

void Player::AddWaypoint(const std::shared_ptr<Waypoint>& waypoint, boost::unique_lock<boost::mutex>& lock)
{
    waypoint->SetContainer(shared_from_this());
	waypoints_.add(waypoint->GetObjectId(), waypoint);
	DISPATCH(Player, Waypoint);
}

void Player::RemoveWaypoint(uint64_t waypoint_id) {
    auto lock = AcquireLock();
    RemoveWaypoint(waypoint_id, lock);
}

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

void Player::ModifyWaypoint(uint64_t waypoint_id) {
    auto lock = AcquireLock();
    ModifyWaypoint(waypoint_id, lock);
}

void Player::ModifyWaypoint(uint64_t way_object_id, boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.update(way_object_id);
	DISPATCH(Player, Waypoint);
}

int32_t Player::GetCurrentForcePower() {
    auto lock = AcquireLock();
    return GetCurrentForcePower(lock);
}

int32_t Player::GetCurrentForcePower(boost::unique_lock<boost::mutex>& lock) 
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(int32_t force_power) {
    auto lock = AcquireLock();
    SetCurrentForcePower(force_power, lock);
}

void Player::SetCurrentForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock)
{
    current_force_power_ = force_power;
	DISPATCH(Player, ForcePower);
}

void Player::IncrementForcePower(int32_t force_power) {
    auto lock = AcquireLock();
    IncrementForcePower(force_power, lock);
}

void Player::IncrementForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    DISPATCH(Player, ForcePower);
}

int32_t Player::GetMaxForcePower() {
    auto lock = AcquireLock();
    return GetMaxForcePower(lock);
}

int32_t Player::GetMaxForcePower(boost::unique_lock<boost::mutex>& lock) 
{
    return max_force_power_;
}

void Player::SetMaxForcePower(int32_t force_power) {
    auto lock = AcquireLock();
    SetMaxForcePower(force_power, lock);
}

void Player::SetMaxForcePower(int32_t force_power, boost::unique_lock<boost::mutex>& lock)
{
    max_force_power_ = force_power;
	DISPATCH(Player, MaxForcePower);
}

uint32_t Player::GetCurrentForceSensitiveQuests() {
    auto lock = AcquireLock();
    return GetCurrentForceSensitiveQuests(lock);
}

uint32_t Player::GetCurrentForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask) {
    auto lock = AcquireLock();
    AddCurrentForceSensitiveQuest(quest_mask, lock);
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ | quest_mask;
    DISPATCH(Player, ForceSensitiveQuests);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask) {
    auto lock = AcquireLock();
    RemoveCurrentForceSensitiveQuest(quest_mask, lock);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ & ~quest_mask;
	DISPATCH(Player, ForceSensitiveQuests);
}

void Player::ClearCurrentForceSensitiveQuests() {
    auto lock = AcquireLock();
    ClearCurrentForceSensitiveQuests(lock);
}

void Player::ClearCurrentForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    current_force_sensitive_quests_ = 0;
	DISPATCH(Player, ForceSensitiveQuests);
}

uint32_t Player::GetCompletedForceSensitiveQuests() {
    auto lock = AcquireLock();
    return GetCompletedForceSensitiveQuests(lock);
}

uint32_t Player::GetCompletedForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask) {
    auto lock = AcquireLock();
    AddCompletedForceSensitiveQuest(quest_mask, lock);
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ | quest_mask;
    DISPATCH(Player, CompletedForceSensitiveQuests);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask) {
    auto lock = AcquireLock();
    RemoveCompletedForceSensitiveQuest(quest_mask, lock);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask, boost::unique_lock<boost::mutex>& lock)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ & ~quest_mask;
	DISPATCH(Player, CompletedForceSensitiveQuests);
}

void Player::ClearCompletedForceSensitiveQuests() {
    auto lock = AcquireLock();
    ClearCompletedForceSensitiveQuests(lock);
}

void Player::ClearCompletedForceSensitiveQuests(boost::unique_lock<boost::mutex>& lock)
{
    completed_force_sensitive_quests_ = 0;
	DISPATCH(Player, CompletedForceSensitiveQuests);
}

std::map<uint32_t, QuestJournalData> Player::GetQuests() {
    auto lock = AcquireLock();
    return GetQuests(lock);
}

std::map<uint32_t, QuestJournalData> Player::GetQuests(boost::unique_lock<boost::mutex>& lock) 
{
    return quest_journal_.raw();
}

void Player::AddQuest(QuestJournalData quest) {
    auto lock = AcquireLock();
    AddQuest(quest, lock);
}

void Player::AddQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
    quest_journal_.add(quest.quest_crc, quest);
	DISPATCH(Player, QuestJournal);
}

void Player::RemoveQuest(QuestJournalData quest) {
    auto lock = AcquireLock();
    RemoveQuest(quest, lock);
}

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

void Player::UpdateQuest(QuestJournalData quest) {
    auto lock = AcquireLock();
    UpdateQuest(quest, lock);
}

void Player::UpdateQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
	quest_journal_.update(quest.quest_crc);
    DISPATCH(Player, QuestJournal);
}

uint32_t Player::GetExperimentationFlag() {
    auto lock = AcquireLock();
    return GetExperimentationFlag(lock);
}

uint32_t Player::GetExperimentationFlag(boost::unique_lock<boost::mutex>& lock) 
{
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag) {
    auto lock = AcquireLock();
    SetExperimentationFlag(experimentation_flag, lock);
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_flag_ = experimentation_flag;
	DISPATCH(Player, ExperimentationFlag);
}

uint32_t Player::GetCraftingStage() {
    auto lock = AcquireLock();
    return GetCraftingStage(lock);
}

uint32_t Player::GetCraftingStage(boost::unique_lock<boost::mutex>& lock) 
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage) {
    auto lock = AcquireLock();
    SetCraftingStage(crafting_stage, lock);
}

void Player::SetCraftingStage(uint32_t crafting_stage, boost::unique_lock<boost::mutex>& lock)
{
    crafting_stage_ = crafting_stage;
    DISPATCH(Player, CraftingStage);
}

uint64_t Player::GetNearestCraftingStation() {
    auto lock = AcquireLock();
    return GetNearestCraftingStation(lock);
}

uint64_t Player::GetNearestCraftingStation(boost::unique_lock<boost::mutex>& lock) 
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id) {
    auto lock = AcquireLock();
    SetNearestCraftingStation(crafting_station_id, lock);
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id, boost::unique_lock<boost::mutex>& lock)
{
    nearest_crafting_station_ = crafting_station_id;
	DISPATCH(Player, NearestCraftingStation);
}

std::vector<DraftSchematicData> Player::GetDraftSchematics() {
    auto lock = AcquireLock();
    return GetDraftSchematics(lock);
}

std::vector<DraftSchematicData> Player::GetDraftSchematics(boost::unique_lock<boost::mutex>& lock) 
{
	return draft_schematics_.raw();
}

void Player::AddDraftSchematic(DraftSchematicData schematic) {
    auto lock = AcquireLock();
    AddDraftSchematic(schematic, lock);
}

void Player::AddDraftSchematic(DraftSchematicData schematic, boost::unique_lock<boost::mutex>& lock)
{ 
	draft_schematics_.add(schematic);
	DISPATCH(Player, DraftSchematic);
}

void Player::RemoveDraftSchematic(uint32_t schematic_id) {
    auto lock = AcquireLock();
    RemoveDraftSchematic(schematic_id, lock);
}

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

void Player::ClearDraftSchematics() {
    auto lock = AcquireLock();
    ClearDraftSchematics(lock);
}

void Player::ClearDraftSchematics(boost::unique_lock<boost::mutex>& lock)
{
	draft_schematics_.clear();
    DISPATCH(Player, DraftSchematic);
}

uint32_t Player::GetExperimentationPoints()  {
    auto lock = AcquireLock();
    return GetExperimentationPoints(lock);
}

uint32_t Player::GetExperimentationPoints(boost::unique_lock<boost::mutex>& lock) 
{
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points) {
    auto lock = AcquireLock();
    AddExperimentationPoints(points, lock);
}

void Player::AddExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_points_ += points;
    DISPATCH(Player, ExperimentationPoints);
}

void Player::RemoveExperimentationPoints(uint32_t points) {
    auto lock = AcquireLock();
    RemoveExperimentationPoints(points, lock);
}

void Player::RemoveExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_points_ -= points;
	DISPATCH(Player, ExperimentationPoints);
}

void Player::ResetExperimentationPoints(uint32_t points) {
    auto lock = AcquireLock();
    ResetExperimentationPoints(points, lock);
}

void Player::ResetExperimentationPoints(uint32_t points, boost::unique_lock<boost::mutex>& lock)
{
    experimentation_points_ = points;
	DISPATCH(Player, ExperimentationPoints);
}

uint32_t Player::GetAccomplishmentCounter() {
    auto lock = AcquireLock();
    return GetAccomplishmentCounter(lock);
}

uint32_t Player::GetAccomplishmentCounter(boost::unique_lock<boost::mutex>& lock) 
{
    return accomplishment_counter_;
}

void Player::ResetAccomplishmentCounter(uint32_t counter) {
    auto lock = AcquireLock();
    ResetAccomplishmentCounter(counter, lock);
}

void Player::ResetAccomplishmentCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock)
{
    accomplishment_counter_ = counter;
	DISPATCH(Player, AccomplishmentCounter);
}

void Player::IncrementAccomplishmentCounter() {
    auto lock = AcquireLock();
    IncrementAccomplishmentCounter(lock);
}

void Player::IncrementAccomplishmentCounter(boost::unique_lock<boost::mutex>& lock)
{
    ++accomplishment_counter_;
	DISPATCH(Player, AccomplishmentCounter);
}

std::vector<std::string> Player::GetFriends() {
    auto lock = AcquireLock();
    return GetFriends(lock);
}

std::vector<std::string> Player::GetFriends(boost::unique_lock<boost::mutex>& lock)
{
    return friends_.raw();
}

bool Player::IsFriend(std::string friend_name) {
    auto lock = AcquireLock();
    return IsFriend(friend_name, lock);
}

bool Player::IsFriend(std::string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(friends_), end(friends_), [=](const std::string& x)->bool {
        return x == friend_name;
    });
    
    return iter != end(friends_);
}

void Player::AddFriend(string friend_name) {
    auto lock = AcquireLock();
    AddFriend(friend_name, lock);
}

void Player::AddFriend(string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    friends_.add(friend_name);
	DISPATCH(Player, Friend);
	if(auto dispatcher = GetEventDispatcher())
	{
		dispatcher->Dispatch(std::make_shared<ValueEvent<std::pair<std::shared_ptr<Object>, std::string>>>(
			"Player::AddFriend", std::make_pair(shared_from_this(), friend_name)));
	}
}

void Player::RemoveFriend(string friend_name) {
    auto lock = AcquireLock();
    RemoveFriend(friend_name, lock);
}

void Player::RemoveFriend(string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(friends_), end(friends_), [=](const std::string& x)->bool {
        return x == friend_name;
    });
        
    if (iter != end(friends_))
    {
		friends_.remove(iter);
		DISPATCH(Player, RemoveFriend);
		if(auto dispatcher = GetEventDispatcher())
		{
			dispatcher->Dispatch(std::make_shared<ValueEvent<std::pair<std::shared_ptr<Object>, std::string>>>(
				"Player::RemoveFriend", std::make_pair(shared_from_this(), friend_name)));
		}
	}
}

void Player::ClearFriends() {
    auto lock = AcquireLock();
    ClearFriends(lock);
}

void Player::ClearFriends(boost::unique_lock<boost::mutex>& lock)
{
    friends_.clear();
	DISPATCH(Player, Friend);
}

std::vector<std::string> Player::GetIgnoredPlayers() {
    auto lock = AcquireLock();
    return GetIgnoredPlayers(lock);
}

std::vector<std::string> Player::GetIgnoredPlayers(boost::unique_lock<boost::mutex>& lock)
{
    return ignored_players_.raw();
}

bool Player::IsIgnored(string player_name) {
    auto lock = AcquireLock();
    return IsIgnored(player_name, lock);
}

bool Player::IsIgnored(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const std::string& x)->bool {
        return x == player_name;
    });
    
	return iter != end(ignored_players_);
}

void Player::IgnorePlayer(string player_name) {
    auto lock = AcquireLock();
    IgnorePlayer(player_name, lock);
}

void Player::IgnorePlayer(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    ignored_players_.add(player_name);
	DISPATCH(Player, IgnorePlayer);
}

void Player::StopIgnoringPlayer(string player_name) {
    auto lock = AcquireLock();
    StopIgnoringPlayer(player_name, lock);
}

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

void Player::ClearIgnored() {
    auto lock = AcquireLock();
    ClearIgnored(lock);
}

void Player::ClearIgnored(boost::unique_lock<boost::mutex>& lock)
{
    ignored_players_.clear();
    DISPATCH(Player, IgnorePlayer);
}

uint32_t Player::GetLanguage() {
    auto lock = AcquireLock();
    return GetLanguage(lock);
}

uint32_t Player::GetLanguage(boost::unique_lock<boost::mutex>& lock) 
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id) {
    auto lock = AcquireLock();
    SetLanguage(language_id, lock);
}

void Player::SetLanguage(uint32_t language_id, boost::unique_lock<boost::mutex>& lock)
{
    language_ = language_id;
	DISPATCH(Player, Language);
}

uint32_t Player::GetCurrentStomach() {
    auto lock = AcquireLock();
    return GetCurrentStomach(lock);
}

uint32_t Player::GetCurrentStomach(boost::unique_lock<boost::mutex>& lock) 
{
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach) {
    auto lock = AcquireLock();
    IncreaseCurrentStomach(stomach, lock);
}

void Player::IncreaseCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;
    DISPATCH(Player, CurrentStomach);
}

void Player::DecreaseCurrentStomach(uint32_t stomach) {
    auto lock = AcquireLock();
    DecreaseCurrentStomach(stomach, lock);
}

void Player::DecreaseCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    current_stomach_ -= stomach;
    DISPATCH(Player, CurrentStomach);
}

void Player::ResetCurrentStomach(uint32_t stomach) {
    auto lock = AcquireLock();
    ResetCurrentStomach(stomach, lock);
}

void Player::ResetCurrentStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    current_stomach_ = stomach;
	DISPATCH(Player, CurrentStomach);
}

uint32_t Player::GetMaxStomach() {
    auto lock = AcquireLock();
    return GetMaxStomach(lock);
}

uint32_t Player::GetMaxStomach(boost::unique_lock<boost::mutex>& lock) 
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach) {
    auto lock = AcquireLock();
    ResetMaxStomach(stomach, lock);
}

void Player::ResetMaxStomach(uint32_t stomach, boost::unique_lock<boost::mutex>& lock)
{
    max_stomach_ = stomach;
	DISPATCH(Player, MaxStomach);
}

uint32_t Player::GetCurrentDrink() {
    auto lock = AcquireLock();
    return GetCurrentDrink(lock);
}

uint32_t Player::GetCurrentDrink(boost::unique_lock<boost::mutex>& lock) 
{
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink) {
    auto lock = AcquireLock();
    IncreaseCurrentDrink(drink, lock);
}

void Player::IncreaseCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
    DISPATCH(Player, CurrentDrink);
}

void Player::DecreaseCurrentDrink(uint32_t drink) {
    auto lock = AcquireLock();
    DecreaseCurrentDrink(drink, lock);
}

void Player::DecreaseCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    current_drink_ -= drink;
    DISPATCH(Player, CurrentDrink);
}

void Player::ResetCurrentDrink(uint32_t drink) {
    auto lock = AcquireLock();
    ResetCurrentDrink(drink, lock);
}

void Player::ResetCurrentDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    current_drink_ = drink;
	DISPATCH(Player, CurrentDrink);
}

uint32_t Player::GetMaxDrink()  {
    auto lock = AcquireLock();
    return GetMaxDrink(lock);
}

uint32_t Player::GetMaxDrink(boost::unique_lock<boost::mutex>& lock) 
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink) {
    auto lock = AcquireLock();
    ResetMaxDrink(drink, lock);
}

void Player::ResetMaxDrink(uint32_t drink, boost::unique_lock<boost::mutex>& lock)
{
    max_drink_ = drink;
	DISPATCH(Player, MaxDrink);
}

uint32_t Player::GetJediState() {
    auto lock = AcquireLock();
    return GetJediState(lock);
}

uint32_t Player::GetJediState(boost::unique_lock<boost::mutex>& lock) 
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state) {
    auto lock = AcquireLock();
    SetJediState(jedi_state, lock);
}

void Player::SetJediState(uint32_t jedi_state, boost::unique_lock<boost::mutex>& lock)
{
    jedi_state_ = jedi_state;
	DISPATCH(Player, JediState);
}

Gender Player::GetGender() {
    auto lock = AcquireLock();
    return GetGender(lock);
}

Gender Player::GetGender(boost::unique_lock<boost::mutex>& lock) 
{
    return gender_;
}

void Player::SetGender(Gender value) {
    auto lock = AcquireLock();
    return SetGender(value, lock);
}

void Player::SetGender(Gender value, boost::unique_lock<boost::mutex>& lock)
{
    gender_ = value;
}

void Player::RemoveBadge(uint32_t id) {
    auto lock = AcquireLock();
    RemoveBadge(id, lock);
}

void Player::RemoveBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock)
{
	auto iter = std::find(badges_.begin(), badges_.end(), id);

	if(iter == badges_.end())
		return;

	badges_sync_queue_.push(std::pair<uint8_t, uint32_t>(0, id));
	badges_.erase(iter);
}

void Player::AddBadge(uint32_t id) {
    auto lock = AcquireLock();
    AddBadge(id, lock);
}

void Player::AddBadge(uint32_t id, boost::unique_lock<boost::mutex>& lock)
{
	
	badges_sync_queue_.push(std::pair<uint8_t, uint32_t>(1, id));
	badges_.push_back(id);
	
	DISPATCH(Player, BadgeFlags);
}

bool Player::HasBadge(uint32_t id) {
    auto lock = AcquireLock();
    return HasBadge(id, lock);
}

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

void Player::SerializeXp(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeXp(message, lock);
}

void Player::SerializeXp(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	experience_.Serialize(message);
}

void Player::SerializeWaypoints(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeWaypoints(message, lock);
}

void Player::SerializeWaypoints(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.Serialize(message);
}

void Player::SerializeQuests(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeQuests(message, lock);
}

void Player::SerializeQuests(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	quest_journal_.Serialize(message);
}

void Player::SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeDraftSchematics(message, lock);
}

void Player::SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	draft_schematics_.Serialize(message);
}

void Player::SerializeFriends(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeFriends(message, lock);
}

void Player::SerializeFriends(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	friends_.Serialize(message);
}

void Player::SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeIgnoredPlayers(message, lock);
}

void Player::SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	ignored_players_.Serialize(message);
}

std::list<uint32_t> Player::GetBadges() {
    auto lock = AcquireLock();
    return GetBadges(lock);
}

std::list<uint32_t> Player::GetBadges(boost::unique_lock<boost::mutex>& lock)
{
	return badges_;
}

std::queue<std::pair<uint8_t, uint32_t>> Player::GetBadgesSyncQueue() {
    auto lock = AcquireLock();
    return GetBadgesSyncQueue(lock);
}

std::queue<std::pair<uint8_t, uint32_t>> Player::GetBadgesSyncQueue(boost::unique_lock<boost::mutex>& lock)
{
	return badges_sync_queue_;
}

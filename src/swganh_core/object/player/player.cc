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
using namespace containers;

Player::Player()
: Intangible()
, status_flags_(array<FlagBitmask, 4>())
, profile_flags_(array<FlagBitmask, 4>())
, profession_tag_("")
, born_date_(0)
, total_playtime_(0)
, admin_tag_(0)
, region_(0)
, experience_(NetworkMap<string, XpData>())
, waypoints_(NetworkMap<uint64_t, PlayerWaypointSerializer>())
, current_force_power_(0)
, max_force_power_(0)
, current_force_sensitive_quests_(0)
, completed_force_sensitive_quests_(0)
, quest_journal_(NetworkMap<uint32_t, QuestJournalData>())
, experimentation_flag_(0)
, crafting_stage_(0)
, nearest_crafting_station_(0)
, draft_schematics_(NetworkSortedList<DraftSchematicData>())
, experimentation_points_(0)
, friends_(NetworkSortedVector<Name>(25))
, ignored_players_(NetworkSortedVector<Name>(25))
, accomplishment_counter_(0)
, language_(0)
, current_stomach_(0)
, max_stomach_(0)
, current_drink_(0)
, max_drink_(0)
, jedi_state_(0)
, gender_(MALE)
{}

std::array<FlagBitmask, 4> Player::GetStatusFlags() { return GetStatusFlags(AcquireLock()); }
std::array<FlagBitmask, 4> Player::GetStatusFlags(boost::unique_lock<boost::mutex>& lock) 
{
    return status_flags_;
}

void Player::AddStatusFlag(StatusFlags flag, StatusIndex index) { AddStatusFlag(flag, index, AcquireLock()); }
void Player::AddStatusFlag(StatusFlags flag, StatusIndex index, boost::unique_lock<boost::mutex>& lock)
{
    status_flags_[index] = FlagBitmask(status_flags_[index].bitmask | flag);
	DISPATCH(Player, StatusBitmask);
}

void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index) { RemoveStatusFlag(flag, index, AcquireLock()); }
void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index, boost::unique_lock<boost::mutex>& lock)
{
	status_flags_[index] = FlagBitmask(status_flags_[index].bitmask & ~flag);
    DISPATCH(Player, StatusBitmask);
}

void Player::ClearStatusFlags() { ClearStatusFlags(AcquireLock()); }
void Player::ClearStatusFlags(boost::unique_lock<boost::mutex>& lock)
{
    for_each(begin(status_flags_), end(status_flags_), [] (vector<FlagBitmask>::value_type& value) {
		value = FlagBitmask(0);
	});
    DISPATCH(Player, StatusBitmask);
}

std::array<FlagBitmask, 4> Player::GetProfileFlags() { return GetProfileFlags(AcquireLock()); }
std::array<FlagBitmask, 4> Player::GetProfileFlags(boost::unique_lock<boost::mutex>& lock) 
{
    return profile_flags_;
}

void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index) { AddProfileFlag(flag, index, AcquireLock()); }
void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index, boost::unique_lock<boost::mutex>& lock)
{
	profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask | flag);
	DISPATCH(Player, ProfileFlag);
}

void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index) { RemoveProfileFlag(flag, index, AcquireLock()); }
void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index, boost::unique_lock<boost::mutex>& lock)
{
	profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask & ~flag);
	DISPATCH(Player, ProfileFlag);
}

void Player::ClearProfileFlags() { ClearProfileFlags(AcquireLock()); }
void Player::ClearProfileFlags(boost::unique_lock<boost::mutex>& lock)
{
    for_each(begin(profile_flags_), end(profile_flags_), [] (vector<FlagBitmask>::value_type& value)  {
		value = FlagBitmask(0);
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
    return std::move(experience_.Get());
}

void Player::AddExperience(XpData experience) { AddExperience(experience, AcquireLock()); }
void Player::AddExperience(XpData experience, boost::unique_lock<boost::mutex>& lock)
{
    experience_.Update(experience.type, experience);
	DISPATCH(Player, Experience);
}

void Player::DeductXp(XpData experience) { DeductXp(experience, AcquireLock()); }
void Player::DeductXp(XpData experience, boost::unique_lock<boost::mutex>& lock)
{
    experience_.Update(experience.type, experience);
	DISPATCH(Player, Experience);
}

void Player::ClearXpType(string type) { ClearXpType(type, AcquireLock()); }
void Player::ClearXpType(string type, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(experience_), end(experience_), [type](pair<string, XpData> xp) {
            return xp.first == type;
	});

    if (iter == end(experience_))
    {
        return;
    }
            
    experience_.Remove(iter);
	DISPATCH(Player, Experience);
}

void Player::ResetXp(std::map<std::string, XpData>& experience) { ResetXp(experience, AcquireLock()); }
void Player::ResetXp(std::map<std::string, XpData>& experience, boost::unique_lock<boost::mutex>& lock)
{
    experience_.Clear();
    for(auto& pair : experience)
    {
        experience_.Add(pair.first, pair.second);
    }
    experience_.Reinstall();
	DISPATCH(Player, Experience);
}

void Player::ClearAllXp() P ClearAllXp(AcquireLock()); }
void Player::ClearAllXp(boost::unique_lock<boost::mutex>& lock)
{
	experience_.Clear();
	DISPATCH(Player, Experience);
}

std::map<uint64_t, PlayerWaypointSerializer> Player::GetWaypoints()  { return GetWaypoints(AcquireLock()); }
std::map<uint64_t, PlayerWaypointSerializer> Player::GetWaypoints(boost::unique_lock<boost::mutex>& lock) 
{
    return std::move(waypoints_.Get());
}

void Player::AddWaypoint(PlayerWaypointSerializer waypoint) { AddWaypoint(waypoint, AcquireLock()); }
void Player::AddWaypoint(PlayerWaypointSerializer waypoint,boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.Add(waypoint.waypoint->GetObjectId(), waypoint);
	DISPATCH(Player, Waypoint);
}

void Player::RemoveWaypoint(uint64_t waypoint_id) { RemoveWaypoint(waypoint_id, AcquireLock()); }
void Player::RemoveWaypoint(uint64_t waypoint_id, boost::unique_lock<boost::mutex>& lock)
{
	auto find_iter = find_if(
        begin(waypoints_),
        end(waypoints_),
        [waypoint_id] (pair<uint64_t, PlayerWaypointSerializer> stored_waypoint)
    {
        return waypoint_id == stored_waypoint.first;
    });

    if (find_iter == end(waypoints_))
    {
        return;
    }

    waypoints_.Remove(find_iter);
	DISPATCH(Player, Waypoint);
}

void Player::ModifyWaypoint(PlayerWaypointSerializer waypoint) { ModifyWaypoint(waypoint, AcquireLock()); }
void Player::ModifyWaypoint(PlayerWaypointSerializer waypoint, boost::unique_lock<boost::mutex>& lock)
{
	waypoints_.Update(waypoint.waypoint->GetObjectId(), waypoint);
	DISPATCH(Player, Waypoint);
}

void Player::ClearAllWaypoints() { ClearAllWaypoints(AcquireLock()); }
void Player::ClearAllWaypoints(boost::unique_lock<boost::mutex>& lock)
{
    waypoints_.Clear();
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

int32_t Player::GetMaxForcePower() { return MaxForcePower(AcquireLock()); }
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

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask) { AddCurrentForceSensitiveQuests(quest_mask, AcquireLock()); }
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

uint32_t Player::GetCompletedForceSensitiveQuests() { return GetCompleteForceSensitiveQuests(AcquireLock()); }
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
    return std::move(quest_journal_.Get());
}

void Player::AddQuest(QuestJournalData quest) { AddQuest(quest, AcquireLock()); }
void Player::AddQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
    quest_journal_.Add(quest.quest_crc, quest);
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

    quest_journal_.Remove(find_iter);
    DISPATCH(Player, QuestJournal);
}

void Player::UpdateQuest(QuestJournalData quest) { UpdateQuest(quest, AcquireLock()); }
void Player::UpdateQuest(QuestJournalData quest, boost::unique_lock<boost::mutex>& lock)
{
	quest_journal_.Update(quest.quest_crc, quest);
    DISPATCH(Player, QuestJournal);
}

void Player::ClearAllQuests() { ClearAllQuests(AcquireLock()); }
void Player::ClearAllQuests(boost::unique_lock<boost::mutex>& lock)
{
    quest_journal_.Clear();
    DISPATCH(Player, QuestJournal);
}

std::vector<Ability> Player::GetAbilityList() { return GetAbilityList(AcquireLock()); }
std::vector<Ability> Player::GetAbilityList(boost::unique_lock<boost::mutex>& lock) 
{
    //@TODO: Lock both Creature and Player simultaneous
	
	auto creature = GetContainer<Creature>();
	auto skill_commands = creature->GetSkillCommands();
	
	std::vector<Ability> abilities;
    for_each(begin(skill_commands), end(skill_commands),[&abilities](pair<uint32_t, string> skill_command){
        abilities.push_back(Ability(skill_command.second));
    });
    return abilities;
}

bool Player::HasAbility(string ability) { return HasAbility(ability, AcquireLock()); }
bool Player::HasAbility(string ability, boost::unique_lock<boost::mutex>& lock)
{
	//@TODO: Lock both Creature and Player simultaneous.

    auto creature = GetContainer<Creature>();
    auto abilities = creature->GetSkillCommands();
    
    auto find_it = find_if(begin(abilities), end(abilities),[=, &abilities](pair<uint32_t, string> skill_command){
        return (ability == skill_command.second);
       });
    if (find_it != end(abilities))
        return true;
    return false;
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

std::vector<DraftSchematicData> Player::GetDraftSchematics() { return GEtDraftSchematics(AcquireLock()); }
std::vector<DraftSchematicData> Player::GetDraftSchematics(boost::unique_lock<boost::mutex>& lock) 
{
	std::vector<DraftSchematicData> schematics;

	for(auto& v : draft_schematics_)
	{
		schematics.push_back(v.second);
	}

    return std::move(schematics);
}

void Player::AddDraftSchematic(DraftSchematicData schematic) { AddDraftSchematic(schematic, AcquireLock()); }
void Player::AddDraftSchematic(DraftSchematicData schematic, boost::unique_lock<boost::mutex>& lock)
{ 
	draft_schematics_.Add(schematic);
	DISPATCH(Player, DraftSchematic);
}

void Player::RemoveDraftSchematic(uint32_t schematic_id) { RemoveDraftSchematic(schematic_id, AcquireLock()); }
void Player::RemoveDraftSchematic(uint32_t schematic_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = draft_schematics_.Find(DraftSchematicData(schematic_id));
    if(iter == end(draft_schematics_))
    {
        return;
    }
            
    draft_schematics_.Remove(iter);
    DISPATCH(Player, DraftSchematic);
}

void Player::ClearDraftSchematics() { ClearDraftSchematics(AcquireLock()); }
void Player::ClearDraftSchematics(boost::unique_lock<boost::mutex>& lock)
{
	draft_schematics_.Clear();
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

void Player::ResetAccomplishmentCounter(uint32_t counter) { SetAccomplishmentCounter(counter, AcquireLock()); }
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

std::vector<Name> Player::GetFriends() { return GetFriends(AcquireLock()); }
std::vector<Name> Player::GetFriends(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(friends_.Get());
}

bool Player::IsFriend(std::string friend_name) { return IsFriend(friend_name, AcquireLock()); }
bool Player::IsFriend(std::string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(friends_), end(friends_), [=](const Name& x)->bool {
        return (x.Contains(friend_name));
    });
    
    if (iter != end(friends_))
        return true;

    return false;
}

void Player::AddFriend(string friend_name, uint64_t id) { AddFriend(friend_name, id, AcquireLock()); }
void Player::AddFriend(string friend_name, uint64_t id, boost::unique_lock<boost::mutex>& lock)
{
    friends_.Add(Name(friend_name, id));
	DISPATCH(Player, Friend);
}

void Player::RemoveFriend(string friend_name) { RemoveFriend(friend_name, AcquireLock()); }
void Player::RemoveFriend(string friend_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(friends_), end(friends_), [=](const Name& x)->bool {
        return (x.Contains(friend_name));
    });
        
    if (iter == end(friends_))
    {
        return;
    }
        
    friends_.ClearDeltas();
    friends_.Remove(iter);
	DISPATCH(Player, RemoveFriend);
}

void Player::ClearFriends() { ClearFriends(AcquireLock()); }
void Player::ClearFriends(boost::unique_lock<boost::mutex>& lock)
{
    friends_.Clear();
	DISPATCH(Player, Friend);
}

std::vector<Name> Player::GetIgnoredPlayers() { return GetIgnoredPlayers(AcquireLock()); }
std::vector<Name> Player::GetIgnoredPlayers(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(ignored_players_.Get());
}

bool Player::IsIgnored(string player_name) { return IsIgnored(player_name, AcquireLock()); }
bool Player::IsIgnored(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const Name& x)->bool {
        return (x.Contains(player_name));
    });
    if (iter != end(ignored_players_))
    {
        return true;  
    }
    return false;
}

void Player::IgnorePlayer(string player_name, uint64_t player_id) { IgnorePlayer(player_name, player_id, AcquireLock()); }
void Player::IgnorePlayer(string player_name, uint64_t player_id, boost::unique_lock<boost::mutex>& lock)
{
    ignored_players_.Add(Name(player_name, player_id));
	DISPATCH(Player, IgnorePlayer);
}

void Player::StopIgnoringPlayer(string player_name) { StopIgnoringPlayer(player_name, AcquireLock()); }
void Player::StopIgnoringPlayer(string player_name, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const Name& x)->bool {
        return (x.Contains(player_name));
    });

    if (iter != end(ignored_players_))
    {
        return;
    }
        
    ignored_players_.Remove(iter); 
	DISPATCH(Player, RemoveIgnoredPlayer);
}

void Player::ClearIgnored() { ClearIgnored(AcquireLock()); }
void Player::ClearIgnored(boost::unique_lock<boost::mutex>& lock)
{
    auto lock = AcquireLock();
    ignored_players_.Clear();
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

void PlayerWaypointSerializer::Serialize(swganh::messages::BaselinesMessage& message)
{
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint32_t>(0);
    auto coordinates_ = waypoint->GetCoordinates();
    message.data.write<float>(coordinates_.x);
    message.data.write<float>(coordinates_.y);
    message.data.write<float>(coordinates_.z);
    message.data.write<uint64_t>(0);
    message.data.write<uint32_t>(swganh::memcrc(waypoint->GetPlanet()));
    message.data.write<std::wstring>(waypoint->GetName());
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint8_t>(waypoint->GetColorByte());
    message.data.write<uint8_t>(waypoint->Active() ? 1 : 0);
}

void PlayerWaypointSerializer::Serialize(swganh::messages::DeltasMessage& message)
{
    message.data.write<uint32_t>(0);
    auto coordinates_ = waypoint->GetCoordinates();
    message.data.write<float>(coordinates_.x);
    message.data.write<float>(coordinates_.y);
    message.data.write<float>(coordinates_.z);
    message.data.write<uint64_t>(0);
    message.data.write<uint32_t>(swganh::memcrc(waypoint->GetPlanet()));
    message.data.write<std::wstring>(waypoint->GetName());
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint8_t>(waypoint->GetColorByte());
    message.data.write<uint8_t>(waypoint->Active() ? 1 : 0);
}

bool PlayerWaypointSerializer::operator==(const PlayerWaypointSerializer& other)
{
    return waypoint->GetObjectId() == other.waypoint->GetObjectId();
}

void Player::CreateBaselines(shared_ptr<swganh::observer::ObserverInterface> observer)
{
    if (event_dispatcher_)
	{
		GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
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

void Player::SerializeAbilities(swganh::messages::BaseSwgMessage* message) { SerializeAbilities(message, AcquireLock()); }
void Player::SerializeAbilities(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	NetworkList<Ability>(GetAbilityList()).Serialize(message);
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

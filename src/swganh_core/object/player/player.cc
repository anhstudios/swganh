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
std::array<FlagBitmask, 4> Player::GetStatusFlags() 
{
    auto lock = AcquireLock();
    return status_flags_;
}
void Player::AddStatusFlag(StatusFlags flag, StatusIndex index)
{
    {
        auto lock = AcquireLock();
        status_flags_[index] = FlagBitmask(status_flags_[index].bitmask | flag);
    }
	DISPATCH(Player, StatusBitmask);
}

void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index)
{
    {
        auto lock = AcquireLock();
        status_flags_[index] = FlagBitmask(status_flags_[index].bitmask & ~flag);
    }
    DISPATCH(Player, StatusBitmask);
}

void Player::ClearStatusFlags()
{
    {
        auto lock = AcquireLock();
    
        for_each(
            begin(status_flags_), 
            end(status_flags_),
            [] (vector<FlagBitmask>::value_type& value) 
            {
                value = FlagBitmask(0);
            });
    }
    DISPATCH(Player, StatusBitmask);
}

std::array<FlagBitmask, 4> Player::GetProfileFlags() 
{
    auto lock = AcquireLock();
    return profile_flags_;
}

void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index)
{
    {
        auto lock = AcquireLock();
        profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask | flag);
    }
	DISPATCH(Player, ProfileFlag);
}

void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index)
{
    {
        auto lock = AcquireLock();
        profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask & ~flag);
    }
	DISPATCH(Player, ProfileFlag);
}

void Player::ClearProfileFlags()
{
    {
        auto lock = AcquireLock();
        
        for_each(
            begin(profile_flags_), 
            end(profile_flags_),
            [] (vector<FlagBitmask>::value_type& value) 
        {
            value = FlagBitmask(0);
        });
    }
	DISPATCH(Player, ProfileFlag);
}

std::string Player::GetProfessionTag() 
{
    auto lock = AcquireLock();
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag)
{
    {
        auto lock = AcquireLock();
        profession_tag_ = profession_tag;
    }
	DISPATCH(Player, ProfessionTag);
}

uint32_t Player::GetBornDate() 
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date)
{
    born_date_ = born_date;
	DISPATCH(Player, BornDate);
}

uint32_t Player::GetTotalPlayTime() 
{
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time)
{
    total_playtime_ = play_time;
	DISPATCH(Player, TotalPlayTime);
}

void Player::IncrementTotalPlayTime(uint32_t increment)
{
    total_playtime_ += increment;
	DISPATCH(Player, TotalPlayTime);
}

uint8_t Player::GetAdminTag() 
{
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag)
{
    admin_tag_ = tag;
	DISPATCH(Player, AdminTag);
}

std::map<string, XpData> Player::GetXp() 
{
    auto lock = AcquireLock();
    return std::move(experience_.Get());
}

void Player::AddExperience(XpData experience)
{
    {
        auto lock = AcquireLock();
        experience_.Update(experience.type, experience);
    }
	DISPATCH(Player, Experience);
}

void Player::DeductXp(XpData experience)
{
    {
        auto lock = AcquireLock();
        experience_.Update(experience.type, experience);
    }
	DISPATCH(Player, Experience);
}

void Player::ClearXpType(string type)
{
    {
        auto lock = AcquireLock();
        auto iter = find_if(begin(experience_), end(experience_), [type](pair<string, XpData> xp) {
            return xp.first == type;
        });
        if (iter == end(experience_))
        {
            return;
        }
            
        experience_.Remove(iter);
    }
	DISPATCH(Player, Experience);
}

void Player::ResetXp(std::map<std::string, XpData>& experience)
{
    {
        auto lock = AcquireLock();
        experience_.Clear();
        for(auto& pair : experience)
        {
            experience_.Add(pair.first, pair.second);
        }
        experience_.Reinstall();
    }
	DISPATCH(Player, Experience);
}

void Player::ClearAllXp()
{
    {
        auto lock = AcquireLock();
        experience_.Clear();
    }
	DISPATCH(Player, Experience);
}

std::map<uint64_t, PlayerWaypointSerializer> Player::GetWaypoints() 
{
    auto lock = AcquireLock();
    return std::move(waypoints_.Get());
}

void Player::AddWaypoint(PlayerWaypointSerializer waypoint)
{
    {
        auto lock = AcquireLock();
        waypoints_.Add(waypoint.waypoint->GetObjectId(), waypoint);
    }
	DISPATCH(Player, Waypoint);
}

void Player::RemoveWaypoint(uint64_t waypoint_id)
{
    {
        auto lock = AcquireLock();
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
    }
	DISPATCH(Player, Waypoint);
}

void Player::ModifyWaypoint(PlayerWaypointSerializer waypoint)
{
    {
        auto lock = AcquireLock();
        waypoints_.Update(waypoint.waypoint->GetObjectId(), waypoint);
    }
	DISPATCH(Player, Waypoint);
}

void Player::ClearAllWaypoints()
{
    {
        auto lock = AcquireLock();
        waypoints_.Clear();
    }
	DISPATCH(Player, Waypoint);
}

int32_t Player::GetCurrentForcePower() 
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(int32_t force_power)
{
    current_force_power_ = force_power;
	DISPATCH(Player, ForcePower);
}

void Player::IncrementForcePower(int32_t force_power)
{
    int32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    DISPATCH(Player, ForcePower);
}

int32_t Player::GetMaxForcePower() 
{
    return max_force_power_;
}

void Player::SetMaxForcePower(int32_t force_power)
{
    max_force_power_ = force_power;
	DISPATCH(Player, MaxForcePower);
}

uint32_t Player::GetCurrentForceSensitiveQuests()
{
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ | quest_mask;
    DISPATCH(Player, ForceSensitiveQuests);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ & ~quest_mask;
	DISPATCH(Player, ForceSensitiveQuests);
}

void Player::ClearCurrentForceSensitiveQuests()
{
    current_force_sensitive_quests_ = 0;
	DISPATCH(Player, ForceSensitiveQuests);
}

uint32_t Player::GetCompletedForceSensitiveQuests()
{
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ | quest_mask;
    DISPATCH(Player, CompletedForceSensitiveQuests);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ & ~quest_mask;
	DISPATCH(Player, CompletedForceSensitiveQuests);
}

void Player::ClearCompletedForceSensitiveQuests()
{
    completed_force_sensitive_quests_ = 0;
	DISPATCH(Player, CompletedForceSensitiveQuests);
}

std::map<uint32_t, QuestJournalData> Player::GetQuests() 
{
    auto lock = AcquireLock();
    return std::move(quest_journal_.Get());
}

void Player::AddQuest(QuestJournalData quest)
{
    {
        auto lock = AcquireLock();
        quest_journal_.Add(quest.quest_crc, quest);
    }
	DISPATCH(Player, QuestJournal);
}

void Player::RemoveQuest(QuestJournalData quest)
{
    {
        auto lock = AcquireLock();
        
        auto find_iter = find_if(
            begin(quest_journal_),
            end(quest_journal_),
            [&quest] ( pair<uint32_t, QuestJournalData> stored_quest)
        {
            return quest.quest_crc == stored_quest.first;
        });

        if (find_iter == end(quest_journal_))
        {
            return;
        }

        quest_journal_.Remove(find_iter);
    }
    DISPATCH(Player, QuestJournal);
}

void Player::UpdateQuest(QuestJournalData quest)
{
    {
        auto lock = AcquireLock();
        quest_journal_.Update(quest.quest_crc, quest);
    }
    DISPATCH(Player, QuestJournal);
}

void Player::ClearAllQuests()
{
    {
        auto lock = AcquireLock();
        quest_journal_.Clear();
    }
    DISPATCH(Player, QuestJournal);
}

std::vector<Ability> Player::GetAbilityList() 
{
    auto creature = GetContainer<Creature>();
	auto skill_commands = creature->GetSkillCommands();
	
	auto lock = AcquireLock();
	std::vector<Ability> abilities;
    for_each(begin(skill_commands), end(skill_commands),[&abilities](pair<uint32_t, string> skill_command){
        abilities.push_back(Ability(skill_command.second));
    });
    return abilities;
}

bool Player::HasAbility(string ability)
{
    auto creature = GetContainer<Creature>();
    auto abilities = creature->GetSkillCommands();
    
    auto lock = AcquireLock();

    auto find_it = find_if(begin(abilities), end(abilities),[=, &abilities](pair<uint32_t, string> skill_command){
        return (ability == skill_command.second);
       });
    if (find_it != end(abilities))
        return true;
    return false;
}

uint32_t Player::GetExperimentationFlag() 
{
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag)
{
    experimentation_flag_ = experimentation_flag;
	DISPATCH(Player, ExperimentationFlag);
}

uint32_t Player::GetCraftingStage() 
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage)
{
    crafting_stage_ = crafting_stage;
    DISPATCH(Player, CraftingStage);
}

uint64_t Player::GetNearestCraftingStation() 
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id)
{
    nearest_crafting_station_ = crafting_station_id;
	DISPATCH(Player, NearestCraftingStation);
}

std::vector<DraftSchematicData> Player::GetDraftSchematics() 
{
    auto lock = AcquireLock();
	std::vector<DraftSchematicData> schematics;

	for(auto& v : draft_schematics_)
	{
		schematics.push_back(v.second);
	}

    return std::move(schematics);
}

void Player::AddDraftSchematic(DraftSchematicData schematic)
{
    {
        auto lock = AcquireLock();
        draft_schematics_.Add(schematic);
    }
	DISPATCH(Player, DraftSchematic);
}

void Player::RemoveDraftSchematic(uint32_t schematic_id)
{
    {
        auto lock = AcquireLock();
        auto iter = draft_schematics_.Find(DraftSchematicData(schematic_id));
        if(iter == end(draft_schematics_))
        {
            return;
        }
            
        draft_schematics_.Remove(iter);
    }
    DISPATCH(Player, DraftSchematic);
}

void Player::ClearDraftSchematics()
{
    {
        auto lock = AcquireLock();
        draft_schematics_.Clear();
    }
    DISPATCH(Player, DraftSchematic);
}

uint32_t Player::GetExperimentationPoints() 
{
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points)
{
    experimentation_points_ += points;
    DISPATCH(Player, ExperimentationPoints);
}

void Player::RemoveExperimentationPoints(uint32_t points)
{
    experimentation_points_ -= points;
	DISPATCH(Player, ExperimentationPoints);
}

void Player::ResetExperimentationPoints(uint32_t points)
{
    experimentation_points_ = points;
	DISPATCH(Player, ExperimentationPoints);
}

uint32_t Player::GetAccomplishmentCounter() 
{
    return accomplishment_counter_;
}

void Player::ResetAccomplishmentCounter(uint32_t counter)
{
    accomplishment_counter_ = counter;
	DISPATCH(Player, AccomplishmentCounter);
}

void Player::IncrementAccomplishmentCounter()
{
    ++accomplishment_counter_;
	DISPATCH(Player, AccomplishmentCounter);
}

std::vector<Name> Player::GetFriends()
{
    auto lock = AcquireLock();
    return std::move(friends_.Get());
}
bool Player::IsFriend(std::string friend_name)
{
    auto lock = AcquireLock();
    auto iter = find_if(begin(friends_), end(friends_), [=](const Name& x)->bool {
        return (x.Contains(friend_name));
    });
    
    if (iter != end(friends_))
        return true;

    return false;
}
void Player::AddFriend(string friend_name, uint64_t id)
{
    {
        auto lock = AcquireLock();
        friends_.Add(Name(friend_name, id));
    }
	DISPATCH(Player, Friend);
}

void Player::RemoveFriend(string friend_name)
{
    {
        auto lock = AcquireLock();
        auto iter = find_if(begin(friends_), end(friends_), [=](const Name& x)->bool {
            return (x.Contains(friend_name));
        });
        
        if (iter == end(friends_))
        {
            return;
        }
        
        friends_.ClearDeltas();
        friends_.Remove(iter);
    }
	DISPATCH(Player, RemoveFriend);
}

void Player::ClearFriends()
{
    {
        auto lock = AcquireLock();
        friends_.Clear();
    }
	DISPATCH(Player, Friend);
}

std::vector<Name> Player::GetIgnoredPlayers()
{
    auto lock = AcquireLock();
    return std::move(ignored_players_.Get());
}

bool Player::IsIgnored(string player_name)
{
    auto lock = AcquireLock();
    auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const Name& x)->bool {
        return (x.Contains(player_name));
    });
    if (iter != end(ignored_players_))
    {
        return true;  
    }
    return false;
}

void Player::IgnorePlayer(string player_name, uint64_t player_id)
{
    {
        auto lock = AcquireLock();
        ignored_players_.Add(Name(player_name, player_id));
    }
	DISPATCH(Player, IgnorePlayer);
}

void Player::StopIgnoringPlayer(string player_name)
{
    {
        auto lock = AcquireLock();
        auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const Name& x)->bool {
            return (x.Contains(player_name));
        });

        if (iter != end(ignored_players_))
        {
            return;
        }
        
        ignored_players_.Remove(iter); 
    } 
	DISPATCH(Player, RemoveIgnoredPlayer);
}

void Player::ClearIgnored()
{
    {
        auto lock = AcquireLock();
        ignored_players_.Clear();
    }
    DISPATCH(Player, IgnorePlayer);
}

uint32_t Player::GetLanguage() 
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id)
{
    language_ = language_id;
	DISPATCH(Player, Language);
}

uint32_t Player::GetCurrentStomach() 
{
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach)
{
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;
    DISPATCH(Player, CurrentStomach);
}

void Player::DecreaseCurrentStomach(uint32_t stomach)
{
    current_stomach_ -= stomach;
    DISPATCH(Player, CurrentStomach);
}

void Player::ResetCurrentStomach(uint32_t stomach)
{
    current_stomach_ = stomach;
	DISPATCH(Player, CurrentStomach);
}

uint32_t Player::GetMaxStomach() 
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach)
{
    max_stomach_ = stomach;
	DISPATCH(Player, MaxStomach);
}

uint32_t Player::GetCurrentDrink() 
{
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink)
{
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
    DISPATCH(Player, CurrentDrink);
}

void Player::DecreaseCurrentDrink(uint32_t drink)
{
    current_drink_ -= drink;
    DISPATCH(Player, CurrentDrink);
}

void Player::ResetCurrentDrink(uint32_t drink)
{
    current_drink_ = drink;
	DISPATCH(Player, CurrentDrink);
}

uint32_t Player::GetMaxDrink() 
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink)
{
    max_drink_ = drink;
	DISPATCH(Player, MaxDrink);
}

uint32_t Player::GetJediState() 
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state)
{
    jedi_state_ = jedi_state;
	DISPATCH(Player, JediState);
}

Gender Player::GetGender() 
{
    auto lock = AcquireLock();
    return gender_;
}
void Player::SetGender(Gender value)
{
    auto lock = AcquireLock();
    gender_ = value;
}

void Player::RemoveBadge(uint32_t id)
{
	auto lock = AcquireLock();
	auto iter = std::find(badges_.begin(), badges_.end(), id);

	if(iter == badges_.end())
		return;

	badges_sync_queue_.push(std::pair<uint8_t, uint32_t>(0, id));
	badges_.erase(iter);
}

void Player::AddBadge(uint32_t id)
{
	auto lock = AcquireLock();
	
	badges_sync_queue_.push(std::pair<uint8_t, uint32_t>(1, id));
	badges_.push_back(id);
	

	DISPATCH(Player, BadgeFlags);
}

bool Player::HasBadge(uint32_t id)
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

void Player::SerializeXp(swganh::messages::BaseSwgMessage* message)
{
	auto lock = AcquireLock();
	experience_.Serialize(message);
}

void Player::SerializeWaypoints(swganh::messages::BaseSwgMessage* message)
{
	auto lock = AcquireLock();
	waypoints_.Serialize(message);
}

void Player::SerializeQuests(swganh::messages::BaseSwgMessage* message)
{
	auto lock = AcquireLock();
	quest_journal_.Serialize(message);
}

void Player::SerializeAbilities(swganh::messages::BaseSwgMessage* message)
{
	auto list = GetAbilityList();
	NetworkList<Ability>(GetAbilityList()).Serialize(message);
}

void Player::SerializeDraftSchematics(swganh::messages::BaseSwgMessage* message)
{
	auto lock = AcquireLock();
	draft_schematics_.Serialize(message);
}

void Player::SerializeFriends(swganh::messages::BaseSwgMessage* message)
{
	auto lock = AcquireLock();
	friends_.Serialize(message);
}

void Player::SerializeIgnoredPlayers(swganh::messages::BaseSwgMessage* message)
{
	auto lock = AcquireLock();
	ignored_players_.Serialize(message);
}

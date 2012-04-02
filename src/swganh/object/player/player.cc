
#include "player.h"

#include "anh/logger.h"

#include "anh/crc.h"

#include "player_events.h"
#include "player_message_builder.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/waypoint/waypoint.h"
#include "swganh/messages/deltas_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::object::waypoint;
using namespace swganh::messages;
using namespace containers;

Player::Player()
: status_flags_(array<FlagBitmask, 4>())
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
    std::lock_guard<std::mutex> lock(player_mutex_);
    return status_flags_;
}
void Player::AddStatusFlag(StatusFlags flag, StatusIndex index)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        status_flags_[index] = FlagBitmask(status_flags_[index].bitmask | flag);
    }

    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        status_flags_[index] = FlagBitmask(status_flags_[index].bitmask & ~flag);
    }

    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

void Player::ClearStatusFlags()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);

        for_each(
            begin(status_flags_), 
            end(status_flags_),
            [] (vector<FlagBitmask>::value_type& value) 
        {
            value = FlagBitmask(0);
        });
    }

    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

std::array<FlagBitmask, 4> Player::GetProfileFlags() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return profile_flags_;
}

void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask | flag);
    }

    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask & ~flag);
    }

    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

void Player::ClearProfileFlags()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        
        for_each(
            begin(profile_flags_), 
            end(profile_flags_),
            [] (vector<FlagBitmask>::value_type& value) 
        {
            value = FlagBitmask(0);
        });
    }

    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

std::string Player::GetProfessionTag() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        profession_tag_ = profession_tag;
    }

    PlayerMessageBuilder::BuildProfessionTagDelta(this);
}

uint32_t Player::GetBornDate() 
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date)
{
    born_date_ = born_date;
    PlayerMessageBuilder::BuildBornDateDelta(this);
}

uint32_t Player::GetTotalPlayTime() 
{
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time)
{
    total_playtime_ = play_time;
    PlayerMessageBuilder::BuildPlayTimeDelta(this);
}

void Player::IncrementTotalPlayTime(uint32_t increment)
{
    total_playtime_ += increment;
    PlayerMessageBuilder::BuildPlayTimeDelta(this);
}

uint8_t Player::GetAdminTag() 
{
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag)
{
    admin_tag_ = tag;
    PlayerMessageBuilder::BuildAdminTagDelta(this);
}

NetworkMap<string, XpData> Player::GetXp() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return experience_;
}

void Player::AddExperience(XpData experience)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        experience_.Update(experience.type, experience);
    }

    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::DeductXp(XpData experience)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        experience_.Update(experience.type, experience);
    }

    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::ClearXpType(string type)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        auto iter = find_if(begin(experience_), end(experience_), [type](pair<string, XpData> xp) {
            return xp.first == type;
        });
        if (iter == end(experience_))
        {
            return;
        }
            
        experience_.Remove(iter);
    }

    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::ResetXp(swganh::messages::containers::NetworkMap<std::string, XpData>& experience)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        experience_.Clear();
        for(auto& pair : experience)
        {
            experience_.Add(pair.first, pair.second);
        }
        experience_.Reinstall();
    }

    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::ClearAllXp()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        experience_.Clear();
    }

    PlayerMessageBuilder::BuildXpDelta(this);
}

NetworkMap<uint64_t, PlayerWaypointSerializer> Player::GetWaypoints() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return waypoints_;
}

void Player::AddWaypoint(PlayerWaypointSerializer waypoint)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        waypoints_.Add(waypoint.waypoint->GetObjectId(), waypoint);
    }

    PlayerMessageBuilder::BuildWaypointDelta(this);
}

void Player::RemoveWaypoint(uint64_t waypoint_id)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
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

    PlayerMessageBuilder::BuildWaypointDelta(this);
}

void Player::ModifyWaypoint(PlayerWaypointSerializer waypoint)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        waypoints_.Update(waypoint.waypoint->GetObjectId(), waypoint);
    }

    PlayerMessageBuilder::BuildWaypointDelta(this);
}

void Player::ClearAllWaypoints()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        waypoints_.Clear();
    }

    PlayerMessageBuilder::BuildWaypointDelta(this);
}

uint32_t Player::GetCurrentForcePower() 
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(uint32_t force_power)
{
    current_force_power_ = force_power;
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

void Player::IncrementForcePower(int32_t force_power)
{
    uint32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

uint32_t Player::GetMaxForcePower() 
{
    return max_force_power_;
}

void Player::SetMaxForcePower(uint32_t force_power)
{
    max_force_power_ = force_power;
    PlayerMessageBuilder::BuildMaxForcePowerDelta(this);
}

uint32_t Player::GetCurrentForceSensitiveQuests()
{
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ | quest_mask;
    PlayerMessageBuilder::BuildForceSensitiveQuestDelta(this);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ & ~quest_mask;
    PlayerMessageBuilder::BuildForceSensitiveQuestDelta(this);
}

void Player::ClearCurrentForceSensitiveQuests()
{
    current_force_sensitive_quests_ = 0;
    PlayerMessageBuilder::BuildForceSensitiveQuestDelta(this);
}

uint32_t Player::GetCompletedForceSensitiveQuests()
{
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ | quest_mask;
    PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(this);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ & ~quest_mask;
    PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(this);
}

void Player::ClearCompletedForceSensitiveQuests()
{
    completed_force_sensitive_quests_ = 0;
    PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(this);
}

swganh::messages::containers::NetworkMap<uint32_t, QuestJournalData> Player::GetQuests() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return quest_journal_;
}

void Player::AddQuest(QuestJournalData quest)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        quest_journal_.Add(quest.quest_crc, quest);
    }

    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

void Player::RemoveQuest(QuestJournalData quest)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        
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

    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

void Player::UpdateQuest(QuestJournalData quest)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        quest_journal_.Update(quest.quest_crc, quest);
    }

    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

void Player::ClearAllQuests()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        quest_journal_.Clear();
    }

    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

swganh::messages::containers::NetworkSortedList<Ability> Player::GetAbilityList() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);

    auto creature = GetContainer<creature::Creature>();
    auto skill_commands = creature->GetSkillCommands();
    swganh::messages::containers::NetworkSortedList<Ability> abilities;
    for_each(begin(skill_commands), end(skill_commands),[&abilities](pair<uint32_t, string> skill_command){
        abilities.Add(Ability(skill_command.second));
    });
    return abilities;
}

bool Player::HasAbility(string ability)
{
    auto creature = GetContainer<creature::Creature>();
    auto abilities = creature->GetSkillCommands();
    
    std::lock_guard<std::mutex> lock(player_mutex_);

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
    PlayerMessageBuilder::BuildExperimentationFlagDelta(this);
}

uint32_t Player::GetCraftingStage() 
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage)
{
    crafting_stage_ = crafting_stage;
    PlayerMessageBuilder::BuildCraftingStageDelta(this);
}

uint64_t Player::GetNearestCraftingStation() 
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id)
{
    nearest_crafting_station_ = crafting_station_id;
    PlayerMessageBuilder::BuildNearestCraftingStationDelta(this);
}

swganh::messages::containers::NetworkSortedList<DraftSchematicData> Player::GetDraftSchematics() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return draft_schematics_;
}

void Player::AddDraftSchematic(DraftSchematicData schematic)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        draft_schematics_.Add(schematic);
    }

    PlayerMessageBuilder::BuildDraftSchematicDelta(this);
}

void Player::RemoveDraftSchematic(uint32_t schematic_id)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        auto iter = draft_schematics_.Find(DraftSchematicData(schematic_id));
        if(iter == end(draft_schematics_))
        {
            return;
        }
            
        draft_schematics_.Remove(iter);
    }
    
    PlayerMessageBuilder::BuildDraftSchematicDelta(this);
}

void Player::ClearDraftSchematics()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        draft_schematics_.Clear();
    }

    PlayerMessageBuilder::BuildDraftSchematicDelta(this);
}

uint32_t Player::GetExperimentationPoints() 
{
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points)
{
    experimentation_points_ += points;
    PlayerMessageBuilder::BuildExperimentationPointsDelta(this);
}

void Player::RemoveExperimentationPoints(uint32_t points)
{
    experimentation_points_ -= points;
    PlayerMessageBuilder::BuildExperimentationPointsDelta(this);
}

void Player::ResetExperimentationPoints(uint32_t points)
{
    experimentation_points_ = points;
    PlayerMessageBuilder::BuildExperimentationPointsDelta(this);
}

uint32_t Player::GetAccomplishmentCounter() 
{
    return accomplishment_counter_;
}

void Player::ResetAccomplishmentCounter(uint32_t counter)
{
    accomplishment_counter_ = counter;
    PlayerMessageBuilder::BuildAccomplishmentCounterDelta(this);
}

void Player::IncrementAccomplishmentCounter()
{
    ++accomplishment_counter_;
    PlayerMessageBuilder::BuildAccomplishmentCounterDelta(this);
}

NetworkSortedVector<Name> Player::GetFriends()
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return friends_;
}
bool Player::IsFriend(std::string friend_name)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    auto iter = find_if(begin(friends_), end(friends_), [=](const Name& x)->bool {
        return (x.Contains(friend_name));
    });
    
    if (iter != end(friends_))
        return true;

    return false;
}
void Player::AddFriend(string  friend_name, uint64_t id)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        friends_.Add(Name(friend_name, id));
    }

    PlayerMessageBuilder::BuildFriendsDelta(this);
}

void Player::RemoveFriend(string friend_name)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        auto iter = find_if(begin(friends_), end(friends_), [=](const Name& x)->bool {
            return (x.Contains(friend_name));
        });
        
        if (iter == end(friends_))
        {
            return;
        }
            
        GetEventDispatcher()->Dispatch(make_shared<NameEvent>("Player::RemoveFriend", static_pointer_cast<Player>(shared_from_this()), iter->id));
        friends_.Remove(iter);
    }

    PlayerMessageBuilder::BuildFriendsDelta(this);  
}

void Player::ClearFriends()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        friends_.Clear();
    }

    PlayerMessageBuilder::BuildFriendsDelta(this);
}

NetworkSortedVector<Name> Player::GetIgnoredPlayers()
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return ignored_players_;
}

bool Player::IsIgnored(string player_name)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
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
        std::lock_guard<std::mutex> lock(player_mutex_);
        ignored_players_.Add(Name(player_name, player_id));
    }

    PlayerMessageBuilder::BuildIgnoredDelta(this);    
}

void Player::StopIgnoringPlayer(string player_name)
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        auto iter = find_if(begin(ignored_players_), end(ignored_players_), [=](const Name& x)->bool {
            return (x.Contains(player_name));
        });

        if (iter != end(ignored_players_))
        {
            return;
        }
            
        GetEventDispatcher()->Dispatch(make_shared<NameEvent>("Player::RemoveIgnoredPlayer", static_pointer_cast<Player>(shared_from_this()), iter->id));
        ignored_players_.Remove(iter);  
    }
            
    PlayerMessageBuilder::BuildIgnoredDelta(this);    
}

void Player::ClearIgnored()
{
    {
        std::lock_guard<std::mutex> lock(player_mutex_);
        ignored_players_.Clear();
    }

    PlayerMessageBuilder::BuildIgnoredDelta(this);
}

uint32_t Player::GetLanguage() 
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id)
{
    language_ = language_id;
    PlayerMessageBuilder::BuildLanguageDelta(this);
}

uint32_t Player::GetCurrentStomach() 
{
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach)
{
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;
    PlayerMessageBuilder::BuildCurrentStomachDelta(this);
}

void Player::DecreaseCurrentStomach(uint32_t stomach)
{
    current_stomach_ -= stomach;
    PlayerMessageBuilder::BuildCurrentStomachDelta(this);
}

void Player::ResetCurrentStomach(uint32_t stomach)
{
    current_stomach_ = stomach;
    PlayerMessageBuilder::BuildCurrentStomachDelta(this);
}

uint32_t Player::GetMaxStomach() 
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach)
{
    max_stomach_ = stomach;
    PlayerMessageBuilder::BuildMaxStomachDelta(this);
}

uint32_t Player::GetCurrentDrink() 
{
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink)
{
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
    PlayerMessageBuilder::BuildCurrentDrinkDelta(this);
}

void Player::DecreaseCurrentDrink(uint32_t drink)
{
    current_drink_ -= drink;
    PlayerMessageBuilder::BuildCurrentDrinkDelta(this);
}

void Player::ResetCurrentDrink(uint32_t drink)
{
    current_drink_ = drink;
    PlayerMessageBuilder::BuildCurrentDrinkDelta(this);
}

uint32_t Player::GetMaxDrink() 
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink)
{
    max_drink_ = drink;
    PlayerMessageBuilder::BuildMaxDrinkDelta(this);
}

uint32_t Player::GetJediState() 
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state)
{
    jedi_state_ = jedi_state;
    PlayerMessageBuilder::BuildJediStateDelta(this);
}

Gender Player::GetGender() 
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    return gender_;
}
void Player::SetGender(Gender value)
{
    std::lock_guard<std::mutex> lock(player_mutex_);
    gender_ = value;
}

boost::optional<BaselinesMessage> Player::GetBaseline3()
{
    return PlayerMessageBuilder::BuildBaseline3(this);
}

boost::optional<BaselinesMessage> Player::GetBaseline6()
{
    return PlayerMessageBuilder::BuildBaseline6(this);
}

boost::optional<BaselinesMessage> Player::GetBaseline8()
{
    return PlayerMessageBuilder::BuildBaseline8(this);
}

boost::optional<BaselinesMessage> Player::GetBaseline9()
{
    return PlayerMessageBuilder::BuildBaseline9(this);
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
    message.data.write<uint32_t>(anh::memcrc(waypoint->GetPlanet()));
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
    message.data.write<uint32_t>(anh::memcrc(waypoint->GetPlanet()));
    message.data.write<std::wstring>(waypoint->GetName());
    message.data.write<uint64_t>(waypoint->GetObjectId());
    message.data.write<uint8_t>(waypoint->GetColorByte());
    message.data.write<uint8_t>(waypoint->Active() ? 1 : 0);
}

bool PlayerWaypointSerializer::operator==(const PlayerWaypointSerializer& other)
{
    return waypoint->GetObjectId() == other.waypoint->GetObjectId();
}

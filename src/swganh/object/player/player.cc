
#include "swganh/object/player/player.h"

#include "anh/logger.h"

#include "anh/crc.h"

#include "swganh/object/player/player_message_builder.h"
#include "swganh/object/creature/creature.h"
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
, waypoints_(NetworkMap<uint64_t, WaypointData>())
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
, accomplishment_counter_(0)
, friends_(NetworkSortedVector<Name>(25))
, ignored_players_(NetworkSortedVector<Name>(25))
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
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return status_flags_;
}
void Player::AddStatusFlag(StatusFlags flag, StatusIndex index)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    status_flags_[index] = FlagBitmask(status_flags_[index].bitmask | flag);
    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

void Player::RemoveStatusFlag(StatusFlags flag, StatusIndex index)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    status_flags_[index] = FlagBitmask(status_flags_[index].bitmask & ~flag);
    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

void Player::ClearStatusFlags()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);

    for_each(
        begin(status_flags_), 
        end(status_flags_),
        [] (vector<FlagBitmask>::value_type& value) 
    {
        value = FlagBitmask(0);
    });

    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

std::array<FlagBitmask, 4> Player::GetProfileFlags() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return profile_flags_;
}

void Player::AddProfileFlag(ProfileFlags flag, StatusIndex index)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask | flag);
    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

void Player::RemoveProfileFlag(ProfileFlags flag, StatusIndex index)
{    
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    profile_flags_[index] = FlagBitmask(profile_flags_[index].bitmask & ~flag);
    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

void Player::ClearProfileFlags()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    
    for_each(
        begin(profile_flags_), 
        end(profile_flags_),
        [] (vector<FlagBitmask>::value_type& value) 
    {
        value = FlagBitmask(0);
    });

    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

std::string Player::GetProfessionTag() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    profession_tag_ = profession_tag;
    PlayerMessageBuilder::BuildProfessionTagDelta(this);
}

uint32_t Player::GetBornDate() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    born_date_ = born_date;
    PlayerMessageBuilder::BuildBornDateDelta(this);
}

uint32_t Player::GetTotalPlayTime() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    total_playtime_ = play_time;
    PlayerMessageBuilder::BuildPlayTimeDelta(this);
}

void Player::IncrementTotalPlayTime(uint32_t increment)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    total_playtime_ += increment;
    PlayerMessageBuilder::BuildPlayTimeDelta(this);
}

uint8_t Player::GetAdminTag() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    admin_tag_ = tag;
    PlayerMessageBuilder::BuildAdminTagDelta(this);
}

NetworkMap<string, XpData> Player::GetXp() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return experience_;
}

void Player::AddExperience(XpData experience)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experience_.Update(experience.type, experience);
    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::DeductXp(XpData experience)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experience_.Update(experience.type, experience);
    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::ClearXpType(string type)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto iter = find_if(experience_.Begin(), experience_.End(), [type](pair<string, XpData> xp) {
        return xp.first == type;
    });
    if (iter != experience_.End())
    {
        experience_.Remove(iter);
        PlayerMessageBuilder::BuildXpDelta(this);
    }
}

void Player::ResetXp(swganh::messages::containers::NetworkMap<std::string, XpData>& experience)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experience_.Clear();
    for_each(experience_.Begin(), experience_.End(), [=] (pair<string, XpData> pair) {
        experience_.Add(pair.first, pair.second);
    });
    experience_.Reinstall();

    PlayerMessageBuilder::BuildXpDelta(this);
}

void Player::ClearAllXp()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experience_.Clear();
    PlayerMessageBuilder::BuildXpDelta(this);
}

NetworkMap<uint64_t, WaypointData> Player::GetWaypoints() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return waypoints_;
}

void Player::AddWaypoint(WaypointData waypoint)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    waypoints_.Add(waypoint.object_id_, waypoint);
    PlayerMessageBuilder::BuildWaypointDelta(this);
}

void Player::RemoveWaypoint(uint64_t waypoint_id)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto find_iter = find_if(
        waypoints_.Begin(),
        waypoints_.End(),
        [waypoint_id] (pair<uint64_t, WaypointData> stored_waypoint)
    {
        return waypoint_id == stored_waypoint.first;
    });

    if (find_iter != waypoints_.End())
    {
        waypoints_.Remove(find_iter);
        PlayerMessageBuilder::BuildWaypointDelta(this);
    }
    
}

void Player::ModifyWaypoint(WaypointData waypoint)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    waypoints_.Update(waypoint.object_id_, waypoint);
    PlayerMessageBuilder::BuildWaypointDelta(this);
}

void Player::ClearAllWaypoints()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    waypoints_.Clear();
    PlayerMessageBuilder::BuildWaypointDelta(this);
}

uint32_t Player::GetCurrentForcePower() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return current_force_power_;
}

void Player::SetCurrentForcePower(uint32_t force_power)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_force_power_ = force_power;
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

void Player::IncrementForcePower(int32_t force_power)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    uint32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

uint32_t Player::GetMaxForcePower() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return max_force_power_;
}

void Player::SetMaxForcePower(uint32_t force_power)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    max_force_power_ = force_power;
    PlayerMessageBuilder::BuildMaxForcePowerDelta(this);
}

uint32_t Player::GetCurrentForceSensitiveQuests()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_force_sensitive_quests_ = current_force_sensitive_quests_ | quest_mask;
    PlayerMessageBuilder::BuildForceSensitiveQuestDelta(this);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_force_sensitive_quests_ = current_force_sensitive_quests_ & ~quest_mask;
    PlayerMessageBuilder::BuildForceSensitiveQuestDelta(this);
}

void Player::ClearCurrentForceSensitiveQuests()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_force_sensitive_quests_ = 0;
    PlayerMessageBuilder::BuildForceSensitiveQuestDelta(this);
}

uint32_t Player::GetCompletedForceSensitiveQuests()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask)
{    
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ | quest_mask;
    PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(this);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ & ~quest_mask;
    PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(this);
}

void Player::ClearCompletedForceSensitiveQuests()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    completed_force_sensitive_quests_ = 0;
    PlayerMessageBuilder::BuildCompletedForceSensitiveQuestDelta(this);
}

swganh::messages::containers::NetworkMap<uint32_t, QuestJournalData> Player::GetQuests() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return quest_journal_;
}

void Player::AddQuest(QuestJournalData quest)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    quest_journal_.Add(quest.quest_crc, quest);
    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

void Player::RemoveQuest(QuestJournalData quest)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    
    auto find_iter = find_if(
        quest_journal_.Begin(),
        quest_journal_.End(),
        [&quest] ( pair<uint32_t, QuestJournalData> stored_quest)
    {
        return quest.quest_crc == stored_quest.first;
    });

    if (find_iter == quest_journal_.End())
    {
        return;
    }

    quest_journal_.Remove(find_iter);
    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

void Player::UpdateQuest(QuestJournalData quest)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    quest_journal_.Update(quest.quest_crc, quest);
    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

void Player::ClearAllQuests()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    quest_journal_.Clear();
    PlayerMessageBuilder::BuildQuestJournalDelta(this);
}

swganh::messages::containers::NetworkSortedList<Ability> Player::GetAbilityList() 
{
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
    
    auto find_it = find_if(begin(abilities), end(abilities),[=, &abilities](pair<uint32_t, string> skill_command){
        return (ability == skill_command.second);
       });
    if (find_it != end(abilities))
        return true;
    return false;
}

uint32_t Player::GetExperimentationFlag() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experimentation_flag_ = experimentation_flag;
    PlayerMessageBuilder::BuildExperimentationFlagDelta(this);
}

uint32_t Player::GetCraftingStage() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    crafting_stage_ = crafting_stage;
    PlayerMessageBuilder::BuildCraftingStageDelta(this);
}

uint64_t Player::GetNearestCraftingStation() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    nearest_crafting_station_ = crafting_station_id;
    PlayerMessageBuilder::BuildNearestCraftingStationDelta(this);
}

swganh::messages::containers::NetworkSortedList<DraftSchematicData> Player::GetDraftSchematics() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return draft_schematics_;
}

void Player::AddDraftSchematic(DraftSchematicData schematic)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    draft_schematics_.Add(schematic);
    PlayerMessageBuilder::BuildDraftSchematicDelta(this);
}

void Player::RemoveDraftSchematic(uint32_t schematic_id)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto iter = draft_schematics_.Find(DraftSchematicData(schematic_id));
    if(iter != draft_schematics_.End())
    {
        draft_schematics_.Remove(iter);
        PlayerMessageBuilder::BuildDraftSchematicDelta(this);
    }
}

void Player::ClearDraftSchematics()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    draft_schematics_.Clear();
    PlayerMessageBuilder::BuildDraftSchematicDelta(this);
}

uint32_t Player::GetExperimentationPoints() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experimentation_points_ += points;
    PlayerMessageBuilder::BuildExperimentationPointsDelta(this);
}

void Player::RemoveExperimentationPoints(uint32_t points)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    experimentation_points_ -= points;
    PlayerMessageBuilder::BuildExperimentationPointsDelta(this);
}

void Player::ResetExperimentationPoints(uint32_t points)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
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
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return friends_;
}
bool Player::IsFriend(std::string friend_name)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto iter = find_if(friends_.Begin(), friends_.End(), [=](const Name& x)->bool {
        return (x.name == friend_name);
    });
    
    if (iter != friends_.End())
        return true;

    return false;
}
void Player::AddFriend(string  friend_name)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    friends_.Add(friend_name);
    PlayerMessageBuilder::BuildFriendsDelta(this);
}

void Player::RemoveFriend(string friend_name)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto iter = find_if(friends_.Begin(), friends_.End(), [=](const Name& x)->bool {
        return (x.name == friend_name);
    });
    
    if (iter != friends_.End())
    {
        friends_.Remove(iter);
        PlayerMessageBuilder::BuildFriendsDelta(this);    
    }
}

void Player::ClearFriends()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    friends_.Clear();
    PlayerMessageBuilder::BuildFriendsDelta(this);
}

NetworkSortedVector<Name> Player::GetIgnoredPlayers()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return ignored_players_;
}

void Player::IgnorePlayer(string player_name)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    ignored_players_.Add(player_name);
    PlayerMessageBuilder::BuildIgnoredDelta(this);    
}

void Player::StopIgnoringPlayer(string player_name)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    auto iter = find_if(ignored_players_.Begin(), ignored_players_.End(), [=](const Name& x)->bool {
        return (x.name == player_name);
    });

    if (iter != ignored_players_.End())
    {
        friends_.Remove(iter);
        PlayerMessageBuilder::BuildIgnoredDelta(this);    
    }
}

void Player::ClearIgnored()
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    ignored_players_.Clear();
    PlayerMessageBuilder::BuildIgnoredDelta(this);
}

uint32_t Player::GetLanguage() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return language_;
}

void Player::SetLanguage(uint32_t language_id)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    language_ = language_id;
    PlayerMessageBuilder::BuildLanguageDelta(this);
}

uint32_t Player::GetCurrentStomach() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;
    PlayerMessageBuilder::BuildCurrentStomachDelta(this);
}

void Player::DecreaseCurrentStomach(uint32_t stomach)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_stomach_ -= stomach;
    PlayerMessageBuilder::BuildCurrentStomachDelta(this);
}

void Player::ResetCurrentStomach(uint32_t stomach)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_stomach_ = stomach;
    PlayerMessageBuilder::BuildCurrentStomachDelta(this);
}

uint32_t Player::GetMaxStomach() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    max_stomach_ = stomach;
    PlayerMessageBuilder::BuildMaxStomachDelta(this);
}

uint32_t Player::GetCurrentDrink() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
    PlayerMessageBuilder::BuildCurrentDrinkDelta(this);
}

void Player::DecreaseCurrentDrink(uint32_t drink)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_drink_ -= drink;
    PlayerMessageBuilder::BuildCurrentDrinkDelta(this);
}

void Player::ResetCurrentDrink(uint32_t drink)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    current_drink_ = drink;
    PlayerMessageBuilder::BuildCurrentDrinkDelta(this);
}

uint32_t Player::GetMaxDrink() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    max_drink_ = drink;
    PlayerMessageBuilder::BuildMaxDrinkDelta(this);
}

uint32_t Player::GetJediState() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    jedi_state_ = jedi_state;
    PlayerMessageBuilder::BuildJediStateDelta(this);
}

Gender Player::GetGender() 
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
    return gender_;
}
void Player::SetGender(Gender value)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);
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

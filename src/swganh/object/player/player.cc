
#include "swganh/object/player/player.h"

#include "anh/crc.h"

#include "swganh/object/player/player_message_builder.h"
#include "swganh/messages/deltas_message.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::object::waypoint;
using namespace swganh::messages;

Player::Player()
{
    // TEMP manually resize these vectors
    abilities_.resize(25);
    experience_.resize(25);
    waypoints_.resize(25);
    draft_schematics_.resize(25);
    friends_.resize(25);
    ignored_players_.resize(25);
    for (int i = 0; i < 26; i++)
    {
        abilities_free_list_.push_back(i);
        experience_free_list_.push_back(i);
        waypoint_free_list_.push_back(i);
        draft_schematics_free_list_.push_back(i);
        friends_free_list_.push_back(i);
        ignored_free_list_.push_back(i);
    }

}

void Player::AddStatusFlag(uint32_t flag)
{
    status_flags_ = status_flags_ | flag;
    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

void Player::RemoveStatusFlag(uint32_t flag)
{
    status_flags_ = status_flags_ & ~flag;
    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}
void Player::ToggleStatusFlag(uint32_t flag)
{
    status_flags_ = status_flags_ ^ flag;
    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}
void Player::ClearStatusFlags()
{
    status_flags_ = 0;
    PlayerMessageBuilder::BuildStatusBitmaskDelta(this);
}

uint32_t Player::GetProfileFlags() const
{
    return profile_flags_;
}

void Player::AddProfileFlag(uint32_t flag)
{
    profile_flags_ = profile_flags_ | flag;
    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

void Player::RemoveProfileFlag(uint32_t flag)
{    
    profile_flags_ = profile_flags_ & ~flag;
    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}
void Player::ToggleProfileFlag(uint32_t flag)
{
    profile_flags_ = profile_flags_ ^ flag;
    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}
void Player::ClearProfileFlags()
{
    profile_flags_ = 0;
    PlayerMessageBuilder::BuildProfileBitmaskDelta(this);
}

std::string Player::GetProfessionTag() const
{
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag)
{
    profession_tag_ = profession_tag;
    PlayerMessageBuilder::BuildProfessionTagDelta(this);
}

uint32_t Player::GetBornDate() const
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date)
{
    born_date_ = born_date;
    PlayerMessageBuilder::BuildBornDateDelta(this);
}

uint32_t Player::GetTotalPlayTime() const
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

uint32_t Player::GetRegionId() const
{
    return region_;
}
void Player::SetRegionId(uint32_t region)
{
    region_ = region;
    PlayerMessageBuilder::BuildRegionIdDelta(this);
}

uint8_t Player::GetAdminTag() const
{
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag)
{
    admin_tag_ = tag;
    PlayerMessageBuilder::BuildAdminTagDelta(this);
}

std::vector<Player::XpData> Player::GetXp() const
{
    return experience_;
}

void Player::AddExperience(std::string type, uint32_t amount)
{
    auto find_iter = find_if(
        experience_.begin(),
        experience_.end(),
        [&type] (Player::XpData& xp)
    {
        return type.compare(xp.type) == 0;
    });
    int total_value = amount;
    if (find_iter == experience_.end())
    {
        Player::XpData xp;
        xp.type = type;
        xp.value = amount;

        experience_.push_back(move(xp));
    }
    else
    {
        find_iter->value += amount;
        total_value = find_iter->value;
    }
    // send total amount of xp for that type
    PlayerMessageBuilder::BuildXpDelta(this, 0, type, total_value);
}

void Player::DeductXp(std::string type, uint32_t amount)
{
    auto find_iter = find_if(
        experience_.begin(),
        experience_.end(),
        [&type] (Player::XpData& xp)
    {
        return type.compare(xp.type) == 0;
    });

    if (find_iter == experience_.end())
    {
        return;
    }
        
    find_iter->value -= amount;
    // send total amount of xp for that type
    PlayerMessageBuilder::BuildXpDelta(this, 1, find_iter->type, find_iter->value);
}

void Player::ClearXpType(std::string type)
{
    vector<Player::XpData>::iterator find_iter = find_if(
        experience_.begin(),
        experience_.end(),
        [&type] (Player::XpData& xp)
    {
        return type.compare(xp.type) == 0;
    });

    if (find_iter == experience_.end())
    {
        return;
    }
    // now erase
    experience_.erase(find_iter);
    PlayerMessageBuilder::BuildXpDelta(this, 2, type, 0);
}

void Player::ResetXp(std::vector<Player::XpData> experience)
{
    experience_ = experience;
    PlayerMessageBuilder::BuildResetXpDelta(this, experience);
}

void Player::ClearAllXp()
{
    experience_.clear();
    // TODO: Clear
    PlayerMessageBuilder::BuildXpDelta(this, 3, 0, 0);
}

vector<shared_ptr<Waypoint>> Player::GetWaypoints() const
{
    return waypoints_;
}

void Player::AddWaypoint(shared_ptr<Waypoint> waypoint)
{
    auto find_iter = find_if(
        waypoints_.begin(),
        waypoints_.end(),
        [&waypoint] (shared_ptr<Waypoint> stored_waypoint) -> bool
    {
        if (stored_waypoint)
            return waypoint->GetObjectId() == stored_waypoint->GetObjectId();
        else
            return false;
    });

    if (find_iter != waypoints_.end())
    {
        return;
    }

    waypoints_.push_back(move(waypoint));
    PlayerMessageBuilder::BuildWaypointDelta(this, 0, waypoint);
}

void Player::RemoveWaypoint(uint64_t waypoint_id)
{
    auto find_iter = find_if(
        waypoints_.begin(),
        waypoints_.end(),
        [waypoint_id] (shared_ptr<Waypoint> stored_waypoint)
    {
        return waypoint_id == stored_waypoint->GetObjectId();
    });

    if (find_iter == waypoints_.end())
    {
        return;
    }
    PlayerMessageBuilder::BuildWaypointDelta(this, 1, *find_iter);
    waypoints_.erase(find_iter);
}

void Player::ModifyWaypoint(shared_ptr<Waypoint> waypoint)
{
    auto find_iter = find_if(
        waypoints_.begin(),
        waypoints_.end(),
        [&waypoint] (shared_ptr<Waypoint> stored_waypoint)
    {
        return waypoint->GetObjectId() == stored_waypoint->GetObjectId();
    });

    if (find_iter == waypoints_.end())
    {
        return;
    }

    *find_iter = move(waypoint);
    PlayerMessageBuilder::BuildWaypointDelta(this, 1, waypoint);
}

void Player::ResetWaypoints(vector<shared_ptr<Waypoint>> waypoints)
{
    waypoints_ = move(waypoints);
    PlayerMessageBuilder::BuildResetWaypointDelta(this, waypoints);
}

void Player::ClearAllWaypoints()
{
    waypoints_.clear();
    // do the delta update
    //if (HasObservers())
    //{
    //    DeltasMessage message = CreateDeltasMessage(VIEW_8);
    //    // update count
    //    message.data.write<uint16_t>(1);
    //    // update type
    //    message.data.write<uint16_t>(1);
    //    // sub type
    //    // TODO: find out if 4 really does reset all
    //    message.data.write<uint16_t>(4);
    //    message.data.write(0);
    //    message.data.write(0);
    //    
    //    AddDeltasUpdate(move(message));
    //}
}

uint32_t Player::GetCurrentForcePower() const
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(uint32_t force_power)
{
    current_force_power_ = force_power;
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

void Player::IncreaseForcePower(uint32_t force_power)
{
    uint32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

void Player::DecreaseForcePower(uint32_t force_power)
{
    current_force_power_ -= force_power;
    PlayerMessageBuilder::BuildCurrentForcePowerDelta(this);
}

uint32_t Player::GetMaxForcePower() const
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

vector<Player::QuestJournalData> Player::GetQuests() const
{
    return quest_journal_;
}

void Player::AddQuest(QuestJournalData quest)
{
    auto find_iter = find_if(
        quest_journal_.begin(),
        quest_journal_.end(),
        [&quest] (const Player::QuestJournalData& stored_quest)
    {
        return quest.quest_crc == stored_quest.quest_crc;
    });

    if (find_iter != quest_journal_.end())
    {
        return;
    }

    quest_journal_.push_back(move(quest));
    PlayerMessageBuilder::BuildQuestJournalDelta(this, 0, quest);
}

void Player::RemoveQuest(Player::QuestJournalData quest)
{
    auto find_iter = find_if(
        quest_journal_.begin(),
        quest_journal_.end(),
        [&quest] (const Player::QuestJournalData& stored_quest)
    {
        return quest.quest_crc == stored_quest.quest_crc;
    });

    if (find_iter == quest_journal_.end())
    {
        return;
    }

    quest_journal_.erase(find_iter);

    PlayerMessageBuilder::BuildQuestJournalDelta(this, 1, quest);
}

void Player::UpdateQuest(Player::QuestJournalData quest)
{
    auto find_iter = find_if(
        quest_journal_.begin(),
        quest_journal_.end(),
        [&quest] (const Player::QuestJournalData& stored_quest)
    {
        return quest.quest_crc == stored_quest.quest_crc;
    });

    if (find_iter == quest_journal_.end())
    {
        return;
    }

    *find_iter = move(quest);

    PlayerMessageBuilder::BuildQuestJournalDelta(this, 2, quest);
}

void Player::ResetQuests(vector<Player::QuestJournalData> quests)
{
    quest_journal_ = move(quests);

    // TODO: Quest Delta Update
}

void Player::ClearAllQuests()
{
    quest_journal_.clear();

    // TODO: Quest Delta Update
}

vector<string> Player::GetAbilityList() const
{
    return abilities_;
}
std::vector<std::string>::iterator Player::GetAbilityIter_(std::string ability)
{
    auto find_iter = find_if(
        abilities_.begin(),
        abilities_.end(),
        [&ability] (const string& stored_ability)
    {
        return ability.compare(stored_ability) == 0;
    });
    return find_iter;
}
void Player::AddAbility(string ability)
{
    if (GetAbilityIter_(ability) != abilities_.end())
    {
        return;
    }

    uint16_t next_index = GetNextAvailableSlot(abilities_, abilities_free_list_);
    abilities_[next_index] = move(ability);
    PlayerMessageBuilder::BuildAbilityDelta(this, 1, next_index, ability);
}

void Player::RemoveAbility(string ability)
{
    auto iter = GetAbilityIter_(ability);
    if (iter == abilities_.end())
    {
        return;
    }
    uint16_t index_position = iter - begin(abilities_);

    abilities_.erase(iter);

    PlayerMessageBuilder::BuildAbilityDelta(this, 0, index_position, ability);
    abilities_free_list_.push_back(index_position);
}

void Player::ClearAllAbilities()
{
    abilities_.clear();
    PlayerMessageBuilder::BuildAbilityDelta(this, 4, 0, "");
    abilities_free_list_.clear();
}

void Player::ResetAbilities(vector<string> abilities)
{
    abilities_ = move(abilities);
    PlayerMessageBuilder::BuildResetAbilityDelta(this, abilities);
    abilities_free_list_.clear();
}

uint32_t Player::GetExperimentationFlag() const
{
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag)
{
    experimentation_flag_ = experimentation_flag;
    PlayerMessageBuilder::BuildExperimentationFlagDelta(this);
}

uint32_t Player::GetCraftingStage() const
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage)
{
    crafting_stage_ = crafting_stage;
    PlayerMessageBuilder::BuildCraftingStageDelta(this);
}

uint64_t Player::GetNearestCraftingStation() const
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id)
{
    nearest_crafting_station_ = crafting_station_id;
    PlayerMessageBuilder::BuildNearestCraftingStationDelta(this);
}

vector<Player::DraftSchematicData> Player::GetDraftSchematics() const
{
    return draft_schematics_;
}
vector<Player::DraftSchematicData>::iterator Player::GetSchematicIter_(uint32_t schematic_crc)
{
    auto find_iter = find_if(
        draft_schematics_.begin(),
        draft_schematics_.end(),
        [&schematic_crc] (const DraftSchematicData& stored_schematic)
    {
        return stored_schematic.schematic_crc == schematic_crc;
    });
    return find_iter;
}

void Player::AddDraftSchematic(DraftSchematicData schematic)
{
    if (GetSchematicIter_(schematic.schematic_crc) != draft_schematics_.end())
    {
        return;
    }
    uint16_t next_index = GetNextAvailableSlot(draft_schematics_, draft_schematics_free_list_);
    draft_schematics_[next_index] = move(schematic);
    PlayerMessageBuilder::BuildDraftSchematicDelta(this, 1, next_index, schematic);
}

void Player::RemoveDraftSchematic(uint32_t schematic_crc)
{
    auto iter = GetSchematicIter_(schematic_crc);
    if ( iter == draft_schematics_.end())
    {
        return;
    }
    uint16_t index_position = iter - begin(draft_schematics_);
    PlayerMessageBuilder::BuildDraftSchematicDelta(this, 0, index_position, *iter);

    draft_schematics_.erase(iter);
    draft_schematics_free_list_.push_back(index_position);
}

void Player::ClearDraftSchematics()
{
    draft_schematics_.clear();
    PlayerMessageBuilder::BuildDraftSchematicDelta(this, 4, 0, DraftSchematicData());
    draft_schematics_free_list_.clear();
}

void Player::ResetDraftSchematics(std::vector<DraftSchematicData> draft_schematics)
{
    draft_schematics_ = move(draft_schematics);
    PlayerMessageBuilder::BuildResetDraftSchematicDelta(this, draft_schematics);
    draft_schematics_free_list_.clear();
}

uint32_t Player::GetExperimentationPoints() const
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

uint32_t Player::GetAccomplishmentCounter() const
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

vector<string> Player::GetFriends()
{
    return friends_;
}
vector<string>::iterator Player::GetFriendsIter_(string friend_name)
{
    auto find_iter = find_if(
        friends_.begin(),
        friends_.end(),
        [&friend_name] (const string& stored_friend)
    {
        return stored_friend.compare(friend_name) == 0;
    });
    return find_iter;
}
void Player::AddFriend(string friend_name)
{
    if (GetFriendsIter_(friend_name) != friends_.end())
    {
        return;
    }

    friends_.push_back(move(friend_name));
    uint16_t next_index = GetNextAvailableSlot(friends_, friends_free_list_);
    PlayerMessageBuilder::BuildFriendsDelta(this, 1, next_index, friend_name);
}

void Player::RemoveFriend(string friend_name)
{
    auto iter = GetFriendsIter_(friend_name);
    if (iter == friends_.end())
    {
        return;
    }
    friends_.erase(iter);
    uint16_t index = iter - begin(friends_);
    PlayerMessageBuilder::BuildFriendsDelta(this, 0, index, friend_name);
    friends_free_list_.push_back(index);
}

void Player::ClearFriends()
{
    friends_.clear();
    PlayerMessageBuilder::BuildFriendsDelta(this, 4, 0, "");
    friends_free_list_.clear();
}

vector<string> Player::GetIgnoredPlayers()
{
    return ignored_players_;
}
vector<string>::iterator Player::GetIgnoredIter_(string ignored_name)
{
    auto find_iter = find_if(
        ignored_players_.begin(),
        ignored_players_.end(),
        [&ignored_name] (const string& stored_ignored_player)
    {
        return stored_ignored_player.compare(ignored_name) == 0;
    });
    return find_iter;
}

void Player::IgnorePlayer(string player_name)
{
    if (GetIgnoredIter_(player_name) == ignored_players_.end())
    {
        return;
    }
    ignored_players_.push_back(player_name);
    uint16_t next_index = GetNextAvailableSlot(ignored_players_, ignored_free_list_);
    PlayerMessageBuilder::BuildIgnoredDelta(this, 1, next_index, player_name);
}

void Player::StopIgnoringPlayer(string player_name)
{
    auto iter = GetIgnoredIter_(player_name);
    if (iter == ignored_players_.end())
    {
        return;
    }
    uint16_t index = iter - begin(ignored_players_);
    PlayerMessageBuilder::BuildIgnoredDelta(this, 0, index, player_name);
    
    ignored_players_.erase(iter);
    ignored_free_list_.push_back(index);
}

void Player::ClearIgnored()
{
    ignored_players_.clear();
    PlayerMessageBuilder::BuildIgnoredDelta(this, 4, 0, "");
}

uint32_t Player::GetLanguage() const
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id)
{
    language_ = language_id;
    PlayerMessageBuilder::BuildLanguageDelta(this);
}

uint32_t Player::GetCurrentStomach() const
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

uint32_t Player::GetMaxStomach() const
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach)
{
    max_stomach_ = stomach;
    PlayerMessageBuilder::BuildMaxStomachDelta(this);
}

uint32_t Player::GetCurrentDrink() const
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

uint32_t Player::GetMaxDrink() const
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink)
{
    max_drink_ = drink;
    PlayerMessageBuilder::BuildMaxDrinkDelta(this);
}

uint32_t Player::GetJediState() const
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state)
{
    jedi_state_ = jedi_state;
    PlayerMessageBuilder::BuildJediStateDelta(this);
}

Player::Gender Player::GetGender() const
{
    return gender_;
}
void Player::SetGender(Player::Gender value)
{
    gender_ = value;
}
boost::optional<BaselinesMessage> Player::GetBaseline3()
{
    return move(PlayerMessageBuilder::BuildBaseline3(this));
}
boost::optional<BaselinesMessage> Player::GetBaseline6()
{
    return move(PlayerMessageBuilder::BuildBaseline6(this));
}
boost::optional<BaselinesMessage> Player::GetBaseline8()
{
    return move(PlayerMessageBuilder::BuildBaseline8(this));
}
boost::optional<BaselinesMessage> Player::GetBaseline9()
{
    return move(PlayerMessageBuilder::BuildBaseline9(this));
}

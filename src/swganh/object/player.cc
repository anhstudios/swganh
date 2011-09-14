
#include "swganh/object/player.h"

using namespace std;
using namespace swganh::object;

vector<uint32_t> Player::GetStatusFlags() const
{
    return status_flags_;
}

void Player::AddStatusFlag(uint32_t flag)
{
    auto find_iter = find_if(
        status_flags_.begin(),
        status_flags_.end(),
        [flag] (uint32_t status_flag)
    {
        return flag == status_flag;
    });

    if (find_iter != status_flags_.end())
    {
        // Already in the list.
        return;
    }

    status_flags_.push_back(flag);
}

void Player::RemoveStatusFlag(uint32_t flag)
{
    auto find_iter = find_if(
        status_flags_.begin(),
        status_flags_.end(),
        [flag] (uint32_t status_flag)
    {
        return flag == status_flag;
    });

    if (find_iter == status_flags_.end())
    {
        // Not in the list.
        return;
    }

    status_flags_.erase(find_iter);
}

void Player::ClearStatusFlags()
{
    status_flags_.clear();
}

vector<uint32_t> Player::GetProfileFlags() const
{
    return profile_flags_;
}

void Player::AddProfileFlag(uint32_t flag)
{
    auto find_iter = find_if(
        profile_flags_.begin(),
        profile_flags_.end(),
        [flag] (uint32_t profile_flag)
    {
        return flag == profile_flag;
    });

    if (find_iter != profile_flags_.end())
    {
        // Already in the list.
        return;
    }

    profile_flags_.push_back(flag);
}

void Player::RemoveProfileFlag(uint32_t flag)
{    
    auto find_iter = find_if(
        profile_flags_.begin(),
        profile_flags_.end(),
        [flag] (uint32_t profile_flag)
    {
        return flag == profile_flag;
    });

    if (find_iter == profile_flags_.end())
    {
        // Not in the list.
        return;
    }

    profile_flags_.erase(find_iter);
}

void Player::ClearProfileFlags()
{
    profile_flags_.clear();
}

std::string Player::GetProfessionTag() const
{
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag)
{
    profession_tag_ = profession_tag;
}

uint32_t Player::GetBornDate() const
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date)
{
    born_date_ = born_date;
}

uint32_t Player::GetTotalPlayTime() const
{
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time)
{
    total_playtime_ = play_time;
}

void Player::IncrementTotalPlayTime(uint32_t increment)
{
    total_playtime_ += increment;
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
    }
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

    experience_.erase(find_iter);
}

void Player::ResetXp(std::vector<Player::XpData> experience)
{
    experience_ = experience;
}

void Player::ClearAllXp()
{
    experience_.clear();
}

std::vector<Waypoint> Player::GetWaypoints() const
{
    return waypoints_;
}

void Player::AddWaypoint(Waypoint waypoint)
{
    auto find_iter = find_if(
        waypoints_.begin(),
        waypoints_.end(),
        [&waypoint] (const Waypoint& stored_waypoint)
    {
        return waypoint.GetObjectId() == stored_waypoint.GetObjectId();
    });

    if (find_iter != waypoints_.end())
    {
        return;
    }

    waypoints_.push_back(move(waypoint));
}

void Player::RemoveWaypoint(uint64_t waypoint_id)
{
    auto find_iter = find_if(
        waypoints_.begin(),
        waypoints_.end(),
        [waypoint_id] (const Waypoint& stored_waypoint)
    {
        return waypoint_id == stored_waypoint.GetObjectId();
    });

    if (find_iter == waypoints_.end())
    {
        return;
    }

    waypoints_.erase(find_iter);
}

void Player::ModifyWaypoint(Waypoint waypoint)
{
    auto find_iter = find_if(
        waypoints_.begin(),
        waypoints_.end(),
        [&waypoint] (const Waypoint& stored_waypoint)
    {
        return waypoint.GetObjectId() == stored_waypoint.GetObjectId();
    });

    if (find_iter == waypoints_.end())
    {
        return;
    }

    *find_iter = move(waypoint);
}

void Player::ResetWaypoints(std::vector<Waypoint> waypoints)
{
    waypoints_ = move(waypoints);
}

void Player::ClearAllWaypoints()
{
    waypoints_.clear();
}

uint32_t Player::GetCurrentForcePower() const
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(uint32_t force_power)
{
    current_force_power_ = force_power;
}

void Player::IncreaseForcePower(uint32_t force_power)
{
    uint32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
}

void Player::DecreaseForcePower(uint32_t force_power)
{
    current_force_power_ -= force_power;
}

uint32_t Player::GetMaxForcePower() const
{
    return max_force_power_;
}

void Player::SetMaxForcePower(uint32_t force_power)
{
    max_force_power_ = force_power;
}

vector<uint8_t> Player::GetCurrentForceSensitiveQuests()
{
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint8_t quest_mask)
{
    auto find_iter = find_if(
        current_force_sensitive_quests_.begin(),
        current_force_sensitive_quests_.end(),
        [quest_mask] (uint8_t stored_quest_mask)
    {
        return quest_mask == stored_quest_mask;
    });

    if (find_iter != current_force_sensitive_quests_.end())
    {
        return;
    }

    current_force_sensitive_quests_.push_back(quest_mask);
}

void Player::RemoveCurrentForceSensitiveQuest(uint8_t quest_mask)
{
    auto find_iter = find_if(
        current_force_sensitive_quests_.begin(),
        current_force_sensitive_quests_.end(),
        [quest_mask] (uint8_t stored_quest_mask)
    {
        return quest_mask == stored_quest_mask;
    });

    if (find_iter == current_force_sensitive_quests_.end())
    {
        return;
    }

    current_force_sensitive_quests_.erase(find_iter);
}

void Player::ClearCurrentForceSensitiveQuests()
{
    current_force_sensitive_quests_.clear();
}

vector<uint8_t> Player::GetCompletedForceSensitiveQuests()
{
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint8_t quest_mask)
{    
    auto find_iter = find_if(
        completed_force_sensitive_quests_.begin(),
        completed_force_sensitive_quests_.end(),
        [quest_mask] (uint8_t stored_quest_mask)
    {
        return quest_mask == stored_quest_mask;
    });

    if (find_iter != completed_force_sensitive_quests_.end())
    {
        return;
    }

    completed_force_sensitive_quests_.push_back(quest_mask);
}

void Player::RemoveCompletedForceSensitiveQuest(uint8_t quest_mask)
{
    auto find_iter = find_if(
        completed_force_sensitive_quests_.begin(),
        completed_force_sensitive_quests_.end(),
        [quest_mask] (uint8_t stored_quest_mask)
    {
        return quest_mask == stored_quest_mask;
    });

    if (find_iter == completed_force_sensitive_quests_.end())
    {
        return;
    }

    completed_force_sensitive_quests_.erase(find_iter);
}

void Player::ClearCompletedForceSensitiveQuests()
{
    completed_force_sensitive_quests_.clear();
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
}

void Player::ResetQuests(vector<Player::QuestJournalData> quests)
{
    quest_journal_ = move(quests);
}

void Player::ClearAllQuests()
{
    quest_journal_.clear();
}

vector<string> Player::GetAbilityList() const
{
    return abilities_;
}

void Player::AddAbility(string ability)
{
    auto find_iter = find_if(
        abilities_.begin(),
        abilities_.end(),
        [&ability] (const string& stored_ability)
    {
        return ability.compare(stored_ability) == 0;
    });

    if (find_iter != abilities_.end())
    {
        return;
    }

    abilities_.push_back(move(ability));
}

void Player::RemoveAbility(string ability)
{
    auto find_iter = find_if(
        abilities_.begin(),
        abilities_.end(),
        [&ability] (const string& stored_ability)
    {
        return ability.compare(stored_ability) == 0;
    });

    if (find_iter == abilities_.end())
    {
        return;
    }

    abilities_.erase(find_iter);
}

void Player::ClearAllAbilities()
{
    abilities_.clear();
}

void Player::ResetAbilities(vector<string> abilities)
{
    abilities_ = move(abilities);
}

uint32_t Player::GetExperimentationFlag() const
{
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag)
{
    experimentation_flag_ = experimentation_flag;
}

uint32_t Player::GetCraftingStage() const
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage)
{
    crafting_stage_ = crafting_stage;
}

uint64_t Player::GetNearestCraftingStation() const
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id)
{
    nearest_crafting_station_ = crafting_station_id;
}

vector<Player::DraftSchematicData> Player::GetDraftSchematics() const
{
    return draft_schematics_;
}

void Player::AddDraftSchematic(DraftSchematicData schematic)
{
    auto find_iter = find_if(
        draft_schematics_.begin(),
        draft_schematics_.end(),
        [&schematic] (const DraftSchematicData& stored_schematic)
    {
        return stored_schematic.schematic_crc == schematic.schematic_crc;
    });

    if (find_iter != draft_schematics_.end())
    {
        return;
    }

    draft_schematics_.push_back(move(schematic));
}

void Player::RemoveDraftSchematic(uint32_t schematic_crc)
{
    auto find_iter = find_if(
        draft_schematics_.begin(),
        draft_schematics_.end(),
        [&schematic_crc] (const DraftSchematicData& stored_schematic)
    {
        return stored_schematic.schematic_crc == schematic_crc;
    });

    if (find_iter == draft_schematics_.end())
    {
        return;
    }

    draft_schematics_.erase(find_iter);
}

void Player::ClearDraftSchematics()
{
    draft_schematics_.clear();
}

void Player::ResetDraftSchematics(std::vector<DraftSchematicData> draft_schematics)
{
    draft_schematics_ = move(draft_schematics);
}

uint32_t Player::GetExperimentationPoints() const
{
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points)
{
    experimentation_points_ += points;
}

void Player::RemoveExperimentationPoints(uint32_t points)
{
    experimentation_points_ -= points;
}

void Player::ResetExperimentationPoints(uint32_t points)
{
    experimentation_points_ = points;
}

uint32_t Player::GetAccomplishmentCounter() const
{
    return accomplishment_counter_;
}

void Player::ResetAccomplishmentCounter(uint32_t counter)
{
    accomplishment_counter_ = counter;
}

void Player::IncrementAccomplishmentCounter()
{
    ++accomplishment_counter_;
}

vector<string> Player::GetFriends()
{
    return friends_;
}

void Player::AddFriend(string friend_name)
{
    auto find_iter = find_if(
        friends_.begin(),
        friends_.end(),
        [&friend_name] (const string& stored_friend)
    {
        return stored_friend.compare(friend_name) == 0;
    });

    if (find_iter != friends_.end())
    {
        return;
    }

    friends_.push_back(move(friend_name));
}

void Player::RemoveFriend(string friend_name)
{
    auto find_iter = find_if(
        friends_.begin(),
        friends_.end(),
        [&friend_name] (const string& stored_friend)
    {
        return stored_friend.compare(friend_name) == 0;
    });

    if (find_iter == friends_.end())
    {
        return;
    }

    friends_.erase(find_iter);
}

void Player::ClearFriends()
{
    friends_.clear();
}

vector<string> Player::GetIgnoredPlayers()
{
    return ignored_players_;
}

void Player::IgnorePlayer(string player_name)
{
    auto find_iter = find_if(
        ignored_players_.begin(),
        ignored_players_.end(),
        [&player_name] (const string& stored_ignored_player)
    {
        return stored_ignored_player.compare(player_name) == 0;
    });

    if (find_iter == ignored_players_.end())
    {
        return;
    }

    ignored_players_.push_back(player_name);
}

void Player::StopIgnoringPlayer(string player_name)
{
    auto find_iter = find_if(
        ignored_players_.begin(),
        ignored_players_.end(),
        [&player_name] (const string& stored_ignored_player)
    {
        return stored_ignored_player.compare(player_name) == 0;
    });

    if (find_iter == ignored_players_.end())
    {
        return;
    }

    ignored_players_.erase(find_iter);
}

void Player::ClearIgnored()
{
    ignored_players_.clear();
}

uint32_t Player::GetLanguage() const
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id)
{
    language_ = language_id;
}

uint32_t Player::GetCurrentStomach() const
{
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach)
{
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;
}

void Player::DecreaseCurrentStomach(uint32_t stomach)
{
    current_stomach_ -= stomach;
}

void Player::ResetCurrentStomach(uint32_t stomach)
{
    current_stomach_ = stomach;
}

uint32_t Player::GetMaxStomach() const
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach)
{
    max_stomach_ = stomach;
}

uint32_t Player::GetCurrentDrink() const
{
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink)
{
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
}

void Player::DecreaseCurrentDrink(uint32_t drink)
{
    current_drink_ -= drink;
}

void Player::ResetCurrentDrink(uint32_t drink)
{
    current_drink_ = drink;
}

uint32_t Player::GetMaxDrink() const
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink)
{
    max_drink_ = drink;
}

uint32_t Player::GetJediState() const
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state)
{
    jedi_state_ = jedi_state;
}

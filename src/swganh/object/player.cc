
#include "swganh/object/player.h"

#include "anh/crc.h"
#include "swganh/scene/messages/deltas_message.h"
#include "swganh/scene/scene.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::scene;
using namespace messages;

void Player::AddStatusFlag(uint32_t flag)
{
    status_flags_ = status_flags_ | flag;
    SetDeltaBitmask_(status_flags_, 5, VIEW_3);
}

void Player::RemoveStatusFlag(uint32_t flag)
{
    status_flags_ = status_flags_ & ~flag;
    SetDeltaBitmask_(status_flags_, 5, VIEW_3);
}
void Player::ToggleStatusFlag(uint32_t flag)
{
    status_flags_ = status_flags_ ^ flag;
    SetDeltaBitmask_(status_flags_, 5, VIEW_3);
}
void Player::ClearStatusFlags()
{
    status_flags_ = 0;
    SetDeltaBitmask_(status_flags_, 5, VIEW_3);
}

uint32_t Player::GetProfileFlags() const
{
    return profile_flags_;
}

void Player::AddProfileFlag(uint32_t flag)
{
    profile_flags_ = profile_flags_ | flag;
    SetDeltaBitmask_(status_flags_, 6, VIEW_3);
}

void Player::RemoveProfileFlag(uint32_t flag)
{    
    profile_flags_ = profile_flags_ & ~flag;
    SetDeltaBitmask_(status_flags_, 6, VIEW_3);
}
void Player::ToggleProfileFlag(uint32_t flag)
{
    profile_flags_ = profile_flags_ ^ flag;
    SetDeltaBitmask_(profile_flags_, 6, VIEW_3);
}
void Player::ClearProfileFlags()
{
    profile_flags_ = 0;
    SetDeltaBitmask_(profile_flags_, 6, VIEW_3);
}

std::string Player::GetProfessionTag() const
{
    return profession_tag_;
}

void Player::SetProfessionTag(string profession_tag)
{
    profession_tag_ = profession_tag;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        message.data.write(profession_tag_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_3, std::move(message)));
    }
}

uint32_t Player::GetBornDate() const
{
    return born_date_;
}

void Player::SetBornDate(uint32_t born_date)
{
    born_date_ = born_date;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        message.data.write(born_date_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_3, std::move(message)));
    }
}

uint32_t Player::GetTotalPlayTime() const
{
    return total_playtime_;
}

void Player::SetTotalPlayTime(uint32_t play_time)
{
    total_playtime_ = play_time;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(9);
        message.data.write(total_playtime_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_3, std::move(message)));
    }
}

void Player::IncrementTotalPlayTime(uint32_t increment)
{
    total_playtime_ += increment;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(9);
        message.data.write(total_playtime_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_3, std::move(message)));
    }
}

uint32_t Player::GetRegionId() const
{
    return region_;
}
void Player::SetRegionId(uint32_t region)
{
    region_ = region;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        message.data.write(region_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_6, std::move(message)));
    }
}

uint8_t Player::GetAdminTag() const
{
    return admin_tag_;
}

void Player::SetAdminTag(uint8_t tag)
{
    admin_tag_ = tag;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_6);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        message.data.write<uint8_t>(admin_tag_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_6, std::move(message)));
    }
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
    experience_counter_++;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        // sub type
        message.data.write<uint16_t>(0);
        message.data.write(experience_.size());
        message.data.write(experience_counter_);
        message.data.write(type);
        message.data.write(amount);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
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
    experience_counter_++;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        // sub type
        message.data.write<uint16_t>(2);
        message.data.write(experience_.size());
        message.data.write(experience_counter_);
        message.data.write(type);
        message.data.write(amount);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
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
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        message.data.write<uint16_t>(1);
        message.data.write(experience_.size());
        message.data.write(experience_counter_);
        message.data.write(type);
        message.data.write((*find_iter).value);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    // now erase
    experience_.erase(find_iter);
    experience_counter_--;
}

void Player::ResetXp(std::vector<Player::XpData> experience)
{
    experience_ = experience;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(experience_.size());
        // update type
        message.data.write<uint16_t>(0);
        // @TODO: Check if there is a reset
        message.data.write<uint16_t>(2);
        message.data.write(experience_.size());
        message.data.write(experience_.size());
        for_each(begin(experience_), end(experience_),[&message](XpData xp){
            message.data.write(xp.type);
            message.data.write(xp.value);
        });
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    experience_counter_ = experience_.size();
}

void Player::ClearAllXp()
{
    experience_.clear();
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(experience_.size());
        // update type
        message.data.write<uint16_t>(0);
        // @TODO: Check if there is a reset all
        message.data.write<uint16_t>(1);
        message.data.write(experience_.size());
        message.data.write(experience_.size());
        for_each(begin(experience_), end(experience_),[&message](XpData xp){
            message.data.write("");
            message.data.write(0);
        });
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    experience_counter_ = 0;
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
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        // sub type
        message.data.write<uint16_t>(0);
        message.data.write(waypoints_.size());
        message.data.write(waypoint_counter_);
        // waypoint
        message.data.write<uint64_t>(waypoint.GetObjectId());
        // cell id
        message.data.write(0);
        auto position = waypoint.GetPosition();
        message.data.write(position.x);
        message.data.write(position.y);
        message.data.write(position.z);
        message.data.write(waypoint.GetScene()->GetId());
        message.data.write(anh::memcrc(waypoint.GetPlanet()));
        message.data.write(waypoint.GetName());
        message.data.write(waypoint.GetObjectId());
        message.data.write<uint8_t>(waypoint.GetColorByte());
        message.data.write<uint8_t>(waypoint.GetActiveFlag());
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    // update counter
    waypoint_counter_++;
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

    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        // sub type
        message.data.write<uint16_t>(1);
        message.data.write(waypoints_.size());
        message.data.write(waypoint_counter_);
        auto waypoint = *find_iter;
        // waypoint
        message.data.write<uint64_t>(waypoint.GetObjectId());
        // cell id
        message.data.write(0);
        auto position = waypoint.GetPosition();
        message.data.write(position.x);
        message.data.write(position.y);
        message.data.write(position.z);
        message.data.write(waypoint.GetScene()->GetId());
        message.data.write(anh::memcrc(waypoint.GetPlanet()));
        message.data.write(waypoint.GetName());
        message.data.write(waypoint.GetObjectId());
        message.data.write<uint8_t>(waypoint.GetColorByte());
        message.data.write<uint8_t>(waypoint.GetActiveFlag());
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    waypoints_.erase(find_iter);
    // update counter
    waypoint_counter_--;
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
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        // sub type
        message.data.write<uint16_t>(2);
        message.data.write(waypoints_.size());
        message.data.write(waypoint_counter_);
        // waypoint
        message.data.write<uint64_t>(waypoint.GetObjectId());
        // cell id
        message.data.write(0);
        auto position = waypoint.GetPosition();
        message.data.write(position.x);
        message.data.write(position.y);
        message.data.write(position.z);
        message.data.write(waypoint.GetScene()->GetId());
        message.data.write(anh::memcrc(waypoint.GetPlanet()));
        message.data.write(waypoint.GetName());
        message.data.write(waypoint.GetObjectId());
        message.data.write<uint8_t>(waypoint.GetColorByte());
        message.data.write<uint8_t>(waypoint.GetActiveFlag());
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    waypoint_counter_++;
}

void Player::ResetWaypoints(std::vector<Waypoint> waypoints)
{
    waypoints_ = move(waypoints);
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(waypoints_.size());
        // update type
        message.data.write<uint16_t>(1);
        // sub type
        message.data.write<uint16_t>(2);
        message.data.write(waypoints_.size());
        message.data.write(waypoints_.size());
        for_each(begin(waypoints_), end(waypoints_), [&message](Waypoint waypoint){
            // waypoint
            message.data.write<uint64_t>(waypoint.GetObjectId());
            // cell id
            message.data.write(0);
            auto position = waypoint.GetPosition();
            message.data.write(position.x);
            message.data.write(position.y);
            message.data.write(position.z);
            message.data.write(waypoint.GetScene()->GetId());
            message.data.write(anh::memcrc(waypoint.GetPlanet()));
            message.data.write(waypoint.GetName());
            message.data.write(waypoint.GetObjectId());
            message.data.write<uint8_t>(waypoint.GetColorByte());
            message.data.write<uint8_t>(waypoint.GetActiveFlag());
        });
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
    waypoint_counter_ = 0;
}

void Player::ClearAllWaypoints()
{
    waypoints_.clear();
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        // sub type
        // TODO: find out if 4 really does reset all
        message.data.write<uint16_t>(4);
        message.data.write(0);
        message.data.write(0);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
}

uint32_t Player::GetCurrentForcePower() const
{
    return current_force_power_;
}

void Player::SetCurrentForcePower(uint32_t force_power)
{
    current_force_power_ = force_power;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(2);
        message.data.write(current_force_power_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }    
}

void Player::IncreaseForcePower(uint32_t force_power)
{
    uint32_t new_force_power = current_force_power_ + force_power;

    current_force_power_ = (new_force_power > GetMaxForcePower()) ? GetMaxForcePower() : new_force_power;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(2);
        message.data.write(current_force_power_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
}

void Player::DecreaseForcePower(uint32_t force_power)
{
    current_force_power_ -= force_power;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(2);
        message.data.write(current_force_power_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
}

uint32_t Player::GetMaxForcePower() const
{
    return max_force_power_;
}

void Player::SetMaxForcePower(uint32_t force_power)
{
    max_force_power_ = force_power;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(3);
        message.data.write(max_force_power_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
}

uint32_t Player::GetCurrentForceSensitiveQuests()
{
    return current_force_sensitive_quests_;
}

void Player::AddCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ | quest_mask;
    SetDeltaBitmask_(current_force_sensitive_quests_, 4, VIEW_8);
}

void Player::RemoveCurrentForceSensitiveQuest(uint32_t quest_mask)
{
    current_force_sensitive_quests_ = current_force_sensitive_quests_ & ~quest_mask;
    SetDeltaBitmask_(current_force_sensitive_quests_, 4, VIEW_8);
}

void Player::ClearCurrentForceSensitiveQuests()
{
    current_force_sensitive_quests_ = 0;
    SetDeltaBitmask_(current_force_sensitive_quests_, 4, VIEW_8);
}

uint32_t Player::GetCompletedForceSensitiveQuests()
{
    return completed_force_sensitive_quests_;
}

void Player::AddCompletedForceSensitiveQuest(uint32_t quest_mask)
{    
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ | quest_mask;
    SetDeltaBitmask_(completed_force_sensitive_quests_, 5, VIEW_8);
}

void Player::RemoveCompletedForceSensitiveQuest(uint32_t quest_mask)
{
    completed_force_sensitive_quests_ = completed_force_sensitive_quests_ & ~quest_mask;
    SetDeltaBitmask_(completed_force_sensitive_quests_, 5, VIEW_8);
}

void Player::ClearCompletedForceSensitiveQuests()
{
    completed_force_sensitive_quests_ = 0;
    SetDeltaBitmask_(completed_force_sensitive_quests_, 5, VIEW_8);
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

    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_8);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(6);
        // update counter
        message.data.write<uint16_t>(1);
        message.data.write<uint8_t>(0);
        message.data.write(quest.quest_crc);
        message.data.write(quest.owner_id);
        message.data.write(quest.active_step_bitmask);
        message.data.write(quest.completed_step_bitmask);
        message.data.write(quest.completed_flag);
        message.data.write(quest.counter);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_8, std::move(message)));
    }
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

    // TODO: Quest Delta Update
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

    // TODO: Quest Delta Update
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

    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        // sub type
        message.data.write<uint8_t>(1);
        message.data.write(abilities_.size());
        message.data.write(ability);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
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
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        // sub type
        message.data.write<uint8_t>(0);
        // index
        message.data.write(find_iter - abilities_.begin());
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }

    abilities_.erase(find_iter);
}

void Player::ClearAllAbilities()
{
    abilities_.clear();
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(0);
        // sub type (clear)
        message.data.write<uint8_t>(4);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::ResetAbilities(vector<string> abilities)
{
    abilities_ = move(abilities);
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(abilities_.size());
        // update type
        message.data.write<uint16_t>(0);
        // sub type (clear)
        message.data.write<uint8_t>(3);
        message.data.write<uint16_t>(abilities_.size());
        for_each(begin(abilities_), end(abilities_),[&message](string ability){
            message.data.write(ability);
        });
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetExperimentationFlag() const
{
    return experimentation_flag_;
}

void Player::SetExperimentationFlag(uint32_t experimentation_flag)
{
    experimentation_flag_ = experimentation_flag;
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        message.data.write(experimentation_flag_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetCraftingStage() const
{
    return crafting_stage_;
}

void Player::SetCraftingStage(uint32_t crafting_stage)
{
    crafting_stage_ = crafting_stage;
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(2);
        message.data.write(crafting_stage_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint64_t Player::GetNearestCraftingStation() const
{
    return nearest_crafting_station_;
}

void Player::SetNearestCraftingStation(uint64_t crafting_station_id)
{
    nearest_crafting_station_ = crafting_station_id;
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(3);
        message.data.write<uint64_t>(nearest_crafting_station_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
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

     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        // sub type (add)
        message.data.write<uint8_t>(1);
        message.data.write<uint16_t>(draft_schematics_.size());
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
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
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        // sub type (remove)
        message.data.write<uint8_t>(0);
        message.data.write<uint16_t>(find_iter - draft_schematics_.begin());
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }

    draft_schematics_.erase(find_iter);
}

void Player::ClearDraftSchematics()
{
    draft_schematics_.clear();
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        // sub type (clear)
        message.data.write<uint8_t>(4);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::ResetDraftSchematics(std::vector<DraftSchematicData> draft_schematics)
{
    draft_schematics_ = move(draft_schematics);
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        // sub type (reset)
        message.data.write<uint8_t>(3);
        message.data.write<uint16_t>(draft_schematics_.size());
        for_each(begin(draft_schematics_), end(draft_schematics_), [&message](DraftSchematicData schematic){
            schematic.schematic_id;
            schematic.schematic_crc;
        });

        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetExperimentationPoints() const
{
    return experimentation_points_;
}

void Player::AddExperimentationPoints(uint32_t points)
{
    experimentation_points_ += points;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
        message.data.write(experimentation_points_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::RemoveExperimentationPoints(uint32_t points)
{
    experimentation_points_ -= points;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
        message.data.write(experimentation_points_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::ResetExperimentationPoints(uint32_t points)
{
    experimentation_points_ = points;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
        message.data.write(experimentation_points_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetAccomplishmentCounter() const
{
    return accomplishment_counter_;
}

void Player::ResetAccomplishmentCounter(uint32_t counter)
{
    accomplishment_counter_ = counter;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(6);
        message.data.write(accomplishment_counter_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::IncrementAccomplishmentCounter()
{
    ++accomplishment_counter_;
    // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(6);
        message.data.write(accomplishment_counter_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
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

     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        // sub type (add)
        message.data.write<uint8_t>(1);
        message.data.write<uint16_t>(friends_.size());
        message.data.write(friend_name);

        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
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
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        // sub type (remove)
        message.data.write<uint8_t>(0);
        message.data.write<uint16_t>(find_iter - friends_.begin());
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }

    friends_.erase(find_iter);
}

void Player::ClearFriends()
{
    friends_.clear();
     // do the delta update
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        // sub type (reset)
        message.data.write<uint8_t>(4);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
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

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        // sub type (add)
        message.data.write<uint8_t>(1);
        message.data.write(player_name);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }  
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
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        // sub type (remove)
        message.data.write<uint8_t>(0);
        message.data.write<uint16_t>(find_iter - ignored_players_.begin());
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
    ignored_players_.erase(find_iter);
}

void Player::ClearIgnored()
{
    ignored_players_.clear();
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        // sub type (clear)
        message.data.write<uint8_t>(4);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetLanguage() const
{
    return language_;
}

void Player::SetLanguage(uint32_t language_id)
{
    language_ = language_id;
    
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(9);
        message.data.write(language_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetCurrentStomach() const
{
    return current_stomach_;
}

void Player::IncreaseCurrentStomach(uint32_t stomach)
{
    uint32_t new_stomach = current_stomach_ + stomach;

    current_stomach_ = (new_stomach > GetMaxStomach()) ? GetMaxStomach() : new_stomach;

    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(10);
        message.data.write(current_stomach_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::DecreaseCurrentStomach(uint32_t stomach)
{
    current_stomach_ -= stomach;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(10);
        message.data.write(current_stomach_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::ResetCurrentStomach(uint32_t stomach)
{
    current_stomach_ = stomach;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(10);
        message.data.write(current_stomach_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetMaxStomach() const
{
    return max_stomach_;
}

void Player::ResetMaxStomach(uint32_t stomach)
{
    max_stomach_ = stomach;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(11);
        message.data.write(current_stomach_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetCurrentDrink() const
{
    return current_drink_;
}

void Player::IncreaseCurrentDrink(uint32_t drink)
{
    uint32_t new_drink = current_drink_ + drink;

    current_drink_ = (new_drink > GetMaxDrink()) ? GetMaxDrink() : new_drink;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(12);
        message.data.write(current_drink_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::DecreaseCurrentDrink(uint32_t drink)
{
    current_drink_ -= drink;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(12);
        message.data.write(current_drink_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

void Player::ResetCurrentDrink(uint32_t drink)
{
    current_drink_ = drink;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(12);
        message.data.write(current_drink_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetMaxDrink() const
{
    return max_drink_;
}

void Player::ResetMaxDrink(uint32_t drink)
{
    max_drink_ = drink;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(13);
        message.data.write(max_drink_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

uint32_t Player::GetJediState() const
{
    return jedi_state_;
}

void Player::SetJediState(uint32_t jedi_state)
{
    jedi_state_ = jedi_state;
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(VIEW_9);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(17);
        message.data.write(jedi_state_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(VIEW_9, std::move(message)));
    }
}

boost::optional<BaselinesMessage> Player::GetBaseline3()
{
    auto message = CreateBaselinesMessage(BaseObject::VIEW_3, 10);

    message.data.append(BaseObject::GetBaseline3().get().data);
    // not used
    message.data.write(0);
    message.data.write(GetStatusFlags());
    message.data.write(GetProfileFlags());
    message.data.write(GetProfessionTag());
    message.data.write(GetBornDate());
    message.data.write(GetTotalPlayTime());

    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<BaselinesMessage> Player::GetBaseline6()
{
    auto message = CreateBaselinesMessage(BaseObject::VIEW_6, 2);

    message.data.write(GetRegionId());
    message.data.write<uint8_t>(GetAdminTag());

    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<BaselinesMessage> Player::GetBaseline8()
{
    auto message = CreateBaselinesMessage(BaseObject::VIEW_8, 7);
    
    // experience
    message.data.write(experience_.size());
    message.data.write(experience_counter_);
    for_each(begin(experience_), end(experience_), [&message] (XpData xp){
        message.data.write(xp.type);
        message.data.write(xp.value);
    });
    // waypoints
    message.data.write(waypoints_.size());
    message.data.write(waypoint_counter_);
    for_each(begin(waypoints_), end(waypoints_), [&message] (Waypoint waypoint){
        message.data.write<uint8_t>(0);
        message.data.write<uint64_t>(waypoint.GetObjectId());
        // cell id
        message.data.write(0);
        auto position = waypoint.GetPosition();
        message.data.write(position.x);
        message.data.write(position.y);
        message.data.write(position.z);
        message.data.write(waypoint.GetScene()->GetId());
        message.data.write(anh::memcrc(waypoint.GetPlanet()));
        message.data.write(waypoint.GetName());
        message.data.write(waypoint.GetObjectId());
        message.data.write<uint8_t>(waypoint.GetColorByte());
        message.data.write<uint8_t>(waypoint.GetActiveFlag());
    });
    // The Force
    message.data.write(GetCurrentForcePower());
    message.data.write(GetMaxForcePower());
    // TODO: Are these really lists or just bitmasks?
    message.data.write(GetCurrentForceSensitiveQuests());
    message.data.write(GetCompletedForceSensitiveQuests());
    // Quests
    message.data.write(quest_journal_.size());
    message.data.write(0);
    for_each(begin(quest_journal_), end(quest_journal_), [&message](QuestJournalData quest){
        message.data.write<uint8_t>(0);
        message.data.write(quest.quest_crc);
        message.data.write(quest.owner_id);
        message.data.write(quest.active_step_bitmask);
        message.data.write(quest.completed_step_bitmask);
        message.data.write(quest.completed_flag);
        message.data.write(quest.counter);
    });
    return boost::optional<BaselinesMessage>(move(message));
}
boost::optional<BaselinesMessage> Player::GetBaseline9()
{
    auto message = CreateBaselinesMessage(BaseObject::VIEW_9, 17);

    // Ability
    message.data.write(abilities_.size());
    message.data.write(0);
    for_each(begin(abilities_), end(abilities_), [&message](string ability){
        message.data.write(ability);
    });
    // Crafting
    message.data.write(GetExperimentationFlag());
    message.data.write(GetCraftingStage());
    message.data.write(GetNearestCraftingStation());
    // Schematics
    message.data.write(draft_schematics_.size());
    message.data.write(0);
    for_each(begin(draft_schematics_), end(draft_schematics_), [&message](DraftSchematicData schem) {
        message.data.write(schem.schematic_id);
        message.data.write(schem.schematic_crc);
    });

    message.data.write(GetExperimentationPoints());
    message.data.write(GetAccomplishmentCounter());
    // friend list
    message.data.write(friends_.size());
    message.data.write(0);
    for_each(begin(friends_), end(friends_), [&message](string name){
        message.data.write(name);
    });
    // ignore list
    message.data.write(ignored_players_.size());
    message.data.write(0);
    for_each(begin(ignored_players_), end(ignored_players_), [&message] (string name) {
        message.data.write(name);
    });
    message.data.write(GetLanguage());
    message.data.write(GetCurrentStomach());
    message.data.write(GetMaxStomach());
    message.data.write(GetCurrentDrink());
    message.data.write(GetMaxDrink());
    // unused
    message.data.write(0);
    message.data.write(0);
    message.data.write(0);
    message.data.write(0);
    message.data.write(GetJediState());

    return boost::optional<BaselinesMessage>(move(message));
}

void Player::SetDeltaBitmask_(uint32_t bitmask, uint16_t update_type, BaseObject::ViewType view_type)
{
    if (GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(view_type);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(update_type);
        message.data.write(status_flags_);
        GetScene()->UpdateObservers(GetObjectId(), message);
        deltas_cache_.push_back(std::make_pair(view_type, std::move(message)));
    }
}
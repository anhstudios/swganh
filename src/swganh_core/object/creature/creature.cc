// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "creature.h"

#include "swganh/crc.h"

#include "swganh_core/object/object_events.h"
#include "swganh_core/object/player/player.h"
#include "creature_message_builder.h"

#include "swganh_core/combat/buff_interface.h"
#include "swganh_core/combat/buff_events.h"
#include "swganh_core/messages/controllers/add_buff.h"
#include "swganh_core/messages/controllers/remove_buff.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::containers;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

Creature::Creature()
: Tangible()
, bank_credits_(0)
, cash_credits_(0)
, stat_base_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, skills_(swganh::messages::containers::NetworkList<Skill>())
, posture_(0)
, faction_rank_(0)
, owner_id_(0)
, scale_(1.0f)
, battle_fatigue_(0)
, state_bitmask_(0)
, stat_wound_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, acceleration_multiplier_base_(1.0f)
, acceleration_multiplier_modifier_(1.0f)
, stat_encumberance_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, skill_mod_list_(swganh::messages::containers::NetworkMap<std::string, SkillMod>())
, speed_multiplier_base_(1.0f)
, speed_multiplier_modifier_(1.0f)
, listen_to_id_(0)
, run_speed_(5.0f)
, slope_modifier_angle_(1.02f)
, slope_modifier_percent_(0)
, turn_radius_(1.0f)
, walking_speed_(1.0f)
, water_modifier_percent_(0.0125f)
, mission_critical_object_list_(swganh::messages::containers::NetworkList<MissionCriticalObject>())
, combat_level_(0)
, animation_("")
, mood_animation_("")
, weapon_id_(0)
, group_id_(0)
, invite_sender_id_(0)
, invite_counter_(0)
, guild_id_(0)
, target_id_(0)
, mood_id_(0)
, performance_counter_(0)
, performance_id_(0)
, stat_current_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, stat_max_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, equipment_list_(swganh::messages::containers::NetworkSortedList<EquipmentItem>())
, disguise_("")
, stationary_(0)
, pvp_status_(PvPStatus_Player)
{}

Creature::~Creature()
{}

uint32_t Creature::GetType() const
{
    return Creature::type;
}

void Creature::SetBankCredits(uint32_t bank_credits)
{
    bank_credits_ = bank_credits;
	DISPATCH(Creature, Bank);
}

uint32_t Creature::GetBankCredits(void)
{
    return bank_credits_;
}

void Creature::SetCashCredits(uint32_t cash_credits)
{
    cash_credits_ = cash_credits;
	DISPATCH(Creature, Cash);
}

uint32_t Creature::GetCashCredits(void)
{
    return cash_credits_;
}
void Creature::SetAllStats(int32_t value)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		stat_base_list_.Update(HEALTH, value);
		stat_base_list_.Update(ACTION, value);
		stat_base_list_.Update(MIND, value);
		stat_current_list_.Update(HEALTH, value);
		stat_current_list_.Update(ACTION, value);
		stat_current_list_.Update(MIND, value);
		stat_max_list_.Update(HEALTH, value);
		stat_max_list_.Update(ACTION, value);
		stat_max_list_.Update(MIND, value);
	}
	DISPATCH(Creature, StatBase);
	DISPATCH(Creature, StatCurrent);
	DISPATCH(Creature, StatMax);
}
void Creature::SetStatBase(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_base_list_.Update(stat_index, Stat(value));
    }
	DISPATCH(Creature, StatBase);
}

void Creature::AddStatBase(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        uint32_t new_stat = stat_base_list_[stat_index].value + value;
        stat_base_list_.Update(stat_index, Stat(new_stat));
    }
	DISPATCH(Creature, StatBase);
}

void Creature::DeductStatBase(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t current = stat_base_list_[stat_index].value;
        if (current > value)
        {
            stat_base_list_.Update(stat_index, Stat(current - value));
        }
        else
        {
            stat_base_list_.Update(stat_index, Stat(0));
        }
    }
	DISPATCH(Creature, StatBase);
}

std::vector<Stat> Creature::GetBaseStats(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(stat_base_list_.Get());
}

int32_t Creature::GetStatBase(StatIndex stat_index)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_base_list_.At(stat_index).value;
}

void Creature::AddSkill(std::string skill)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        skills_.Add(Skill(skill));
    }
	DISPATCH(Creature, Skill);
}

void Creature::RemoveSkill(std::string skill)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(skills_), end(skills_), [=](const Skill& other_skill){
            return (skill == other_skill.name);
        });

        if(iter == end(skills_))
        {
            return;
        }

        skills_.Remove(iter);
    }
	DISPATCH(Creature, Skill);
}

std::list<Skill> Creature::GetSkills(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(skills_.Get());
}

bool Creature::HasSkill(std::string skill)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto iter = std::find_if(begin(skills_), end(skills_), [=](const Skill& other_skill){
        return (skill == other_skill.name);
    });

    if(iter != end(skills_))
        return true;
    else
        return false;
}

std::map<uint32_t, std::string>  Creature::GetSkillCommands()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return skill_commands_;
}
bool  Creature::HasSkillCommand(std::string skill_command)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto find_it = find_if(begin(skill_commands_), end(skill_commands_), [=] (pair<uint32_t, string> command){
        return command.second == skill_command;
    });
    if (find_it != end(skill_commands_))
        return true;

    return false;
}
void  Creature::AddSkillCommand(std::pair<uint32_t, std::string> skill_command)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto find_it = skill_commands_.find(skill_command.first);

    if (find_it == end(skill_commands_))
        skill_commands_.insert(skill_command);

}
void  Creature::RemoveSkillCommand(std::string skill_command)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto find_it = find_if(begin(skill_commands_), end(skill_commands_), [=] (pair<uint32_t, string> command){
        return command.second == skill_command;
    });
    if (find_it != end(skill_commands_))
    {
        skill_commands_.erase(find_it);
    }
}

void Creature::SetPosture(Posture posture)
{
    posture_ = posture;
	DISPATCH(Creature, Posture);
}

Posture Creature::GetPosture(void)
{
    uint32_t posture = posture_;
    return (Posture)posture;
}

bool Creature::IsDead()
{
    return posture_ == DEAD;
}
bool Creature::IsIncapacitated()
{
    return posture_ == INCAPACITATED;
}

void Creature::SetFactionRank(uint8_t faction_rank)
{
    faction_rank_ = faction_rank;
	DISPATCH(Creature, FactionRank);
}

uint8_t Creature::GetFactionRank(void)
{
    return faction_rank_;
}

void Creature::SetOwnerId(uint64_t owner_id)
{
    owner_id_ = owner_id;
	DISPATCH(Creature, OwnerId);
}

uint64_t Creature::GetOwnerId(void)
{
    return owner_id_;
}

void Creature::SetScale(float scale)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        scale_ = scale;
    }
	DISPATCH(Creature, Scale);
}

float Creature::GetScale(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return scale_;
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue)
{
    battle_fatigue_ = battle_fatigue;
    DISPATCH(Creature, BattleFatigue);
}
void Creature::AddBattleFatigue(uint32_t battle_fatigue)
{
    battle_fatigue += battle_fatigue;
	DISPATCH(Creature, BattleFatigue);
}
uint32_t Creature::GetBattleFatigue(void)
{
    return battle_fatigue_;
}

void Creature::SetStateBitmask(uint64_t state_bitmask)
{
    state_bitmask_ = state_bitmask;
	DISPATCH(Creature, StateBitmask);
}

uint64_t Creature::GetStateBitmask(void)
{
    return state_bitmask_;
}
bool Creature::HasState(uint64_t state)
{
    return state == (state & state_bitmask_);
}
void Creature::ToggleStateOn(uint64_t state)
{
    state_bitmask_ = ( state_bitmask_ | state);
	DISPATCH(Creature, StateBitmask);
}
void Creature::ToggleStateOff(uint64_t state)
{
    state_bitmask_ = ( state_bitmask_ & ~ state);
	DISPATCH(Creature, StateBitmask);
}
void Creature::ToggleStateBitmask(uint64_t state_bitmask)
{
    state_bitmask_ = (state_bitmask_ ^ state_bitmask);
	DISPATCH(Creature, StateBitmask);
}

void Creature::SetStatWound(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_wound_list_.Update(stat_index, Stat(value));
    }
	DISPATCH(Creature, StatWound);
}

void Creature::AddStatWound(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t new_stat = stat_wound_list_[stat_index].value + value;
        stat_wound_list_.Update(stat_index, Stat(new_stat));
    }
	DISPATCH(Creature, StatWound);
}

void Creature::DeductStatWound(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t current = stat_wound_list_[stat_index].value;
        if (current > value)
        {
            stat_wound_list_.Update(stat_index, Stat(current - value));
        }
        else
        {
            stat_wound_list_.Update(stat_index, Stat(0));
        }
    }
	DISPATCH(Creature, StatWound);
}

std::vector<Stat> Creature::GetStatWounds(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(stat_wound_list_.Get());
}

int32_t Creature::GetStatWound(StatIndex stat_index)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_wound_list_.At(stat_index).value;
}

void Creature::SetAccelerationMultiplierBase(float acceleration_multiplier_base)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        acceleration_multiplier_base_ = acceleration_multiplier_base;
    }
	DISPATCH(Creature, AccelerationMultiplierBase);
}

float Creature::GetAccelerationMultiplierBase(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return acceleration_multiplier_base_;
}

void Creature::SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        acceleration_multiplier_modifier_ = acceleration_multiplier_modifier;
    }
	DISPATCH(Creature, AccelerationMultiplierModifier);
}

float Creature::GetAccelerationMultiplierModifier(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return acceleration_multiplier_modifier_;
}

void Creature::SetStatEncumberance(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_encumberance_list_.Update(stat_index, Stat(value));
    }
	DISPATCH(Creature, StatEncumberance);
}

void Creature::AddStatEncumberance(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t new_stat = stat_encumberance_list_[stat_index].value + value;
        stat_encumberance_list_.Update(stat_index, Stat(new_stat));
    }
	DISPATCH(Creature, StatEncumberance);
}

void Creature::DeductStatEncumberance(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t current = stat_encumberance_list_[stat_index].value;
        if (current > value)
        {
            stat_encumberance_list_.Update(stat_index, Stat(current - value));
        }
        else
        {
            stat_encumberance_list_.Update(stat_index, Stat(0));
        }
    }
	DISPATCH(Creature, StatEncumberance);
}

std::vector<Stat> Creature::GetStatEncumberances(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(stat_encumberance_list_.Get());
}

int32_t Creature::GetStatEncumberance(StatIndex stat_index)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_encumberance_list_.At(stat_index).value;
}

void Creature::AddSkillMod(SkillMod mod)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        skill_mod_list_.Add(mod.identifier, mod);
    }
	DISPATCH(Creature, SkillMod);
}

void Creature::RemoveSkillMod(std::string identifier)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(skill_mod_list_), end(skill_mod_list_), [=](std::pair<std::string, SkillMod> pair)->bool {
            return (identifier == pair.first);
        });

        if(iter != end(skill_mod_list_))
        {
            return;
        }

        skill_mod_list_.Remove(iter);
    }
	DISPATCH(Creature, SkillMod);
}

void Creature::SetSkillMod(SkillMod mod)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        skill_mod_list_.Update(mod.identifier, mod);
    }
	DISPATCH(Creature, SkillMod);
}

void Creature::ClearSkillMods(void)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        skill_mod_list_.Clear();
    }
	DISPATCH(Creature, SkillMod);
}

std::map<std::string, SkillMod> Creature::GetSkillMods()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(skill_mod_list_.Get());
}

SkillMod Creature::GetSkillMod(std::string identifier)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto iter = std::find_if(begin(skill_mod_list_), end(skill_mod_list_), [=](std::pair<std::string, SkillMod> pair)->bool {
        return (pair.second.identifier == identifier);
    });

    if(iter != end(skill_mod_list_))
        return iter->second;
    else
        return SkillMod();
}

void Creature::SetSpeedMultiplierBase(float speed_multiplier_base)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        speed_multiplier_base_ = speed_multiplier_base;
    }
	DISPATCH(Creature, SpeedMultiplierBase);
}

float Creature::GetSpeedMultiplierBase(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return speed_multiplier_base_;
}

void Creature::SetSpeedMultiplierModifier(float speed_multiplier_modifier)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        speed_multiplier_modifier_ = speed_multiplier_modifier;
    }
	DISPATCH(Creature, SpeedMultiplierModifer);
}

float Creature::GetSpeedMultiplierModifier(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return speed_multiplier_modifier_;
}

void Creature::SetListenToId(uint64_t listen_to_id)
{
    listen_to_id_ = listen_to_id;
	DISPATCH(Creature, ListenToId);
}

uint64_t Creature::GetListenToId(void)
{
    return listen_to_id_;
}

void Creature::SetRunSpeed(float run_speed)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        run_speed_ = run_speed;
    }
	DISPATCH(Creature, RunSpeed);
}

float Creature::GetRunSpeed(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return run_speed_;
}

void Creature::SetSlopeModifierAngle(float slope_modifier_angle)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        slope_modifier_angle_ = slope_modifier_angle;
    }
	DISPATCH(Creature, SlopeModifierAngle);
}

float Creature::GetSlopeModifierAngle(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return slope_modifier_angle_;
}

void Creature::SetSlopeModifierPercent(float slope_modifier_percent)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        slope_modifier_percent_ = slope_modifier_percent;
    }
	DISPATCH(Creature, SlopeModifierPercent);
}

float Creature::GetSlopeModifierPercent(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return slope_modifier_percent_;
}

void Creature::SetTurnRadius(float turn_radius)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        turn_radius_ = turn_radius;
    }
	DISPATCH(Creature, TurnRadius);
}

float Creature::GetTurnRadius(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return turn_radius_;
}

void Creature::SetWalkingSpeed(float walking_speed)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        walking_speed_ = walking_speed;
    }
	DISPATCH(Creature, WalkingSpeed);
}

float Creature::GetWalkingSpeed(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return walking_speed_;
}

void Creature::SetWaterModifierPercent(float water_modifier_percent)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        water_modifier_percent_ = water_modifier_percent;
    }
	DISPATCH(Creature, WaterModifierPercent);
}

float Creature::GetWaterModifierPercent(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return water_modifier_percent_;
}

void Creature::AddMissionCriticalObject(MissionCriticalObject& object)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        mission_critical_object_list_.Add(object);
    }
	DISPATCH(Creature, MissionCriticalObject);
}

void Creature::RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(mission_critical_object_list_), end(mission_critical_object_list_), [=](const MissionCriticalObject& obj)->bool {
            if(mission_owner != obj.mission_owner_id_)
                return false;

            if(object_id != obj.critical_object_id_)
                return false;

            return true;
        });

        if(iter != end(mission_critical_object_list_))
        {
            return;
        }

        mission_critical_object_list_.Remove(iter);
    }
	DISPATCH(Creature, MissionCriticalObject);
}

MissionCriticalObject Creature::GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto iter = std::find_if(begin(mission_critical_object_list_), end(mission_critical_object_list_), [=](const MissionCriticalObject& x)->bool {
        if(x.mission_owner_id_ != mission_owner)
            return false;

        if(x.critical_object_id_ != object_id)
            return false;

        return true;
    });

    if(iter != end(mission_critical_object_list_))
        return *iter;
    else
        return MissionCriticalObject(0, 0);
}

std::list<MissionCriticalObject> Creature::GetMissionCriticalObjects(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(mission_critical_object_list_.Get());
}

void Creature::SetCombatLevel(uint16_t combat_level)
{
    combat_level_ = combat_level;
	DISPATCH(Creature, CombatLevel);
}

uint16_t Creature::GetCombatLevel(void)
{
    return combat_level_;
}

void Creature::SetAnimation(std::string animation)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        animation_ = animation;
    }
	DISPATCH(Creature, Animation);
}

std::string Creature::GetAnimation(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return animation_;
}

void Creature::SetMoodAnimation(std::string mood_animation)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        mood_animation_ = mood_animation;
    }
	DISPATCH(Creature, MoodAnimation);
}

std::string Creature::GetMoodAnimation(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mood_animation_;
}

void Creature::SetWeaponId(uint64_t weapon_id)
{
    weapon_id_ = weapon_id;
	DISPATCH(Creature, WeaponId);
}

uint64_t Creature::GetWeaponId(void)
{
    return weapon_id_;
}

void Creature::SetGroupId(uint64_t group_id)
{
    group_id_ = group_id;
    DISPATCH(Creature, GroupId);
}

uint64_t Creature::GetGroupId(void)
{
    return group_id_;
}

void Creature::SetInviteSenderId(uint64_t invite_sender_id)
{
    invite_sender_id_ = invite_sender_id;
    DISPATCH(Creature, InviteSenderId);
}

uint64_t Creature::GetInviteSenderId(void)
{
    return invite_sender_id_;
}

void Creature::SetInviteCounter(uint64_t invite_counter)
{
    invite_counter_ = invite_counter;
}

uint64_t Creature::IncrementInviteCounter()
{
    return invite_counter_++;
}

uint64_t Creature::GetInviteCounter(void) const
{
    return invite_counter_;
}

void Creature::SetGuildId(uint32_t guild_id)
{
    guild_id_ = guild_id;
	DISPATCH(Creature, GuildId);
}

uint32_t Creature::GetGuildId(void)
{
    return guild_id_;
}

void Creature::SetTargetId(uint64_t target_id)
{
    target_id_ = target_id;
    DISPATCH(Creature, TargetId);
}

uint64_t Creature::GetTargetId(void)
{
    return target_id_;
}

void Creature::SetMoodId(uint8_t mood_id)
{
    mood_id_ = mood_id;
    DISPATCH(Creature, MoodId);
}

uint8_t Creature::GetMoodId(void)
{
    return mood_id_;
}

void Creature::SetPerformanceId(uint32_t performance_id)
{
    performance_id_ = performance_id;
    DISPATCH(Creature, PerformanceId);
}

uint32_t Creature::GetPerformanceId(void)
{
    return performance_id_;
}

void Creature::SetPerformanceCounter(uint32_t performance_counter)
{
    performance_counter_ = performance_counter;
}

uint32_t Creature::IncrementPerformanceCounter()
{
    return performance_counter_++;
}

uint32_t Creature::GetPerformanceCounter(void) const
{
    return performance_counter_;
}

void Creature::SetStatCurrent(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_current_list_.Update(stat_index, Stat(value));
    }
	DISPATCH(Creature, StatCurrent);
}

void Creature::AddStatCurrent(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t new_value = stat_current_list_[stat_index].value + value;
        stat_current_list_.Update(stat_index, Stat(new_value));
    }
	DISPATCH(Creature, StatCurrent);
}

void Creature::DeductStatCurrent(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t current = stat_current_list_[stat_index].value;
        if (current > value)
        {
            stat_current_list_.Update(stat_index, Stat(current - value));
        }
        else
        {
            stat_current_list_.Update(stat_index, Stat(0));
        }
    }
	DISPATCH(Creature, StatCurrent);
}

std::vector<Stat> Creature::GetCurrentStats(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(stat_current_list_.Get());
}

int32_t Creature::GetStatCurrent(StatIndex stat_index)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_current_list_.At(stat_index).value;
}

void Creature::SetStatMax(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_max_list_.Update(stat_index, Stat(value));
    }
	DISPATCH(Creature, StatMax);
}

void Creature::AddStatMax(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_max_list_.Update(stat_index, Stat(stat_max_list_.At(stat_index).value + value));
    }
	DISPATCH(Creature, StatMax);
}

void Creature::DeductStatMax(StatIndex stat_index, int32_t value)
{
    {   
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t current = stat_max_list_[stat_index].value;
        if (current > value)
        {
            stat_max_list_.Update(stat_index, Stat(current - value));
        }
        else
        {
            stat_max_list_.Update(stat_index, Stat(0));
        }
    }
	DISPATCH(Creature, StatMax);
}

std::vector<Stat> Creature::GetMaxStats(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return std::move(stat_max_list_.Get());
}

int32_t Creature::GetStatMax(StatIndex stat_index)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_max_list_.At(stat_index).value;
}

void Creature::AddEquipmentItem(EquipmentItem& item)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        equipment_list_.Add(item);
    }
	DISPATCH(Creature, EquipmentItem);
}

void Creature::RemoveEquipmentItem(uint64_t object_id)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = std::find_if(begin(equipment_list_), end(equipment_list_), [=](std::pair<uint16_t, EquipmentItem> item)->bool {
            return (object_id == item.second.object_id);
        });

        if(iter != end(equipment_list_))
        {
            return;
        }
        equipment_list_.Remove(iter);
    }
	DISPATCH(Creature, EquipmentItem);
}

void Creature::UpdateEquipmentItem(EquipmentItem& item)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = equipment_list_.Find(item);
        if(iter != end(equipment_list_))
            equipment_list_.Update(iter->first, item);
    }
	DISPATCH(Creature, EquipmentItem);
}

std::list<EquipmentItem> Creature::GetEquipment(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
	std::list<EquipmentItem> result;
	for(auto& v : equipment_list_)
		result.push_back(v.second);
    return std::move(result);
}

EquipmentItem Creature::GetEquipmentItem(uint64_t object_id)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto iter = std::find_if(begin(equipment_list_), end(equipment_list_), [=](std::pair<uint16_t, EquipmentItem> pair) {
        return pair.second.object_id == object_id;
    });

    if(iter != end(equipment_list_))
        return iter->second;
    else
        return EquipmentItem();
}

void Creature::SetDisguise(std::string disguise)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        disguise_ = disguise;
    }
    DISPATCH(Creature, Disguise);
}

std::string Creature::GetDisguise(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return disguise_;
}

void Creature::SetStationary(bool stationary)
{
    stationary_ = stationary;
    DISPATCH(Creature, Stationary);
}

bool Creature::IsStationary(void)
{
    return stationary_;
}

PvpStatus Creature::GetPvpStatus() const
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return pvp_status_;
}

void Creature::SetPvPStatus(PvpStatus status)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = status;
    }
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpStateOn(PvpStatus state)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = static_cast<PvpStatus>(pvp_status_ | state);
    }
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpStateOff(PvpStatus state)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = static_cast<PvpStatus>(pvp_status_ & ~state);
    }
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpState(PvpStatus state)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = static_cast<PvpStatus>(pvp_status_ ^ state);
    }
	DISPATCH(Creature, PvPStatus);
}

bool Creature::CheckPvpState(PvpStatus state) const
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return static_cast<PvpStatus>(pvp_status_ & state) == state;
}
bool Creature::CanAttack(Creature* creature)
{
    if (creature->CheckPvpState(PvPStatus_Attackable) || creature->CheckPvpState(PvPStatus_Enemy))
    {
       return true;
    }
    if (creature->CheckPvpState(PvPStatus_Overt) || creature->CheckPvpState(PvPStatus_Tef))
    {
        if (CheckPvpState(PvPStatus_Overt) || CheckPvpState(PvPStatus_Tef))
            return true;
    }
    if (creature->CheckPvpState(PvPStatus_Duel) && InDuelList(creature->GetObjectId()) && creature->InDuelList(GetObjectId()))
        return true;

    return false;
}

void Creature::AddToDuelList(uint64_t id)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    if (found == end(duel_list_))
        duel_list_.push_back(id);
}
void Creature::RemoveFromDuelList(uint64_t id)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    if (found != end(duel_list_))
        duel_list_.erase(found);
}
bool Creature::InDuelList(uint64_t id)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    return found != end(duel_list_);

}
std::vector<uint64_t> Creature::GetDuelList()
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return duel_list_;
}

void Creature::CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	if (event_dispatcher_)
	{
		GetEventDispatcher()->Dispatch(make_shared<ObserverEvent>
			("Creature::Baselines", shared_from_this(), observer));
	}
}

bool Creature::HasBuff(std::string buff_name)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto find_itr = std::find_if(buffs_.begin(), buffs_.end(), [&] (BuffMap::value_type& entry) {
		if(entry.second->GetName() == buff_name)
		{
			return true;
		}
		return false;
	});
	return find_itr != buffs_.end();
}

void Creature::AddBuff(std::string buff_name, uint32_t duration)
{
	if(event_dispatcher_ && !HasBuff(buff_name))
	{
		event_dispatcher_->Dispatch(std::make_shared<swganh::combat::BuffEvent>
			("CombatService::AddBuff", std::static_pointer_cast<Creature>(shared_from_this()), buff_name, duration));
	}
}

void Creature::__AddBuffInternal(boost::posix_time::ptime time, std::shared_ptr<swganh::combat::BuffInterface> buff, uint32_t duration)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		buffs_.insert(BuffMap::value_type(time, buff));
	}

	auto controller = GetController();
	if(controller)
	{
		AddBuffMessage msg;
		msg.buff = buff->GetName();
		msg.duration = static_cast<float>(duration);
		controller->Notify(&msg);

		buff->ApplyBuff(std::static_pointer_cast<Creature>(shared_from_this()));
	}
}

void Creature::RemoveBuff(std::string name)
{
	std::shared_ptr<swganh::combat::BuffInterface> interface_;
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_itr = std::find_if(buffs_.begin(), buffs_.end(), [&] (BuffMap::value_type& entry) {
			if(entry.second->GetName() == name)
			{
				interface_ = entry.second;
				return true;
			}
			return false;
		});

		if(find_itr != buffs_.end())
		{
			buffs_.erase(find_itr);
		}
	}

	auto controller = GetController();
	if(interface_)
	{
		RemoveBuffMessage msg;
		msg.buff = name;
		controller->Notify(&msg);

		interface_->RemoveBuff(std::static_pointer_cast<Creature>(shared_from_this()));
	}
}

void Creature::ClearBuffs()
{
	std::set<std::shared_ptr<swganh::combat::BuffInterface>> removed_buffs_;
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);

		std::for_each(buffs_.begin(), buffs_.end(), [&] (BuffMap::value_type& entry) {
			if(controller_)
			{
				RemoveBuffMessage msg;
				msg.buff = entry.second->GetName();
				controller_->Notify(&msg);

				removed_buffs_.insert(entry.second);
			}
		});

		buffs_.clear();
	}

	for(auto& v : removed_buffs_)
	{
		v->RemoveBuff(std::static_pointer_cast<Creature>(shared_from_this()));
	}
}

void Creature::ClearBuffs(boost::posix_time::ptime current_time)
{
	std::set<std::shared_ptr<swganh::combat::BuffInterface>> removed_buffs_;
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto lower_bound = buffs_.lower_bound(current_time);
		auto end = buffs_.end();

		auto begin = buffs_.begin();
		while(begin != end && begin != lower_bound)
		{
			removed_buffs_.insert(begin->second);
			buffs_.erase(begin);
			begin = buffs_.begin();
		}
	}

	for(auto& v : removed_buffs_)
	{
		v->RemoveBuff(std::static_pointer_cast<Creature>(shared_from_this()));
	}
}

void Creature::ViewBuffs(BuffIterator functor)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	for_each(buffs_.begin(), buffs_.end(), functor);
}

void Creature::CleanUpBuffs()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	buffs_.clear();
}

std::shared_ptr<Object> Creature::Clone()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	auto other = make_shared<Creature>();
	Clone(other);
	return other;
}

void Creature::Clone(std::shared_ptr<Creature> other)
{
	other->bank_credits_.store(bank_credits_);
    other->cash_credits_.store(cash_credits_);
    other->stat_base_list_ = stat_base_list_;
    other->skills_ = skills_;
    other->skill_commands_ = skill_commands_;
    other->posture_.store(posture_);
    other->faction_rank_.store(faction_rank_);
    other->owner_id_.store(owner_id_);
    other->scale_ = scale_;
    other->battle_fatigue_.store(battle_fatigue_);
    other->state_bitmask_.store(state_bitmask_);
	other->stat_wound_list_ = stat_wound_list_;
	other->acceleration_multiplier_base_ = acceleration_multiplier_base_;
	other->acceleration_multiplier_modifier_ = acceleration_multiplier_modifier_;
	other->stat_encumberance_list_ = stat_encumberance_list_;
    other->skill_mod_list_ = skill_mod_list_;
	other->speed_multiplier_base_ = speed_multiplier_base_;
	other->speed_multiplier_modifier_ =  speed_multiplier_modifier_;
	other->listen_to_id_.store(listen_to_id_);
    other->run_speed_ = run_speed_;
	other->slope_modifier_angle_ = slope_modifier_angle_;
    other->slope_modifier_percent_ = slope_modifier_percent_;
    other->turn_radius_ = turn_radius_;
    other->walking_speed_ = walking_speed_;
	other->water_modifier_percent_ = water_modifier_percent_;
	other->mission_critical_object_list_ = mission_critical_object_list_;
	other->combat_level_.store(combat_level_);
    other->animation_ = animation_;
    other->mood_animation_ = mood_animation_;
    other->weapon_id_.store(weapon_id_);
    other->group_id_.store(group_id_);
	other->invite_sender_id_.store(invite_sender_id_);
    other->invite_counter_.store(invite_counter_);
    other->guild_id_.store(guild_id_);
    other->target_id_.store(target_id_);
    other->mood_id_.store(mood_id_);
    other->performance_counter_.store(performance_counter_);
    other->performance_id_.store(performance_id_);
	other->stat_current_list_ = stat_current_list_;
	other->stat_max_list_ = stat_max_list_;
    other->equipment_list_ =  equipment_list_;
	other->disguise_ = disguise_;
	other->stationary_.store(stationary_);
	other->pvp_status_ = pvp_status_;
    other->duel_list_ = duel_list_;

	Tangible::Clone(other);
}

void Creature::SerializeBaseStats(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	stat_base_list_.Serialize(message);
}

void Creature::SerializeSkills(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	skills_.Serialize(message);
}

void Creature::SerializeStatWounds(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	stat_wound_list_.Serialize(message);
}

void Creature::SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	stat_encumberance_list_.Serialize(message);
}

void Creature::SerializeSkillMods(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	skill_mod_list_.Serialize(message);
}

void Creature::SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	mission_critical_object_list_.Serialize(message);
}

void Creature::SerializeCurrentStats(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	stat_current_list_.Serialize(message);
}

void Creature::SerializeMaxStats(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	stat_max_list_.Serialize(message);
}

void Creature::SerializeEquipment(swganh::messages::BaseSwgMessage* message)
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
	equipment_list_.Serialize(message);
}

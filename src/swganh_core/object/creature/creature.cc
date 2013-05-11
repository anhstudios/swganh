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

void Creature::SetBankCredits(uint32_t bank_credits) { SetBankCredits(bank_credits, AcquireLock()); }
void Creature::SetBankCredits(uint32_t bank_credits, boost::unique_lock<boost::mutex>& lock)
{
    bank_credits_ = bank_credits;
	DISPATCH(Creature, Bank);
}

uint32_t Creature::GetBankCredits() { return GetBankCredits(AcquireLock()); }
uint32_t Creature::GetBankCredits(boost::unique_lock<boost::mutex>& lock)
{
    return bank_credits_;
}

void Creature::SetCashCredits(uint32_t cash_credits) { SetCashCredits(cash_credits, AcquireLock()); }
void Creature::SetCashCredits(uint32_t cash_credits, boost::unique_lock<boost::mutex>& lock)
{
    cash_credits_ = cash_credits;
	DISPATCH(Creature, Cash);
}

uint32_t Creature::GetCashCredits() { return GetCashCredits(AcquireLock()); }
uint32_t Creature::GetCashCredits(boost::unique_lock<boost::mutex>& lock)
{
    return cash_credits_;
}

void Creature::SetAllStats(int32_t value) {SetAllStats(value, AcquireLock());}
void Creature::SetAllStats(int32_t value, boost::unique_lock<boost::mutex>& lock)
{
	stat_base_list_.Update(HEALTH, value);
	stat_base_list_.Update(ACTION, value);
	stat_base_list_.Update(MIND, value);
	stat_current_list_.Update(HEALTH, value);
	stat_current_list_.Update(ACTION, value);
	stat_current_list_.Update(MIND, value);
	stat_max_list_.Update(HEALTH, value);
	stat_max_list_.Update(ACTION, value);
	stat_max_list_.Update(MIND, value);
	DISPATCH(Creature, StatBase);
	DISPATCH(Creature, StatCurrent);
	DISPATCH(Creature, StatMax);
}

void Creature::SetStatBase(StatIndex stat_index, int32_t value) {SetStatBase(stat_index, value, AcquireLock());}
void Creature::SetStatBase(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock) 
{
	stat_base_list_.Update(stat_index, Stat(value));
	DISPATCH(Creature, StatBase);
}

void Creature::AddStatBase(StatIndex stat_index, int32_t value) { AddStatBase(stat_index, value, AcquireLock());}
void Creature::AddStatBase(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    uint32_t new_stat = stat_base_list_[stat_index].value + value;
    stat_base_list_.Update(stat_index, Stat(new_stat));
	DISPATCH(Creature, StatBase);
}

void Creature::DeductStatBase(StatIndex stat_index, int32_t value) { DeductStatBase(stat_index, value, AcquireLock());}
void Creature::DeductStatBase(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_base_list_[stat_index].value;
    if (current > value)
    {
        stat_base_list_.Update(stat_index, Stat(current - value));
    }
    else
    {
        stat_base_list_.Update(stat_index, Stat(0));
    }
	DISPATCH(Creature, StatBase);
}

std::vector<Stat> Creature::GetBaseStats() { return GetBaseStats(AcquireLock());}
std::vector<Stat> Creature::GetBaseStats(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(stat_base_list_.Get());
}

int32_t Creature::GetStatBase(StatIndex stat_index) { return GetStatBase(stat_index, AcquireLock());}
int32_t Creature::GetStatBase(StatIndex stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_base_list_.At(stat_index).value;
}

void Creature::AddSkill(std::string skill) { AddSkill(skill, AcquireLock());}
void Creature::AddSkill(std::string skill, boost::unique_lock<boost::mutex>& lock)
{
    skills_.Add(Skill(skill));
	DISPATCH(Creature, Skill);
}

void Creature::RemoveSkill(std::string skill) { RemoveSkill(skill, AcquireLock());}
void Creature::RemoveSkill(std::string skill, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(skills_), end(skills_), [=](const Skill& other_skill){
        return (skill == other_skill.name);
    });

    if(iter == end(skills_))
    {
        return;
    }

    skills_.Remove(iter);
	DISPATCH(Creature, Skill);
}

std::list<Skill> Creature::GetSkills() { return GetSkills(AcquireLock());}
std::list<Skill> Creature::GetSkills(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(skills_.Get());
}

bool Creature::HasSkill(std::string skill) { return HasSkill(skill, AcquireLock());}
bool Creature::HasSkill(std::string skill, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(skills_), end(skills_), [=](const Skill& other_skill){
        return (skill == other_skill.name);
    });

    if(iter != end(skills_))
	{
        return true;
	}
    else
	{
        return false;
	}
}

std::map<uint32_t, std::string>  Creature::GetSkillCommands() { return GetSkillCommands(AcquireLock());}
std::map<uint32_t, std::string>  Creature::GetSkillCommands(boost::unique_lock<boost::mutex>& lock)
{
    return skill_commands_;
}


bool  Creature::HasSkillCommand(std::string skill_command) { return HasSkillCommand(skill_command, AcquireLock());}
bool  Creature::HasSkillCommand(std::string skill_command, boost::unique_lock<boost::mutex>& lock)
{
    auto find_it = find_if(begin(skill_commands_), end(skill_commands_), [=] (pair<uint32_t, string> command){
        return command.second == skill_command;
    });
    if (find_it != end(skill_commands_))
	{
        return true;
	}

    return false;
}


void  Creature::AddSkillCommand(std::pair<uint32_t, std::string> skill_command) { AddSkillCommand(skill_command, AcquireLock());}
void  Creature::AddSkillCommand(std::pair<uint32_t, std::string> skill_command, boost::unique_lock<boost::mutex>& lock)
{
    auto find_it = skill_commands_.find(skill_command.first);

    if (find_it == end(skill_commands_))
	{
        skill_commands_.insert(skill_command);
	}
}

void  Creature::RemoveSkillCommand(std::string skill_command) { RemoveSkillCommand(skill_command, AcquireLock());}
void  Creature::RemoveSkillCommand(std::string skill_command, boost::unique_lock<boost::mutex>& lock)
{
    auto find_it = find_if(begin(skill_commands_), end(skill_commands_), [=] (pair<uint32_t, string> command){
        return command.second == skill_command;
    });
    if (find_it != end(skill_commands_))
    {
        skill_commands_.erase(find_it);
    }
}

void Creature::SetPosture(Posture posture) { SetPosture(posture, AcquireLock()); }
void Creature::SetPosture(Posture posture, boost::unique_lock<boost::mutex>& lock)
{
    posture_ = posture;
	DISPATCH(Creature, Posture);
}

Posture Creature::GetPosture() { return GetPosture(AcquireLock()); }
Posture Creature::GetPosture(boost::unique_lock<boost::mutex>& lock)
{
    uint32_t posture = posture_;
    return (Posture)posture;
}

bool Creature::IsDead() { return IsDead(AcquireLock()); }
bool Creature::IsDead(boost::unique_lock<boost::mutex>& lock)
{
    return posture_ == DEAD;
}

bool Creature::IsIncapacitated() { return IsIncapacitated(AcquireLock()); }
bool Creature::IsIncapacitated(boost::unique_lock<boost::mutex>& lock)
{
    return posture_ == INCAPACITATED;
}

void Creature::SetFactionRank(uint8_t faction_rank) { SetFactionRank(faction_rank, AcquireLock()); }
void Creature::SetFactionRank(uint8_t faction_rank, boost::unique_lock<boost::mutex>& lock)
{
    faction_rank_ = faction_rank;
	DISPATCH(Creature, FactionRank);
}

uint8_t Creature::GetFactionRank() { return GetFactionRank(AcquireLock()); }
uint8_t Creature::GetFactionRank(boost::unique_lock<boost::mutex>& lock)
{
    return faction_rank_;
}

void Creature::SetOwnerId(uint64_t owner_id) { SetOwnerId(owner_id, AcquireLock()); }
void Creature::SetOwnerId(uint64_t owner_id, boost::unique_lock<boost::mutex>& lock)
{
    owner_id_ = owner_id;
	DISPATCH(Creature, OwnerId);
}

uint64_t Creature::GetOwnerId(void) { return GetOwnerId(AcquireLock()); }
uint64_t Creature::GetOwnerId(boost::unique_lock<boost::mutex>& lock)
{
    return owner_id_;
}

void Creature::SetScale(float scale) { SetScale(scale, AcquireLock());}
void Creature::SetScale(float scale, boost::unique_lock<boost::mutex>& lock)
{
    {
        auto lock = AcquireLock();
        scale_ = scale;
    }
	DISPATCH(Creature, Scale);
}

float Creature::GetScale() { return GetScale(AcquireLock());}
float Creature::GetScale(boost::unique_lock<boost::mutex>& lock)
{
    auto lock = AcquireLock();
    return scale_;
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue) { SetBattleFatigue(battle_fatigue, AcquireLock()); }
void Creature::SetBattleFatigue(uint32_t battle_fatigue, boost::unique_lock<boost::mutex>& lock)
{
    battle_fatigue_ = battle_fatigue;
    DISPATCH(Creature, BattleFatigue);
}

void Creature::AddBattleFatigue(uint32_t battle_fatigue) { AddBattleFatigue(battle_fatigue, AcquireLock()); }
void Creature::AddBattleFatigue(uint32_t battle_fatigue, boost::unique_lock<boost::mutex>& lock)
{
    battle_fatigue += battle_fatigue;
	DISPATCH(Creature, BattleFatigue);
}

uint32_t Creature::GetBattleFatigue() { return GetBattleFatigue(AcquireLock()); }
uint32_t Creature::GetBattleFatigue(boost::unique_lock<boost::mutex>& lock)
{
    return battle_fatigue_;
}

void Creature::SetStateBitmask(uint64_t state_bitmask) { SetStateBitmask(state_bitmask, AcquireLock()); }
void Creature::SetStateBitmask(uint64_t state_bitmask, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = state_bitmask;
	DISPATCH(Creature, StateBitmask);
}

uint64_t Creature::GetStateBitmask() { return GetStateBitmask(AcquireLock()); }
uint64_t Creature::GetStateBitmask(boost::unique_lock<boost::mutex>& lock)
{
    return state_bitmask_;
}

bool Creature::HasState(uint64_t state) { return HasState(state, AcquireLock()); }
bool Creature::HasState(uint64_t state, boost::unique_lock<boost::mutex>& lock)
{
    return state == (state & state_bitmask_);
}

void Creature::ToggleStateOn(uint64_t state) { ToggleStateOn(state, AcquireLock()); }
void Creature::ToggleStateOn(uint64_t state, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = ( state_bitmask_ | state);
	DISPATCH(Creature, StateBitmask);
}

void Creature::ToggleStateOff(uint64_t state) { ToggleStateOff(state, AcquireLock()); }
void Creature::ToggleStateOff(uint64_t state, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = ( state_bitmask_ & ~ state);
	DISPATCH(Creature, StateBitmask);
}

void Creature::ToggleStateBitmask(uint64_t state_bitmask) { ToggleStateBitmask(state_bitmask, AcquireLock()); }
void Creature::ToggleStateBitmask(uint64_t state_bitmask, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = (state_bitmask_ ^ state_bitmask);
	DISPATCH(Creature, StateBitmask);
}

void Creature::SetStatWound(StatIndex stat_index, int32_t value) { SetStatWound(stat_index, value, AcquireLock());}
void Creature::SetStatWound(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_wound_list_.Update(stat_index, Stat(value));
	DISPATCH(Creature, StatWound);
}

void Creature::AddStatWound(StatIndex stat_index, int32_t value) { AddStatWound(stat_index, value, AcquireLock());}
void Creature::AddStatWound(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_stat = stat_wound_list_[stat_index].value + value;
    stat_wound_list_.Update(stat_index, Stat(new_stat));
	DISPATCH(Creature, StatWound);
}

void Creature::DeductStatWound(StatIndex stat_index, int32_t value) { DeductStatWound(stat_index, value, AcquireLock());}
void Creature::DeductStatWound(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_wound_list_[stat_index].value;
    if (current > value)
    {
        stat_wound_list_.Update(stat_index, Stat(current - value));
    }
    else
    {
        stat_wound_list_.Update(stat_index, Stat(0));
    }
	DISPATCH(Creature, StatWound);
}

std::vector<Stat> Creature::GetStatWounds() { return GetStatWounds(AcquireLock());}
std::vector<Stat> Creature::GetStatWounds(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(stat_wound_list_.Get());
}

int32_t Creature::GetStatWound(StatIndex stat_index) { GetStatWound(stat_index, AcquireLock());}
int32_t Creature::GetStatWound(StatIndex stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_wound_list_.At(stat_index).value;
}

void Creature::SetAccelerationMultiplierBase(float acceleration_multiplier_bas) { SetAccelerationMultiplierBase(acceleration_multiplier_bas, AcquireLock());}
void Creature::SetAccelerationMultiplierBase(float acceleration_multiplier_bas, boost::unique_lock<boost::mutex>& lock)
{
    acceleration_multiplier_base_ = acceleration_multiplier_bas;
	DISPATCH(Creature, AccelerationMultiplierBase);
}

float Creature::GetAccelerationMultiplierBase() { return GetAccelerationMultiplierBase(AcquireLock());}
float Creature::GetAccelerationMultiplierBase(boost::unique_lock<boost::mutex>& lock)
{
    return acceleration_multiplier_base_;
}

void Creature::SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier) { SetAccelerationMultiplierModifier(acceleration_multiplier_modifier, AcquireLock());}
void Creature::SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier, boost::unique_lock<boost::mutex>& lock)
{
    acceleration_multiplier_modifier_ = acceleration_multiplier_modifier;
	DISPATCH(Creature, AccelerationMultiplierModifier);
}

float Creature::GetAccelerationMultiplierModifier() { return GetAccelerationMultiplierModifier(AcquireLock()); }
float Creature::GetAccelerationMultiplierModifier(boost::unique_lock<boost::mutex>& lock)
{
    return acceleration_multiplier_modifier_;
}

void Creature::SetStatEncumberance(StatIndex stat_index, int32_t value) { SetStatEncumberance(stat_index, value, AcquireLock()); }
void Creature::SetStatEncumberance(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_encumberance_list_.Update(stat_index, Stat(value));
	DISPATCH(Creature, StatEncumberance);
}

void Creature::AddStatEncumberance(StatIndex stat_index, int32_t value) { AddStatEncumberance(stat_index, value, AcquireLock()); }
void Creature::AddStatEncumberance(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_stat = stat_encumberance_list_[stat_index].value + value;
    stat_encumberance_list_.Update(stat_index, Stat(new_stat));
	DISPATCH(Creature, StatEncumberance);
}

void Creature::DeductStatEncumberance(StatIndex stat_index, int32_t value) { DeductStatEncumberance(stat_index, value, AcquireLock()); }
void Creature::DeductStatEncumberance(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_encumberance_list_[stat_index].value;
    if (current > value)
    {
        stat_encumberance_list_.Update(stat_index, Stat(current - value));
    }
    else
    {
        stat_encumberance_list_.Update(stat_index, Stat(0));
    }
	DISPATCH(Creature, StatEncumberance);
}

std::vector<Stat> Creature::GetStatEncumberances() { return GetStatEncumberances(AcquireLock()); }
std::vector<Stat> Creature::GetStatEncumberances(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(stat_encumberance_list_.Get());
}

int32_t Creature::GetStatEncumberance(StatIndex stat_index) { return GetStatEncumberance(stat_index, AcquireLock()); }
int32_t Creature::GetStatEncumberance(StatIndex stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_encumberance_list_.At(stat_index).value;
}

void Creature::AddSkillMod(SkillMod mod) { AddSkillMod(mod, AcquireLock()); }
void Creature::AddSkillMod(SkillMod mod, boost::unique_lock<boost::mutex>& lock)
{
    skill_mod_list_.Add(mod.identifier, mod);
	DISPATCH(Creature, SkillMod);
}

void Creature::RemoveSkillMod(std::string identifier) { RemoveSkillMod(identifier, AcquireLock()); }
void Creature::RemoveSkillMod(std::string identifier, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(skill_mod_list_), end(skill_mod_list_), [=](std::pair<std::string, SkillMod> pair)->bool {
        return (identifier == pair.first);
    });

    if(iter != end(skill_mod_list_))
    {
        return;
    }

    skill_mod_list_.Remove(iter);
	DISPATCH(Creature, SkillMod);
}

void Creature::SetSkillMod(SkillMod mod) { SetSkillMod(mod, AcquireLock()); }
void Creature::SetSkillMod(SkillMod mod, boost::unique_lock<boost::mutex>& lock)
{
    skill_mod_list_.Update(mod.identifier, mod);
	DISPATCH(Creature, SkillMod);
}

void Creature::ClearSkillMods() { ClearSkillMods(AcquireLock()); }
void Creature::ClearSkillMods(boost::unique_lock<boost::mutex>& lock)
{
	skill_mod_list_.Clear();
	DISPATCH(Creature, SkillMod);
}

std::map<std::string, SkillMod> Creature::GetSkillMods() { return GetSkillMods(AcquireLock()); }
std::map<std::string, SkillMod> Creature::GetSkillMods(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(skill_mod_list_.Get());
}

SkillMod Creature::GetSkillMod(std::string identifier) { return GetSkillMod(identifier, AcquireLock()); }
SkillMod Creature::GetSkillMod(std::string identifier, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(skill_mod_list_), end(skill_mod_list_), [=](std::pair<std::string, SkillMod> pair)->bool {
        return (pair.second.identifier == identifier);
    });

    if(iter != end(skill_mod_list_))
	{
        return iter->second;
	}
    else
	{
        return SkillMod();
	}
}

void Creature::SetSpeedMultiplierBase(float speed_multiplier_base) { SetSpeedMultiplierBase(speed_multiplier_base, AcquireLock()); }
void Creature::SetSpeedMultiplierBase(float speed_multiplier_base, boost::unique_lock<boost::mutex>& lock)
{
	speed_multiplier_base_ = speed_multiplier_base;
	DISPATCH(Creature, SpeedMultiplierBase);
}

float Creature::GetSpeedMultiplierBase() { return GetSpeedMultiplierBase(AcquireLock()); }
float Creature::GetSpeedMultiplierBase(boost::unique_lock<boost::mutex>& lock)
{
    return speed_multiplier_base_;
}

void Creature::SetSpeedMultiplierModifier(float speed_multiplier_modifier) { SetSpeedMultiplierModifier(speed_multiplier_modifier, AcquireLock()); }
void Creature::SetSpeedMultiplierModifier(float speed_multiplier_modifier, boost::unique_lock<boost::mutex>& lock)
{
    speed_multiplier_modifier_ = speed_multiplier_modifier;
	DISPATCH(Creature, SpeedMultiplierModifer);
}

float Creature::GetSpeedMultiplierModifier() { return GetSpeedMultiplierModifier(AcquireLock()); }
float Creature::GetSpeedMultiplierModifier(boost::unique_lock<boost::mutex>& lock)
{
    return speed_multiplier_modifier_;
}

void Creature::SetListenToId(uint64_t listen_to_id) { SetListenToId(listen_to_id, AcquireLock()); }
void Creature::SetListenToId(uint64_t listen_to_id, boost::unique_lock<boost::mutex>& lock)
{
    listen_to_id_ = listen_to_id;
	DISPATCH(Creature, ListenToId);
}

uint64_t Creature::GetListenToId() { return GetListenToId(AcquireLock()); }
uint64_t Creature::GetListenToId(boost::unique_lock<boost::mutex>& lock)
{
    return listen_to_id_;
}

void Creature::SetRunSpeed(float run_speed) { SetRunSpeed(run_speed, AcquireLock()); }
void Creature::SetRunSpeed(float run_speed, boost::unique_lock<boost::mutex>& lock)
{
	run_speed_ = run_speed;
	DISPATCH(Creature, RunSpeed);
}

float Creature::GetRunSpeed() { return GetRunSpeed(AcquireLock()); }
float Creature::GetRunSpeed(boost::unique_lock<boost::mutex>& lock)
{
    return run_speed_;
}

void Creature::SetSlopeModifierAngle(float slope_modifier_angle) { SetSlopeModifierAngle(slope_modifier_angle, AcquireLock()); }
void Creature::SetSlopeModifierAngle(float slope_modifier_angle, boost::unique_lock<boost::mutex>& lock)
{
	slope_modifier_angle_ = slope_modifier_angle;
	DISPATCH(Creature, SlopeModifierAngle);
}

float Creature::GetSlopeModifierAngle() { return GetSlopeModifierAngle(AcquireLock()); }
float Creature::GetSlopeModifierAngle(boost::unique_lock<boost::mutex>& lock)
{
    return slope_modifier_angle_;
}

void Creature::SetSlopeModifierPercent(float slope_modifier_percent) { SetSlopeModifierPercent(slope_modifier_percent, AcquireLock()); }
void Creature::SetSlopeModifierPercent(float slope_modifier_percent, boost::unique_lock<boost::mutex>& lock)
{
    slope_modifier_percent_ = slope_modifier_percent;
	DISPATCH(Creature, SlopeModifierPercent);
}

float Creature::GetSlopeModifierPercent() { return GetSlopeModifierPercent(AcquireLock()); }
float Creature::GetSlopeModifierPercent(boost::unique_lock<boost::mutex>& lock)
{
    return slope_modifier_percent_;
}

void Creature::SetTurnRadius(float turn_radius) { SetTurnRadius(turn_radius, AcquireLock()); }
void Creature::SetTurnRadius(float turn_radius, boost::unique_lock<boost::mutex>& lock)
{
	turn_radius_ = turn_radius;
	DISPATCH(Creature, TurnRadius);
}

float Creature::GetTurnRadius() { return GetTurnRadius(AcquireLock()); }
float Creature::GetTurnRadius(boost::unique_lock<boost::mutex>& lock)
{
    return turn_radius_;
}

void Creature::SetWalkingSpeed(float walking_speed) { SetWalkingSpeed(walking_speed, AcquireLock()); }
void Creature::SetWalkingSpeed(float walking_speed, boost::unique_lock<boost::mutex>& lock)
{
	walking_speed_ = walking_speed;
	DISPATCH(Creature, WalkingSpeed);
}

float Creature::GetWalkingSpeed() { return GetWalkingSpeed(AcquireLock()); }
float Creature::GetWalkingSpeed(boost::unique_lock<boost::mutex>& lock)
{
    return walking_speed_;
}

void Creature::SetWaterModifierPercent(float water_modifier_percent) { SetWaterModifierPercent(water_modifier_percent, AcquireLock()); }
void Creature::SetWaterModifierPercent(float water_modifier_percent, boost::unique_lock<boost::mutex>& lock)
{
	water_modifier_percent_ = water_modifier_percent;
	DISPATCH(Creature, WaterModifierPercent);
}

float Creature::GetWaterModifierPercent(void) { return GetWaterModifierPercent(AcquireLock()); }
float Creature::GetWaterModifierPercent(boost::unique_lock<boost::mutex>& lock)
{
    return water_modifier_percent_;
}

void Creature::AddMissionCriticalObject(MissionCriticalObject& object) { AddMissionCriticalObject(object, AcquireLock()); }
void Creature::AddMissionCriticalObject(MissionCriticalObject& object, boost::unique_lock<boost::mutex>& lock)
{
	mission_critical_object_list_.Add(object);
	DISPATCH(Creature, MissionCriticalObject);
}

void Creature::RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id) { RemoveMissionCriticalObject(mission_owner, object_id, AcquireLock()); }
void Creature::RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(mission_critical_object_list_), end(mission_critical_object_list_), [=](const MissionCriticalObject& obj)->bool {
        if(mission_owner != obj.mission_owner_id_)
		{
            return false;
		}

        if(object_id != obj.critical_object_id_)
		{
            return false;
		}

        return true;
    });

    if(iter != end(mission_critical_object_list_))
    {
        return;
    }

    mission_critical_object_list_.Remove(iter);
	DISPATCH(Creature, MissionCriticalObject);
}

MissionCriticalObject Creature::GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner) { return GetMissionCriticalObject(object_id, mission_owner, AcquireLock()); }
MissionCriticalObject Creature::GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(mission_critical_object_list_), end(mission_critical_object_list_), [=](const MissionCriticalObject& x)->bool {
        if(x.mission_owner_id_ != mission_owner)
            return false;

        if(x.critical_object_id_ != object_id)
            return false;

        return true;
    });

    if(iter != end(mission_critical_object_list_))
	{
        return *iter;
	}
    else
	{
        return MissionCriticalObject(0, 0);
	}
}

std::list<MissionCriticalObject> Creature::GetMissionCriticalObjects(void) { return GetMissionCriticalObjects(AcquireLock()); }
std::list<MissionCriticalObject> Creature::GetMissionCriticalObjects(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(mission_critical_object_list_.Get());
}

void Creature::SetCombatLevel(uint16_t combat_level) { SetCombatLevel(combat_level, AcquireLock()); }
void Creature::SetCombatLevel(uint16_t combat_level, boost::unique_lock<boost::mutex>& lock)
{
    combat_level_ = combat_level;
	DISPATCH(Creature, CombatLevel);
}

uint16_t Creature::GetCombatLevel() { return GetCombatLevel(AcquireLock()); }
uint16_t Creature::GetCombatLevel()
{
    return combat_level_;
}

void Creature::SetAnimation(std::string animation) { SetAnimation(animation, AcquireLock()); }
void Creature::SetAnimation(std::string animation, boost::unique_lock<boost::mutex>& lock)
{
    animation_ = animation;
	DISPATCH(Creature, Animation);
}

std::string Creature::GetAnimation(void) { return GetAnimation(AcquireLock()); }
std::string Creature::GetAnimation(boost::unique_lock<boost::mutex>& lock)
{
    return animation_;
}

void Creature::SetMoodAnimation(std::string mood_animation) { SetMoodAnimation(mood_animation, AcquireLock()); }
void Creature::SetMoodAnimation(std::string mood_animation, boost::unique_lock<boost::mutex>& lock)
{
    mood_animation_ = mood_animation;
	DISPATCH(Creature, MoodAnimation);
}

std::string Creature::GetMoodAnimation(void) { return GetMoodAnimation(AcquireLock()); }
std::string Creature::GetMoodAnimation(boost::unique_lock<boost::mutex>& lock)
{
    return mood_animation_;
}

void Creature::SetWeaponId(uint64_t weapon_id) { SetWeaponId(weapon_id, AcquireLock()); }
void Creature::SetWeaponId(uint64_t weapon_id, boost::unique_lock<boost::mutex>& lock)
{
    weapon_id_ = weapon_id;
	DISPATCH(Creature, WeaponId);
}

uint64_t Creature::GetWeaponId() { return GetWeaponId(AcquireLock()); }
uint64_t Creature::GetWeaponId(boost::unique_lock<boost::mutex>& lock)
{
    return weapon_id_;
}

void Creature::SetGroupId(uint64_t group_id) { SetGroupId(group_id, AcquireLock()); }
void Creature::SetGroupId(uint64_t group_id, boost::unique_lock<boost::mutex>& lock)
{
    group_id_ = group_id;
    DISPATCH(Creature, GroupId);
}

uint64_t Creature::GetGroupId() { return GetGroupId(AcquireLock()); }
uint64_t Creature::GetGroupId(boost::unique_lock<boost::mutex>& lock)
{
    return group_id_;
}

void Creature::SetInviteSenderId(uint64_t invite_sender_id) { SetInviteSenderId(invite_sender_id, AcquireLock()); }
void Creature::SetInviteSenderId(uint64_t invite_sender_id, boost::unique_lock<boost::mutex>& lock)
{
    invite_sender_id_ = invite_sender_id;
    DISPATCH(Creature, InviteSenderId);
}

uint64_t Creature::GetInviteSenderId() { return GetInviteSenderId(AcquireLock()); }
uint64_t Creature::GetInviteSenderId(boost::unique_lock<boost::mutex>& lock)
{
    return invite_sender_id_;
}

void Creature::SetInviteCounter(uint64_t invite_counter) { SetInviteCounter(invite_counter, AcquireLock()); }
void Creature::SetInviteCounter(uint64_t invite_counter, boost::unique_lock<boost::mutex>& lock)
{
    invite_counter_ = invite_counter;
}

uint64_t Creature::IncrementInviteCounter() { IncrementInviteCounter(AcquireLock()); }
uint64_t Creature::IncrementInviteCounter(boost::unique_lock<boost::mutex>& lock)
{
    return invite_counter_++;
}

uint64_t Creature::GetInviteCounter() const { return GetInviteCounter(AcquireLock()); }
uint64_t Creature::GetInviteCounter(boost::unique_lock<boost::mutex>& lock) const
{
    return invite_counter_;
}

void Creature::SetGuildId(uint32_t guild_id) { SetGuildId(guild_id, AcquireLock()); }
void Creature::SetGuildId(uint32_t guild_id, boost::unique_lock<boost::mutex>& lock)
{
    guild_id_ = guild_id;
	DISPATCH(Creature, GuildId);
}

uint32_t Creature::GetGuildId() { return GetGuildId(AcquireLock()); }
uint32_t Creature::GetGuildId(boost::unique_lock<boost::mutex>& lock)
{
    return guild_id_;
}

void Creature::SetTargetId(uint64_t target_id) { SetTargetId(AcquireLock()); }
void Creature::SetTargetId(uint64_t target_id, boost::unique_lock<boost::mutex>& lock)
{
    target_id_ = target_id;
    DISPATCH(Creature, TargetId);
}

uint64_t Creature::GetTargetId() { return GetTargetId(AcquireLock()); }
uint64_t Creature::GetTargetId(boost::unique_lock<boost::mutex>& lock)
{
    return target_id_;
}

void Creature::SetMoodId(uint8_t mood_id) { SetMoodId(mood_id, AcquireLock()); }
void Creature::SetMoodId(uint8_t mood_id, boost::unique_lock<boost::mutex>& lock)
{
    mood_id_ = mood_id;
    DISPATCH(Creature, MoodId);
}

uint8_t Creature::GetMoodId() { return GetMoodId(AcquireLock()); }
uint8_t Creature::GetMoodId(boost::unique_lock<boost::mutex>& lock)
{
    return mood_id_;
}

void Creature::SetPerformanceId(uint32_t performance_id) { SetPerformanceId(performance_id, AcquireLock()); }
void Creature::SetPerformanceId(uint32_t performance_id, boost::unique_lock<boost::mutex>& lock)
{
    performance_id_ = performance_id;
    DISPATCH(Creature, PerformanceId);
}

uint32_t Creature::GetPerformanceId() { return GetPerformanceId(AcquireLock()); }
uint32_t Creature::GetPerformanceId(boost::unique_lock<boost::mutex>& lock)
{
    return performance_id_;
}

void Creature::SetPerformanceCounter(uint32_t performance_counter) { SetPerformanceCounter(performance_counter, AcquireLock()); }
void Creature::SetPerformanceCounter(uint32_t performance_counter, boost::unique_lock<boost::mutex>& lock)
{
    performance_counter_ = performance_counter;
}

uint32_t Creature::IncrementPerformanceCounter() { IncrementPerformanceCounter(AcquireLock()); }
uint32_t Creature::IncrementPerformanceCounter(boost::unique_lock<boost::mutex>& lock)
{
    return performance_counter_++;
}

uint32_t Creature::GetPerformanceCounter() const {return GetPerformanceCounter(AcquireLock()); }
uint32_t Creature::GetPerformanceCounter(boost::unique_lock<boost::mutex>& lock) const
{
    return performance_counter_;
}

void Creature::SetStatCurrent(StatIndex stat_index, int32_t value) { SetStatCurrent(stat_index, value, AcquireLock()); }
void Creature::SetStatCurrent(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{   
    stat_current_list_.Update(stat_index, Stat(value));
	DISPATCH(Creature, StatCurrent);
}

void Creature::AddStatCurrent(StatIndex stat_index, int32_t value) { AddStatCurrent(stat_index, value, AcquireLock()); }
void Creature::AddStatCurrent(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_value = stat_current_list_[stat_index].value + value;
    stat_current_list_.Update(stat_index, Stat(new_value));
	DISPATCH(Creature, StatCurrent);
}

void Creature::DeductStatCurrent(StatIndex stat_index, int32_t value) { DeductStatCurrent(stat_index, value, AcquireLock()); }
void Creature::DeductStatCurrent(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_current_list_[stat_index].value;
    if (current > value)
    {
        stat_current_list_.Update(stat_index, Stat(current - value));
    }
    else
    {
        stat_current_list_.Update(stat_index, Stat(0));
    }
	DISPATCH(Creature, StatCurrent);
}

std::vector<Stat> Creature::GetCurrentStats(void) { return GetCurrentStats(AcquireLock()); }
std::vector<Stat> Creature::GetCurrentStats(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(stat_current_list_.Get());
}

int32_t Creature::GetStatCurrent(StatIndex stat_index) { return GetStatCurrent(stat_index, AcquireLock()); }
int32_t Creature::GetStatCurrent(StatIndex stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_current_list_.At(stat_index).value;
}

void Creature::SetStatMax(StatIndex stat_index, int32_t value) { SetStatMax(stat_index, value, AcquireLock()); }
void Creature::SetStatMax(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
	stat_max_list_.Update(stat_index, Stat(value));
	DISPATCH(Creature, StatMax);
}

void Creature::AddStatMax(StatIndex stat_index, int32_t value) { AddStatMax(stat_index, value, AcquireLock()); }
void Creature::AddStatMax(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_max_list_.Update(stat_index, Stat(stat_max_list_.At(stat_index).value + value));
	DISPATCH(Creature, StatMax);
}

void Creature::DeductStatMax(StatIndex stat_index, int32_t value) { DeductStatMax(stat_index, value, AcquireLock()); }
void Creature::DeductStatMax(StatIndex stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_max_list_[stat_index].value;
    if (current > value)
    {
        stat_max_list_.Update(stat_index, Stat(current - value));
    }
    else
    {
        stat_max_list_.Update(stat_index, Stat(0));
    }
	DISPATCH(Creature, StatMax);
}

std::vector<Stat> Creature::GetMaxStats(void) { return GetMaxStats(AcquireLock()); }
std::vector<Stat> Creature::GetMaxStats(boost::unique_lock<boost::mutex>& lock)
{
    return std::move(stat_max_list_.Get());
}

int32_t Creature::GetStatMax(StatIndex stat_index) { return GetStatMax(stat_index, AcquireLock()); }
int32_t Creature::GetStatMax(StatIndex stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_max_list_.At(stat_index).value;
}

void Creature::AddEquipmentItem(EquipmentItem& item) { AddEquipmentItem(item, AcquireLock()); }
void Creature::AddEquipmentItem(EquipmentItem& item, boost::unique_lock<boost::mutex>& lock)
{
    equipment_list_.Add(item);
	DISPATCH(Creature, EquipmentItem);
}

void Creature::RemoveEquipmentItem(uint64_t object_id) { RemoveEquipmentItem(object_id, AcquireLock()); }
void Creature::RemoveEquipmentItem(uint64_t object_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(equipment_list_), end(equipment_list_), [=](std::pair<uint16_t, EquipmentItem> item)->bool {
        return (object_id == item.second.object_id);
    });

    if(iter != end(equipment_list_))
    {
        return;
    }
    equipment_list_.Remove(iter);
   
	DISPATCH(Creature, EquipmentItem);
}

void Creature::UpdateEquipmentItem(EquipmentItem& item) { UpdateEquipmentItem(item, AcquireLock()); }
void Creature::UpdateEquipmentItem(EquipmentItem& item, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = equipment_list_.Find(item);
    if(iter != end(equipment_list_))
	{
        equipment_list_.Update(iter->first, item);
	}
	DISPATCH(Creature, EquipmentItem);
}

std::list<EquipmentItem> Creature::GetEquipment(void) { return GetEquipment(AcquireLock()); }
std::list<EquipmentItem> Creature::GetEquipment(boost::unique_lock<boost::mutex>& lock)
{
	std::list<EquipmentItem> result;
	for(auto& v : equipment_list_)
	{
		result.push_back(v.second);
	}
    return std::move(result);
}

EquipmentItem Creature::GetEquipmentItem(uint64_t object_id) { return GetEquipmentItem(object_id, AcquireLock()); }
EquipmentItem Creature::GetEquipmentItem(uint64_t object_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(equipment_list_), end(equipment_list_), [=](std::pair<uint16_t, EquipmentItem> pair) {
        return pair.second.object_id == object_id;
    });

    if(iter != end(equipment_list_))
	{
        return iter->second;
	}
    else
	{
        return EquipmentItem();
	}
}

void Creature::SetDisguise(std::string disguise) { SetDisguise(disguise, AcquireLock()); }
void Creature::SetDisguise(std::string disguise, boost::unique_lock<boost::mutex>& lock)
{
	disguise_ = disguise;
    DISPATCH(Creature, Disguise);
}

std::string Creature::GetDisguise(void) { return GetDisguise(AcquireLock()); }
std::string Creature::GetDisguise(boost::unique_lock<boost::mutex>& lock)
{
    return disguise_;
}

void Creature::SetStationary(bool stationary) { SetStationary(stationary, AcquireLock()); }
void Creature::SetStationary(bool stationary, boost::unique_lock<boost::mutex>& lock)
{
    stationary_ = stationary;
    DISPATCH(Creature, Stationary);
}

bool Creature::IsStationary() { return IsStationary(AcquireLock()); }
bool Creature::IsStationary(boost::unique_lock<boost::mutex>& lock)
{
    return stationary_;
}

PvpStatus Creature::GetPvpStatus() const { return GetPvpStatus(AcquireLock()); }
PvpStatus Creature::GetPvpStatus(boost::unique_lock<boost::mutex>& lock) const
{
    return pvp_status_;
}

void Creature::SetPvPStatus(PvpStatus status) { SetPvPStatus(status, AcquireLock()); }
void Creature::SetPvPStatus(PvpStatus status, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = status;
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpStateOn(PvpStatus state) { TogglePvpStateOn(state, AcquireLock()); }
void Creature::TogglePvpStateOn(PvpStatus state, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = static_cast<PvpStatus>(pvp_status_ | state);
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpStateOff(PvpStatus state) { TogglePvpStateOff(state, AcquireLock()); }
void Creature::TogglePvpStateOff(PvpStatus state, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = static_cast<PvpStatus>(pvp_status_ & ~state);
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpState(PvpStatus state) { return TogglePvpState(state, AcquireLock()); }
void Creature::TogglePvpState(PvpStatus state, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = static_cast<PvpStatus>(pvp_status_ ^ state);
	DISPATCH(Creature, PvPStatus);
}

bool Creature::CheckPvpState(PvpStatus state) const { return CheckPvpState(state, AcquireLock()); }
bool Creature::CheckPvpState(PvpStatus state, boost::unique_lock<boost::mutex>& lock) const
{
    return static_cast<PvpStatus>(pvp_status_ & state) == state;
}

bool Creature::CanAttack(Creature* creature)
{
    if (creature->CheckPvpState(PvPStatus_Attackable, lock) || creature->CheckPvpState(PvPStatus_Enemy))
    {
       return true;
    }
    
	if (creature->CheckPvpState(PvPStatus_Overt) || creature->CheckPvpState(PvPStatus_Tef))
    {
        if (CheckPvpState(PvPStatus_Overt) || CheckPvpState(PvPStatus_Tef))
		{
            return true;
		}
    }
	
	if (creature->CheckPvpState(PvPStatus_Duel) && InDuelList(creature->GetObjectId()) && creature->InDuelList(GetObjectId()))
	{
        return true;
	}

    return false;
}

void Creature::AddToDuelList(uint64_t id) { AddToDuelList(id, AcquireLock()); }
void Creature::AddToDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock)
{
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    
	if (found == end(duel_list_))
	{
        duel_list_.push_back(id);
	}
}

void Creature::RemoveFromDuelList(uint64_t id) { RemoveFromDuelList(id, AcquireLock()); }
void Creature::RemoveFromDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock)
{
    auto lock = AcquireLock();
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    
	if (found != end(duel_list_))
	{
        duel_list_.erase(found);
	}
}

bool Creature::InDuelList(uint64_t id) { return InDuelList(id, AcquireLock()); }
bool Creature::InDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock)
{
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    return found != end(duel_list_);

}
std::vector<uint64_t> Creature::GetDuelList() { return GetDuelList(AcquireLock()); }
std::vector<uint64_t> Creature::GetDuelList(boost::unique_lock<boost::mutex>& lock)
{
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
	auto lock = AcquireLock();
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
		auto lock = AcquireLock();
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
		auto lock = AcquireLock();
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
		auto lock = AcquireLock();

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
		auto lock = AcquireLock();
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
	auto lock = AcquireLock();
	for_each(buffs_.begin(), buffs_.end(), functor);
}

void Creature::CleanUpBuffs()
{
	auto lock = AcquireLock();
	buffs_.clear();
}

void Creature::SerializeBaseStats(swganh::messages::BaseSwgMessage* message) { SerializeBaseStats(message, AcquireLock()); }
void Creature::SerializeBaseStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	auto lock = AcquireLock();
	stat_base_list_.Serialize(message);
}

void Creature::SerializeSkills(swganh::messages::BaseSwgMessage* message) { SerializeSkills(message, AcquireLock()); }
void Creature::SerializeSkills(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	skills_.Serialize(message);
}

void Creature::SerializeStatWounds(swganh::messages::BaseSwgMessage* message) { SerializeStatWounds(message, AcquireLock()); }
void Creature::SerializeStatWounds(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_wound_list_.Serialize(message);
}

void Creature::SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message) { SerializeStatEncumberances(message, AcquireLock()); }
void Creature::SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_encumberance_list_.Serialize(message);
}

void Creature::SerializeSkillMods(swganh::messages::BaseSwgMessage* message) { SerializeSkillMods(message, AcquireLock()); }
void Creature::SerializeSkillMods(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	skill_mod_list_.Serialize(message);
}

void Creature::SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message) { SerializeMissionCriticalObjects(message, AcquireLock()); }
void Creature::SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	mission_critical_object_list_.Serialize(message);
}

void Creature::SerializeCurrentStats(swganh::messages::BaseSwgMessage* message) { SerializeCurrentStats(message, AcquireLock()); }
void Creature::SerializeCurrentStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_current_list_.Serialize(message);
}

void Creature::SerializeMaxStats(swganh::messages::BaseSwgMessage* message) { SerializeMaxStats(message, AcquireLock()); }
void Creature::SerializeMaxStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_max_list_.Serialize(message);
}

void Creature::SerializeEquipment(swganh::messages::BaseSwgMessage* message) { SerializeEquipment(message, AcquireLock()); }
void Creature::SerializeEquipment(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	equipment_list_.Serialize(message);
}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "creature.h"

#include "swganh/crc.h"

#include "swganh_core/object/object_events.h"
#include "creature_message_builder.h"

#include "swganh_core/combat/buff_interface.h"
#include "swganh_core/combat/buff_events.h"
#include "swganh_core/messages/controllers/add_buff.h"
#include "swganh_core/messages/controllers/remove_buff.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::containers;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

Creature::Creature()
: Tangible()
, bank_credits_(0)
, cash_credits_(0)
, stat_base_list_(9)
, posture_(0)
, faction_rank_(0)
, owner_id_(0)
, scale_(1.0f)
, battle_fatigue_(0)
, state_bitmask_(0)
, stat_wound_list_(9)
, acceleration_multiplier_base_(1.0f)
, acceleration_multiplier_modifier_(1.0f)
, stat_encumberance_list_(9)
, speed_multiplier_base_(1.0f)
, speed_multiplier_modifier_(1.0f)
, listen_to_id_(0)
, run_speed_(5.0f)
, slope_modifier_angle_(1.02f)
, slope_modifier_percent_(0)
, turn_radius_(1.0f)
, walking_speed_(1.0f)
, water_modifier_percent_(0.0125f)
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
, stat_current_list_(9)
, stat_max_list_(9)
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

void Creature::SetBankCredits(uint32_t bank_credits) {
    auto lock = AcquireLock();
    SetBankCredits(bank_credits, lock);
}

void Creature::SetBankCredits(uint32_t bank_credits, boost::unique_lock<boost::mutex>& lock)
{
    bank_credits_ = bank_credits;
	DISPATCH(Creature, Bank);
}

uint32_t Creature::GetBankCredits() {
    auto lock = AcquireLock();
    return GetBankCredits(lock);
}

uint32_t Creature::GetBankCredits(boost::unique_lock<boost::mutex>& lock)
{
    return bank_credits_;
}

void Creature::SetCashCredits(uint32_t cash_credits) {
    auto lock = AcquireLock();
    SetCashCredits(cash_credits, lock);
}

void Creature::SetCashCredits(uint32_t cash_credits, boost::unique_lock<boost::mutex>& lock)
{
    cash_credits_ = cash_credits;
	DISPATCH(Creature, Cash);
}

uint32_t Creature::GetCashCredits() {
    auto lock = AcquireLock();
    return GetCashCredits(lock);
}

uint32_t Creature::GetCashCredits(boost::unique_lock<boost::mutex>& lock)
{
    return cash_credits_;
}

void Creature::SetAllStats(int32_t value) {
    auto lock = AcquireLock();
    SetAllStats(value, lock);
}

void Creature::SetAllStats(int32_t value, boost::unique_lock<boost::mutex>& lock)
{
	stat_base_list_.update(HEALTH, value);
	stat_base_list_.update(ACTION, value);
	stat_base_list_.update(MIND, value);
	stat_current_list_.update(HEALTH, value);
	stat_current_list_.update(ACTION, value);
	stat_current_list_.update(MIND, value);
	stat_max_list_.update(HEALTH, value);
	stat_max_list_.update(ACTION, value);
	stat_max_list_.update(MIND, value);

	DISPATCH(Creature, StatBase);
	DISPATCH(Creature, StatCurrent);
	DISPATCH(Creature, StatMax);
}

void Creature::SetStatBase(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    SetStatBase(stat_index, value, lock);
}

void Creature::SetStatBase(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock) 
{
	stat_base_list_.update(stat_index, value);
	DISPATCH(Creature, StatBase);
}

void Creature::AddStatBase(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    AddStatBase(stat_index, value, lock);
}

void Creature::AddStatBase(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    uint32_t new_stat = stat_base_list_[stat_index] + value;
    stat_base_list_.update(stat_index, new_stat);
	DISPATCH(Creature, StatBase);
}

void Creature::DeductStatBase(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    DeductStatBase(stat_index, value, lock);
}

void Creature::DeductStatBase(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_base_list_[stat_index];
	stat_base_list_.update(stat_index, (current > value) ? current - value : 0);
	DISPATCH(Creature, StatBase);
}

std::vector<int32_t> Creature::GetBaseStats() {
    auto lock = AcquireLock();
    return GetBaseStats(lock);
}

std::vector<int32_t> Creature::GetBaseStats(boost::unique_lock<boost::mutex>& lock)
{
    return stat_base_list_.raw();
}

int32_t Creature::GetStatBase(uint16_t stat_index) {
    auto lock = AcquireLock();
    return GetStatBase(stat_index, lock);
}

int32_t Creature::GetStatBase(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_base_list_[stat_index];
}

void Creature::AddSkill(std::string skill) {
    auto lock = AcquireLock();
    AddSkill(skill, lock);
}

void Creature::AddSkill(std::string skill, boost::unique_lock<boost::mutex>& lock)
{
    skills_.add(skill);
	DISPATCH(Creature, Skill);
}

void Creature::RemoveSkill(std::string skill) {
    auto lock = AcquireLock();
    RemoveSkill(skill, lock);
}

void Creature::RemoveSkill(std::string skill, boost::unique_lock<boost::mutex>& lock)
{
    skills_.remove(skill);
	DISPATCH(Creature, Skill);
}

std::set<std::string> Creature::GetSkills() {
    auto lock = AcquireLock();
    return GetSkills(lock);
}

std::set<std::string> Creature::GetSkills(boost::unique_lock<boost::mutex>& lock)
{
    return skills_.raw();
}

bool Creature::HasSkill(std::string skill) {
    auto lock = AcquireLock();
    return HasSkill(skill, lock);
}

bool Creature::HasSkill(std::string skill, boost::unique_lock<boost::mutex>& lock)
{
    return skills_.contains(skill);
}

std::map<uint32_t, std::string>  Creature::GetSkillCommands() {
    auto lock = AcquireLock();
    return GetSkillCommands(lock);
}

std::map<uint32_t, std::string>  Creature::GetSkillCommands(boost::unique_lock<boost::mutex>& lock)
{
    return skill_commands_;
}


bool  Creature::HasSkillCommand(std::string skill_command) {
    auto lock = AcquireLock();
    return HasSkillCommand(skill_command, lock);
}

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


void  Creature::AddSkillCommand(std::pair<uint32_t, std::string> skill_command) {
    auto lock = AcquireLock();
    AddSkillCommand(skill_command, lock);
}

void  Creature::AddSkillCommand(std::pair<uint32_t, std::string> skill_command, boost::unique_lock<boost::mutex>& lock)
{
    auto find_it = skill_commands_.find(skill_command.first);

    if (find_it == end(skill_commands_))
	{
        skill_commands_.insert(skill_command);
	}
}

void  Creature::RemoveSkillCommand(std::string skill_command) {
    auto lock = AcquireLock();
    RemoveSkillCommand(skill_command, lock);
}

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

void Creature::SetPosture(Posture posture) {
    auto lock = AcquireLock();
    SetPosture(posture, lock);
}

void Creature::SetPosture(Posture posture, boost::unique_lock<boost::mutex>& lock)
{
    posture_ = posture;
	DISPATCH(Creature, Posture);
}

Posture Creature::GetPosture() {
    auto lock = AcquireLock();
    return GetPosture(lock);
}

Posture Creature::GetPosture(boost::unique_lock<boost::mutex>& lock)
{
    uint32_t posture = posture_;
    return (Posture)posture;
}

bool Creature::IsDead() {
    auto lock = AcquireLock();
    return IsDead(lock);
}

bool Creature::IsDead(boost::unique_lock<boost::mutex>& lock)
{
    return posture_ == DEAD;
}

bool Creature::IsIncapacitated() {
    auto lock = AcquireLock();
    return IsIncapacitated(lock);
}

bool Creature::IsIncapacitated(boost::unique_lock<boost::mutex>& lock)
{
    return posture_ == INCAPACITATED;
}

void Creature::SetFactionRank(uint8_t faction_rank) {
    auto lock = AcquireLock();
    SetFactionRank(faction_rank, lock);
}

void Creature::SetFactionRank(uint8_t faction_rank, boost::unique_lock<boost::mutex>& lock)
{
    faction_rank_ = faction_rank;
	DISPATCH(Creature, FactionRank);
}

uint8_t Creature::GetFactionRank() {
    auto lock = AcquireLock();
    return GetFactionRank(lock);
}

uint8_t Creature::GetFactionRank(boost::unique_lock<boost::mutex>& lock)
{
    return faction_rank_;
}

void Creature::SetOwnerId(uint64_t owner_id) {
    auto lock = AcquireLock();
    SetOwnerId(owner_id, lock);
}

void Creature::SetOwnerId(uint64_t owner_id, boost::unique_lock<boost::mutex>& lock)
{
    owner_id_ = owner_id;
	DISPATCH(Creature, OwnerId);
}

uint64_t Creature::GetOwnerId(void) {
    auto lock = AcquireLock();
    return GetOwnerId(lock);
}

uint64_t Creature::GetOwnerId(boost::unique_lock<boost::mutex>& lock)
{
    return owner_id_;
}

void Creature::SetScale(float scale) {
    auto lock = AcquireLock();
    SetScale(scale, lock);
}

void Creature::SetScale(float scale, boost::unique_lock<boost::mutex>& lock)
{
	scale_ = scale;
	DISPATCH(Creature, Scale);
}

float Creature::GetScale() {
    auto lock = AcquireLock();
    return GetScale(lock);
}

float Creature::GetScale(boost::unique_lock<boost::mutex>& lock)
{
    return scale_;
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue) {
    auto lock = AcquireLock();
    SetBattleFatigue(battle_fatigue, lock);
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue, boost::unique_lock<boost::mutex>& lock)
{
    battle_fatigue_ = battle_fatigue;
    DISPATCH(Creature, BattleFatigue);
}

void Creature::AddBattleFatigue(uint32_t battle_fatigue) {
    auto lock = AcquireLock();
    AddBattleFatigue(battle_fatigue, lock);
}

void Creature::AddBattleFatigue(uint32_t battle_fatigue, boost::unique_lock<boost::mutex>& lock)
{
    battle_fatigue += battle_fatigue;
	DISPATCH(Creature, BattleFatigue);
}

uint32_t Creature::GetBattleFatigue() {
    auto lock = AcquireLock();
    return GetBattleFatigue(lock);
}

uint32_t Creature::GetBattleFatigue(boost::unique_lock<boost::mutex>& lock)
{
    return battle_fatigue_;
}

void Creature::SetStateBitmask(uint64_t state_bitmask) {
    auto lock = AcquireLock();
    SetStateBitmask(state_bitmask, lock);
}

void Creature::SetStateBitmask(uint64_t state_bitmask, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = state_bitmask;
	DISPATCH(Creature, StateBitmask);
}

uint64_t Creature::GetStateBitmask() {
    auto lock = AcquireLock();
    return GetStateBitmask(lock);
}

uint64_t Creature::GetStateBitmask(boost::unique_lock<boost::mutex>& lock)
{
    return state_bitmask_;
}

bool Creature::HasState(uint64_t state) {
    auto lock = AcquireLock();
    return HasState(state, lock);
}

bool Creature::HasState(uint64_t state, boost::unique_lock<boost::mutex>& lock)
{
    return state == (state & state_bitmask_);
}

void Creature::ToggleStateOn(uint64_t state) {
    auto lock = AcquireLock();
    ToggleStateOn(state, lock);
}

void Creature::ToggleStateOn(uint64_t state, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = ( state_bitmask_ | state);
	DISPATCH(Creature, StateBitmask);
}

void Creature::ToggleStateOff(uint64_t state) {
    auto lock = AcquireLock();
    ToggleStateOff(state, lock);
}

void Creature::ToggleStateOff(uint64_t state, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = ( state_bitmask_ & ~ state);
	DISPATCH(Creature, StateBitmask);
}

void Creature::ToggleStateBitmask(uint64_t state_bitmask) {
    auto lock = AcquireLock();
    ToggleStateBitmask(state_bitmask, lock);
}

void Creature::ToggleStateBitmask(uint64_t state_bitmask, boost::unique_lock<boost::mutex>& lock)
{
    state_bitmask_ = (state_bitmask_ ^ state_bitmask);
	DISPATCH(Creature, StateBitmask);
}

void Creature::SetStatWound(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    SetStatWound(stat_index, value, lock);
}

void Creature::SetStatWound(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_wound_list_.update(stat_index, value);
	DISPATCH(Creature, StatWound);
}

void Creature::AddStatWound(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    AddStatWound(stat_index, value, lock);
}

void Creature::AddStatWound(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_wound_list_.update(stat_index, stat_wound_list_[stat_index] + value);
	DISPATCH(Creature, StatWound);
}

void Creature::DeductStatWound(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    DeductStatWound(stat_index, value, lock);
}

void Creature::DeductStatWound(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_wound_list_[stat_index];
	stat_wound_list_.update(stat_index, (current > value) ? current - value : 0);
	DISPATCH(Creature, StatWound);
}

std::vector<int32_t> Creature::GetStatWounds() {
    auto lock = AcquireLock();
    return GetStatWounds(lock);
}

std::vector<int32_t> Creature::GetStatWounds(boost::unique_lock<boost::mutex>& lock)
{
    return stat_wound_list_.raw();
}

int32_t Creature::GetStatWound(uint16_t stat_index) {
    auto lock = AcquireLock();
    return GetStatWound(stat_index, lock);
}

int32_t Creature::GetStatWound(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_wound_list_[stat_index];
}

void Creature::SetAccelerationMultiplierBase(float acceleration_multiplier_bas) {
    auto lock = AcquireLock();
    SetAccelerationMultiplierBase(acceleration_multiplier_bas, lock);
}

void Creature::SetAccelerationMultiplierBase(float acceleration_multiplier_bas, boost::unique_lock<boost::mutex>& lock)
{
    acceleration_multiplier_base_ = acceleration_multiplier_bas;
	DISPATCH(Creature, AccelerationMultiplierBase);
}

float Creature::GetAccelerationMultiplierBase() {
    auto lock = AcquireLock();
    return GetAccelerationMultiplierBase(lock);
}

float Creature::GetAccelerationMultiplierBase(boost::unique_lock<boost::mutex>& lock)
{
    return acceleration_multiplier_base_;
}

void Creature::SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier) {
    auto lock = AcquireLock();
    SetAccelerationMultiplierModifier(acceleration_multiplier_modifier, lock);
}

void Creature::SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier, boost::unique_lock<boost::mutex>& lock)
{
    acceleration_multiplier_modifier_ = acceleration_multiplier_modifier;
	DISPATCH(Creature, AccelerationMultiplierModifier);
}

float Creature::GetAccelerationMultiplierModifier() {
    auto lock = AcquireLock();
    return GetAccelerationMultiplierModifier(lock);
}

float Creature::GetAccelerationMultiplierModifier(boost::unique_lock<boost::mutex>& lock)
{
    return acceleration_multiplier_modifier_;
}

void Creature::SetStatEncumberance(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    SetStatEncumberance(stat_index, value, lock);
}

void Creature::SetStatEncumberance(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_encumberance_list_.update(stat_index, value);
	DISPATCH(Creature, StatEncumberance);
}

void Creature::AddStatEncumberance(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    AddStatEncumberance(stat_index, value, lock);
}

void Creature::AddStatEncumberance(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_stat = stat_encumberance_list_[stat_index] + value;
    stat_encumberance_list_.update(stat_index, new_stat);
	DISPATCH(Creature, StatEncumberance);
}

void Creature::DeductStatEncumberance(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    DeductStatEncumberance(stat_index, value, lock);
}

void Creature::DeductStatEncumberance(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_encumberance_list_[stat_index];
    stat_encumberance_list_.update(stat_index, (current > value) ? (current - value) : 0);
	DISPATCH(Creature, StatEncumberance);
}

std::vector<int32_t> Creature::GetStatEncumberances() {
    auto lock = AcquireLock();
    return GetStatEncumberances(lock);
}

std::vector<int32_t> Creature::GetStatEncumberances(boost::unique_lock<boost::mutex>& lock)
{
    return stat_encumberance_list_.raw();
}

int32_t Creature::GetStatEncumberance(uint16_t stat_index) {
    auto lock = AcquireLock();
    return GetStatEncumberance(stat_index, lock);
}

int32_t Creature::GetStatEncumberance(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_encumberance_list_[stat_index];
}

void Creature::AddSkillMod(SkillMod mod) {
    auto lock = AcquireLock();
    AddSkillMod(mod, lock);
}

void Creature::AddSkillMod(SkillMod mod, boost::unique_lock<boost::mutex>& lock)
{
    skill_mod_list_.add(mod.identifier, mod);
	DISPATCH(Creature, SkillMod);
}

void Creature::RemoveSkillMod(std::string identifier) {
    auto lock = AcquireLock();
    RemoveSkillMod(identifier, lock);
}

void Creature::RemoveSkillMod(std::string identifier, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(skill_mod_list_), end(skill_mod_list_), [=](std::pair<std::string, SkillMod> pair)->bool {
        return (identifier == pair.first);
    });

    if(iter != end(skill_mod_list_))
    {
        return;
    }

    skill_mod_list_.remove(iter);
	DISPATCH(Creature, SkillMod);
}

void Creature::SetSkillMod(SkillMod mod) {
    auto lock = AcquireLock();
    SetSkillMod(mod, lock);
}

void Creature::SetSkillMod(SkillMod mod, boost::unique_lock<boost::mutex>& lock)
{
    skill_mod_list_.update(mod.identifier);
	DISPATCH(Creature, SkillMod);
}

std::map<std::string, SkillMod> Creature::GetSkillMods() {
    auto lock = AcquireLock();
    return GetSkillMods(lock);
}

std::map<std::string, SkillMod> Creature::GetSkillMods(boost::unique_lock<boost::mutex>& lock)
{
    return skill_mod_list_.raw();
}

SkillMod Creature::GetSkillMod(std::string identifier) {
    auto lock = AcquireLock();
    return GetSkillMod(identifier, lock);
}

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
        throw SkillModNotFound();
	}
}

void Creature::SetSpeedMultiplierBase(float speed_multiplier_base) {
    auto lock = AcquireLock();
    SetSpeedMultiplierBase(speed_multiplier_base, lock);
}

void Creature::SetSpeedMultiplierBase(float speed_multiplier_base, boost::unique_lock<boost::mutex>& lock)
{
	speed_multiplier_base_ = speed_multiplier_base;
	DISPATCH(Creature, SpeedMultiplierBase);
}

float Creature::GetSpeedMultiplierBase() {
    auto lock = AcquireLock();
    return GetSpeedMultiplierBase(lock);
}

float Creature::GetSpeedMultiplierBase(boost::unique_lock<boost::mutex>& lock)
{
    return speed_multiplier_base_;
}

void Creature::SetSpeedMultiplierModifier(float speed_multiplier_modifier) {
    auto lock = AcquireLock();
    SetSpeedMultiplierModifier(speed_multiplier_modifier, lock);
}

void Creature::SetSpeedMultiplierModifier(float speed_multiplier_modifier, boost::unique_lock<boost::mutex>& lock)
{
    speed_multiplier_modifier_ = speed_multiplier_modifier;
	DISPATCH(Creature, SpeedMultiplierModifer);
}

float Creature::GetSpeedMultiplierModifier() {
    auto lock = AcquireLock();
    return GetSpeedMultiplierModifier(lock);
}

float Creature::GetSpeedMultiplierModifier(boost::unique_lock<boost::mutex>& lock)
{
    return speed_multiplier_modifier_;
}

void Creature::SetListenToId(uint64_t listen_to_id) {
    auto lock = AcquireLock();
    SetListenToId(listen_to_id, lock);
}

void Creature::SetListenToId(uint64_t listen_to_id, boost::unique_lock<boost::mutex>& lock)
{
    listen_to_id_ = listen_to_id;
	DISPATCH(Creature, ListenToId);
}

uint64_t Creature::GetListenToId() {
    auto lock = AcquireLock();
    return GetListenToId(lock);
}

uint64_t Creature::GetListenToId(boost::unique_lock<boost::mutex>& lock)
{
    return listen_to_id_;
}

void Creature::SetRunSpeed(float run_speed) {
    auto lock = AcquireLock();
    SetRunSpeed(run_speed, lock);
}

void Creature::SetRunSpeed(float run_speed, boost::unique_lock<boost::mutex>& lock)
{
	run_speed_ = run_speed;
	DISPATCH(Creature, RunSpeed);
}

float Creature::GetRunSpeed() {
    auto lock = AcquireLock();
    return GetRunSpeed(lock);
}

float Creature::GetRunSpeed(boost::unique_lock<boost::mutex>& lock)
{
    return run_speed_;
}

void Creature::SetSlopeModifierAngle(float slope_modifier_angle) {
    auto lock = AcquireLock();
    SetSlopeModifierAngle(slope_modifier_angle, lock);
}

void Creature::SetSlopeModifierAngle(float slope_modifier_angle, boost::unique_lock<boost::mutex>& lock)
{
	slope_modifier_angle_ = slope_modifier_angle;
	DISPATCH(Creature, SlopeModifierAngle);
}

float Creature::GetSlopeModifierAngle() {
    auto lock = AcquireLock();
    return GetSlopeModifierAngle(lock);
}

float Creature::GetSlopeModifierAngle(boost::unique_lock<boost::mutex>& lock)
{
    return slope_modifier_angle_;
}

void Creature::SetSlopeModifierPercent(float slope_modifier_percent) {
    auto lock = AcquireLock();
    SetSlopeModifierPercent(slope_modifier_percent, lock);
}

void Creature::SetSlopeModifierPercent(float slope_modifier_percent, boost::unique_lock<boost::mutex>& lock)
{
    slope_modifier_percent_ = slope_modifier_percent;
	DISPATCH(Creature, SlopeModifierPercent);
}

float Creature::GetSlopeModifierPercent() {
    auto lock = AcquireLock();
    return GetSlopeModifierPercent(lock);
}

float Creature::GetSlopeModifierPercent(boost::unique_lock<boost::mutex>& lock)
{
    return slope_modifier_percent_;
}

void Creature::SetTurnRadius(float turn_radius) {
    auto lock = AcquireLock();
    SetTurnRadius(turn_radius, lock);
}

void Creature::SetTurnRadius(float turn_radius, boost::unique_lock<boost::mutex>& lock)
{
	turn_radius_ = turn_radius;
	DISPATCH(Creature, TurnRadius);
}

float Creature::GetTurnRadius() {
    auto lock = AcquireLock();
    return GetTurnRadius(lock);
}

float Creature::GetTurnRadius(boost::unique_lock<boost::mutex>& lock)
{
    return turn_radius_;
}

void Creature::SetWalkingSpeed(float walking_speed) {
    auto lock = AcquireLock();
    SetWalkingSpeed(walking_speed, lock);
}

void Creature::SetWalkingSpeed(float walking_speed, boost::unique_lock<boost::mutex>& lock)
{
	walking_speed_ = walking_speed;
	DISPATCH(Creature, WalkingSpeed);
}

float Creature::GetWalkingSpeed() {
    auto lock = AcquireLock();
    return GetWalkingSpeed(lock);
}

float Creature::GetWalkingSpeed(boost::unique_lock<boost::mutex>& lock)
{
    return walking_speed_;
}

void Creature::SetWaterModifierPercent(float water_modifier_percent) {
    auto lock = AcquireLock();
    SetWaterModifierPercent(water_modifier_percent, lock);
}

void Creature::SetWaterModifierPercent(float water_modifier_percent, boost::unique_lock<boost::mutex>& lock)
{
	water_modifier_percent_ = water_modifier_percent;
	DISPATCH(Creature, WaterModifierPercent);
}

float Creature::GetWaterModifierPercent(void) {
    auto lock = AcquireLock();
    return GetWaterModifierPercent(lock);
}

float Creature::GetWaterModifierPercent(boost::unique_lock<boost::mutex>& lock)
{
    return water_modifier_percent_;
}

void Creature::AddMissionCriticalObject(MissionCriticalObject& object) {
    auto lock = AcquireLock();
    AddMissionCriticalObject(object, lock);
}

void Creature::AddMissionCriticalObject(MissionCriticalObject& object, boost::unique_lock<boost::mutex>& lock)
{
	mission_critical_object_list_.add(object);
	DISPATCH(Creature, MissionCriticalObject);
}

void Creature::RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id) {
    auto lock = AcquireLock();
    RemoveMissionCriticalObject(mission_owner, object_id, lock);
}

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

    mission_critical_object_list_.remove(iter);
	DISPATCH(Creature, MissionCriticalObject);
}

MissionCriticalObject Creature::GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner) {
    auto lock = AcquireLock();
    return GetMissionCriticalObject(object_id, mission_owner, lock);
}

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

std::set<MissionCriticalObject> Creature::GetMissionCriticalObjects(void) {
    auto lock = AcquireLock();
    return GetMissionCriticalObjects(lock);
}

std::set<MissionCriticalObject> Creature::GetMissionCriticalObjects(boost::unique_lock<boost::mutex>& lock)
{
    return mission_critical_object_list_.raw();
}

void Creature::SetCombatLevel(uint16_t combat_level) {
    auto lock = AcquireLock();
    SetCombatLevel(combat_level, lock);
}

void Creature::SetCombatLevel(uint16_t combat_level, boost::unique_lock<boost::mutex>& lock)
{
    combat_level_ = combat_level;
	DISPATCH(Creature, CombatLevel);
}

uint16_t Creature::GetCombatLevel() {
    auto lock = AcquireLock();
    return GetCombatLevel(lock);
}

uint16_t Creature::GetCombatLevel(boost::unique_lock<boost::mutex>& lock)
{
    return combat_level_;
}

void Creature::SetAnimation(std::string animation) {
    auto lock = AcquireLock();
    SetAnimation(animation, lock);
}

void Creature::SetAnimation(std::string animation, boost::unique_lock<boost::mutex>& lock)
{
    animation_ = animation;
	DISPATCH(Creature, Animation);
}

std::string Creature::GetAnimation(void) {
    auto lock = AcquireLock();
    return GetAnimation(lock);
}

std::string Creature::GetAnimation(boost::unique_lock<boost::mutex>& lock)
{
    return animation_;
}

void Creature::SetMoodAnimation(std::string mood_animation) {
    auto lock = AcquireLock();
    SetMoodAnimation(mood_animation, lock);
}

void Creature::SetMoodAnimation(std::string mood_animation, boost::unique_lock<boost::mutex>& lock)
{
    mood_animation_ = mood_animation;
	DISPATCH(Creature, MoodAnimation);
}

std::string Creature::GetMoodAnimation(void) {
    auto lock = AcquireLock();
    return GetMoodAnimation(lock);
}

std::string Creature::GetMoodAnimation(boost::unique_lock<boost::mutex>& lock)
{
    return mood_animation_;
}

void Creature::SetWeaponId(uint64_t weapon_id) {
    auto lock = AcquireLock();
    SetWeaponId(weapon_id, lock);
}

void Creature::SetWeaponId(uint64_t weapon_id, boost::unique_lock<boost::mutex>& lock)
{
    weapon_id_ = weapon_id;
	DISPATCH(Creature, WeaponId);
}

uint64_t Creature::GetWeaponId() {
    auto lock = AcquireLock();
    return GetWeaponId(lock);
}

uint64_t Creature::GetWeaponId(boost::unique_lock<boost::mutex>& lock)
{
    return weapon_id_;
}

void Creature::SetGroupId(uint64_t group_id) {
    auto lock = AcquireLock();
    SetGroupId(group_id, lock);
}

void Creature::SetGroupId(uint64_t group_id, boost::unique_lock<boost::mutex>& lock)
{
    group_id_ = group_id;
    DISPATCH(Creature, GroupId);
}

uint64_t Creature::GetGroupId() {
    auto lock = AcquireLock();
    return GetGroupId(lock);
}

uint64_t Creature::GetGroupId(boost::unique_lock<boost::mutex>& lock)
{
    return group_id_;
}

void Creature::SetInviteSenderId(uint64_t invite_sender_id) {
    auto lock = AcquireLock();
    SetInviteSenderId(invite_sender_id, lock);
}

void Creature::SetInviteSenderId(uint64_t invite_sender_id, boost::unique_lock<boost::mutex>& lock)
{
    invite_sender_id_ = invite_sender_id;
    DISPATCH(Creature, InviteSenderId);
}

uint64_t Creature::GetInviteSenderId() {
    auto lock = AcquireLock();
    return GetInviteSenderId(lock);
}

uint64_t Creature::GetInviteSenderId(boost::unique_lock<boost::mutex>& lock)
{
    return invite_sender_id_;
}

void Creature::SetInviteCounter(uint64_t invite_counter) {
    auto lock = AcquireLock();
    SetInviteCounter(invite_counter, lock);
}

void Creature::SetInviteCounter(uint64_t invite_counter, boost::unique_lock<boost::mutex>& lock)
{
    invite_counter_ = invite_counter;
}

uint64_t Creature::IncrementInviteCounter() {
    auto lock = AcquireLock();
    return IncrementInviteCounter(lock);
}

uint64_t Creature::IncrementInviteCounter(boost::unique_lock<boost::mutex>& lock)
{
    return invite_counter_++;
}

uint64_t Creature::GetInviteCounter() const {
    auto lock = AcquireLock();
    return GetInviteCounter(lock);
}

uint64_t Creature::GetInviteCounter(boost::unique_lock<boost::mutex>& lock) const
{
    return invite_counter_;
}

void Creature::SetGuildId(uint32_t guild_id) {
    auto lock = AcquireLock();
    SetGuildId(guild_id, lock);
}

void Creature::SetGuildId(uint32_t guild_id, boost::unique_lock<boost::mutex>& lock)
{
    guild_id_ = guild_id;
	DISPATCH(Creature, GuildId);
}

uint32_t Creature::GetGuildId() {
    auto lock = AcquireLock();
    return GetGuildId(lock);
}

uint32_t Creature::GetGuildId(boost::unique_lock<boost::mutex>& lock)
{
    return guild_id_;
}

void Creature::SetTargetId(uint64_t target_id) {
    auto lock = AcquireLock();
    SetTargetId(target_id, lock);
}

void Creature::SetTargetId(uint64_t target_id, boost::unique_lock<boost::mutex>& lock)
{
    target_id_ = target_id;
    DISPATCH(Creature, TargetId);
}

uint64_t Creature::GetTargetId() {
    auto lock = AcquireLock();
    return GetTargetId(lock);
}

uint64_t Creature::GetTargetId(boost::unique_lock<boost::mutex>& lock)
{
    return target_id_;
}

void Creature::SetMoodId(uint8_t mood_id) {
    auto lock = AcquireLock();
    SetMoodId(mood_id, lock);
}

void Creature::SetMoodId(uint8_t mood_id, boost::unique_lock<boost::mutex>& lock)
{
    mood_id_ = mood_id;
    DISPATCH(Creature, MoodId);
}

uint8_t Creature::GetMoodId() {
    auto lock = AcquireLock();
    return GetMoodId(lock);
}

uint8_t Creature::GetMoodId(boost::unique_lock<boost::mutex>& lock)
{
    return mood_id_;
}

void Creature::SetPerformanceId(uint32_t performance_id) {
    auto lock = AcquireLock();
    SetPerformanceId(performance_id, lock);
}

void Creature::SetPerformanceId(uint32_t performance_id, boost::unique_lock<boost::mutex>& lock)
{
    performance_id_ = performance_id;
    DISPATCH(Creature, PerformanceId);
}

uint32_t Creature::GetPerformanceId() {
    auto lock = AcquireLock();
    return GetPerformanceId(lock);
}

uint32_t Creature::GetPerformanceId(boost::unique_lock<boost::mutex>& lock)
{
    return performance_id_;
}

void Creature::SetPerformanceCounter(uint32_t performance_counter) {
    auto lock = AcquireLock();
    SetPerformanceCounter(performance_counter, lock);
}

void Creature::SetPerformanceCounter(uint32_t performance_counter, boost::unique_lock<boost::mutex>& lock)
{
    performance_counter_ = performance_counter;
}

uint32_t Creature::IncrementPerformanceCounter() {
    auto lock = AcquireLock();
    return IncrementPerformanceCounter(lock);
}

uint32_t Creature::IncrementPerformanceCounter(boost::unique_lock<boost::mutex>& lock)
{
    return performance_counter_++;
}

uint32_t Creature::GetPerformanceCounter() const {
    auto lock = AcquireLock();
    return GetPerformanceCounter(lock);
}

uint32_t Creature::GetPerformanceCounter(boost::unique_lock<boost::mutex>& lock) const
{
    return performance_counter_;
}

void Creature::SetStatCurrent(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    SetStatCurrent(stat_index, value, lock);
}

void Creature::SetStatCurrent(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{   
    stat_current_list_.update(stat_index, value);
	DISPATCH(Creature, StatCurrent);
}

void Creature::AddStatCurrent(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    AddStatCurrent(stat_index, value, lock);
}

void Creature::AddStatCurrent(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t new_value = stat_current_list_[stat_index] + value;
    stat_current_list_.update(stat_index, new_value);
	DISPATCH(Creature, StatCurrent);
}

void Creature::DeductStatCurrent(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    DeductStatCurrent(stat_index, value, lock);
}

void Creature::DeductStatCurrent(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_current_list_[stat_index];
	stat_current_list_.update(stat_index, (current > value) ? (current - value) : 0);
	DISPATCH(Creature, StatCurrent);
}

std::vector<int32_t> Creature::GetCurrentStats(void) {
    auto lock = AcquireLock();
    return GetCurrentStats(lock);
}

std::vector<int32_t> Creature::GetCurrentStats(boost::unique_lock<boost::mutex>& lock)
{
    return stat_current_list_.raw();
}

int32_t Creature::GetStatCurrent(uint16_t stat_index) {
    auto lock = AcquireLock();
    return GetStatCurrent(stat_index, lock);
}

int32_t Creature::GetStatCurrent(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_current_list_[stat_index];
}

void Creature::SetStatMax(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    SetStatMax(stat_index, value, lock);
}

void Creature::SetStatMax(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
	stat_max_list_.update(stat_index, value);
	DISPATCH(Creature, StatMax);
}

void Creature::AddStatMax(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    AddStatMax(stat_index, value, lock);
}

void Creature::AddStatMax(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    stat_max_list_.update(stat_index, stat_max_list_[stat_index] + value);
	DISPATCH(Creature, StatMax);
}

void Creature::DeductStatMax(uint16_t stat_index, int32_t value) {
    auto lock = AcquireLock();
    DeductStatMax(stat_index, value, lock);
}

void Creature::DeductStatMax(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock)
{
    int32_t current = stat_max_list_[stat_index];
	stat_max_list_.update(stat_index, (current > value) ? (current - value) : 0);
	DISPATCH(Creature, StatMax);
}

std::vector<int32_t> Creature::GetMaxStats(void) {
    auto lock = AcquireLock();
    return GetMaxStats(lock);
}

std::vector<int32_t> Creature::GetMaxStats(boost::unique_lock<boost::mutex>& lock)
{
    return stat_max_list_.raw();
}

int32_t Creature::GetStatMax(uint16_t stat_index) {
    auto lock = AcquireLock();
    return GetStatMax(stat_index, lock);
}

int32_t Creature::GetStatMax(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock)
{
    return stat_max_list_[stat_index];
}

void Creature::AddEquipmentItem(EquipmentItem& item) {
    auto lock = AcquireLock();
    AddEquipmentItem(item, lock);
}

void Creature::AddEquipmentItem(EquipmentItem& item, boost::unique_lock<boost::mutex>& lock)
{
    equipment_list_.add(item);
	DISPATCH(Creature, EquipmentItem);
}

void Creature::RemoveEquipmentItem(uint64_t object_id) {
    auto lock = AcquireLock();
    RemoveEquipmentItem(object_id, lock);
}

void Creature::RemoveEquipmentItem(uint64_t object_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(equipment_list_), end(equipment_list_), [=](EquipmentItem& item)->bool {
        return (object_id == item.object_id);
    });

    if(iter != end(equipment_list_))
    {
        return;
    }
    equipment_list_.remove(iter);
   
	DISPATCH(Creature, EquipmentItem);
}

void Creature::UpdateEquipmentItem(EquipmentItem& item) {
    auto lock = AcquireLock();
    UpdateEquipmentItem(item, lock);
}

void Creature::UpdateEquipmentItem(EquipmentItem& item, boost::unique_lock<boost::mutex>& lock)
{
	uint64_t object_id = item.object_id;
	equipment_list_.update(std::find_if(begin(equipment_list_), end(equipment_list_), [=](EquipmentItem& item)->bool {
        return (object_id == item.object_id);
    }));
	DISPATCH(Creature, EquipmentItem);
}

std::vector<EquipmentItem> Creature::GetEquipment(void) {
    auto lock = AcquireLock();
    return GetEquipment(lock);
}

std::vector<EquipmentItem> Creature::GetEquipment(boost::unique_lock<boost::mutex>& lock)
{
	return equipment_list_.raw();
}

EquipmentItem Creature::GetEquipmentItem(uint64_t object_id) {
    auto lock = AcquireLock();
    return GetEquipmentItem(object_id, lock);
}

EquipmentItem Creature::GetEquipmentItem(uint64_t object_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(equipment_list_.begin(), equipment_list_.end(), [=](EquipmentItem& object) {
        return object.object_id == object_id;
    });

    if(iter != end(equipment_list_))
	{
        return *iter;
	}

	throw EquipmentNotFound();
}

void Creature::SetDisguise(std::string disguise) {
    auto lock = AcquireLock();
    SetDisguise(disguise, lock);
}

void Creature::SetDisguise(std::string disguise, boost::unique_lock<boost::mutex>& lock)
{
	disguise_ = disguise;
    DISPATCH(Creature, Disguise);
}

std::string Creature::GetDisguise(void) {
    auto lock = AcquireLock();
    return GetDisguise(lock);
}

std::string Creature::GetDisguise(boost::unique_lock<boost::mutex>& lock)
{
    return disguise_;
}

void Creature::SetStationary(bool stationary) {
    auto lock = AcquireLock();
    SetStationary(stationary, lock);
}

void Creature::SetStationary(bool stationary, boost::unique_lock<boost::mutex>& lock)
{
    stationary_ = stationary;
    DISPATCH(Creature, Stationary);
}

bool Creature::IsStationary() {
    auto lock = AcquireLock();
    return IsStationary(lock);
}

bool Creature::IsStationary(boost::unique_lock<boost::mutex>& lock)
{
    return stationary_;
}

PvpStatus Creature::GetPvpStatus() const {
    auto lock = AcquireLock();
    return GetPvpStatus(lock);
}

PvpStatus Creature::GetPvpStatus(boost::unique_lock<boost::mutex>& lock) const
{
    return pvp_status_;
}

void Creature::SetPvPStatus(PvpStatus status) {
    auto lock = AcquireLock();
    SetPvPStatus(status, lock);
}

void Creature::SetPvPStatus(PvpStatus status, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = status;
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpStateOn(PvpStatus state) {
    auto lock = AcquireLock();
    TogglePvpStateOn(state, lock);
}

void Creature::TogglePvpStateOn(PvpStatus state, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = static_cast<PvpStatus>(pvp_status_ | state);
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpStateOff(PvpStatus state) {
    auto lock = AcquireLock();
    TogglePvpStateOff(state, lock);
}

void Creature::TogglePvpStateOff(PvpStatus state, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = static_cast<PvpStatus>(pvp_status_ & ~state);
	DISPATCH(Creature, PvPStatus);
}

void Creature::TogglePvpState(PvpStatus state) {
    auto lock = AcquireLock();
    return TogglePvpState(state, lock);
}

void Creature::TogglePvpState(PvpStatus state, boost::unique_lock<boost::mutex>& lock)
{
	pvp_status_ = static_cast<PvpStatus>(pvp_status_ ^ state);
	DISPATCH(Creature, PvPStatus);
}

bool Creature::CheckPvpState(PvpStatus state) const {
    auto lock = AcquireLock();
    return CheckPvpState(state, lock);
}

bool Creature::CheckPvpState(PvpStatus state, boost::unique_lock<boost::mutex>& lock) const
{
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

void Creature::AddToDuelList(uint64_t id) {
    auto lock = AcquireLock();
    AddToDuelList(id, lock);
}

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

void Creature::RemoveFromDuelList(uint64_t id) {
    auto lock = AcquireLock();
    RemoveFromDuelList(id, lock);
}

void Creature::RemoveFromDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock)
{
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    
	if (found != end(duel_list_))
	{
        duel_list_.erase(found);
	}
}

bool Creature::InDuelList(uint64_t id) {
    auto lock = AcquireLock();
    return InDuelList(id, lock);
}

bool Creature::InDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock)
{
    auto found = find_if(begin(duel_list_), end(duel_list_), [=] (uint64_t dueler) {
        return id == dueler;
    });
    return found != end(duel_list_);

}

std::vector<uint64_t> Creature::GetDuelList() {
    auto lock = AcquireLock();
    return GetDuelList(lock);
}

std::vector<uint64_t> Creature::GetDuelList(boost::unique_lock<boost::mutex>& lock)
{
    return duel_list_;
}

void Creature::CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer)
{
	if (GetEventDispatcher())
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
	auto dispatch = GetEventDispatcher();
	if(dispatch && !HasBuff(buff_name))
	{
		dispatch->Dispatch(std::make_shared<swganh::combat::BuffEvent>
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
		auto controller = GetController(lock);

		std::for_each(buffs_.begin(), buffs_.end(), [&] (BuffMap::value_type& entry) {
			if(controller)
			{
				RemoveBuffMessage msg;
				msg.buff = entry.second->GetName();
				controller->Notify(&msg);

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

void Creature::SerializeBaseStats(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeBaseStats(message, lock);
}

void Creature::SerializeBaseStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_base_list_.Serialize(message);
}

void Creature::SerializeSkills(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeSkills(message, lock);
}

void Creature::SerializeSkills(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	skills_.Serialize(message);
}

void Creature::SerializeStatWounds(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeStatWounds(message, lock);
}

void Creature::SerializeStatWounds(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_wound_list_.Serialize(message);
}

void Creature::SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeStatEncumberances(message, lock);
}

void Creature::SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_encumberance_list_.Serialize(message);
}

void Creature::SerializeSkillMods(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeSkillMods(message, lock);
}

void Creature::SerializeSkillMods(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	skill_mod_list_.Serialize(message);
}

void Creature::SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeMissionCriticalObjects(message, lock);
}

void Creature::SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	mission_critical_object_list_.Serialize(message);
}

void Creature::SerializeCurrentStats(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeCurrentStats(message, lock);
}

void Creature::SerializeCurrentStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_current_list_.Serialize(message);
}

void Creature::SerializeMaxStats(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeMaxStats(message, lock);
}

void Creature::SerializeMaxStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	stat_max_list_.Serialize(message);
}

void Creature::SerializeEquipment(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeEquipment(message, lock);
}

void Creature::SerializeEquipment(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	equipment_list_.Serialize(message);
}

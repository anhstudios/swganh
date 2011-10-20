#include "anh/crc.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/creature/creature_message_builder.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::creature;

Creature::Creature()
: BaseTangible()
, bank_credits_(0)
, cash_credits_(0)
, stat_base_list_counter_(0)
, stat_base_list_(std::vector<uint32_t>(10))
, skill_list_counter_(0)
, skill_list_(std::list<std::string>())
, posture_(0)
, faction_rank_(0)
, owner_id_(0)
, scale_(1.0f)
, battle_fatigue_(0)
, state_bitmask_(0)
, stat_wound_list_counter_(0)
, stat_wound_list_(std::vector<uint32_t>(10))
, acceleration_multiplier_base_(1.0f)
, acceleration_multiplier_modifier_(1.0f)
, stat_encumberance_list_counter_(0)
, stat_encumberance_list_(std::vector<uint32_t>(10))
, skill_mod_list_counter_(0)
, skill_mod_list_(std::list<SkillMod>())
, speed_multiplier_base_(1.0f)
, speed_multiplier_modifier_(1.0f)
, listen_to_id_(0)
, run_speed_(5.0f)
, slope_modifier_angle_(1.02f)
, slope_modifier_percent_(0)
, mission_critical_object_list_counter_(0)
, mission_critical_object_list_(std::list<MissionCriticalObject>())
, turn_radius_(1.0f)
, walking_speed_(1.0f)
, water_modifier_percent_(0.0125f)
, combat_level_(0)
, animation_("")
, mood_animation_("")
, weapon_id_(0)
, group_id_(0)
, invite_counter_(0)
, invite_sender_id_(0)
, guild_id_(0)
, target_id_(0)
, mood_id_(0)
, performance_counter_(0)
, performance_id_(0)
, stat_current_list_counter_(0)
, stat_current_list_(std::vector<uint32_t>(10))
, stat_max_list_counter_(0)
, stat_max_list_(std::vector<uint32_t>(10))
, equipment_list_counter_(0)
, equipment_list_(std::list<EquipmentItem>())
, disguise_("")
, stationary_(0)
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
    CreatureMessageBuilder::BuildBankCreditsDelta(this);
}

void Creature::SetCashCredits(uint32_t cash_credits)
{
    cash_credits = cash_credits;
    CreatureMessageBuilder::BuildCashCreditsDelta(this);
}

void Creature::SetStatBase(Stat stat, uint32_t value)
{
    stat_base_list_[stat] = value;
    CreatureMessageBuilder::BuildStatBaseDelta(this, 2, stat);
}

void Creature::AddStatBase(Stat stat, uint32_t value)
{
    stat_base_list_[stat] += value;
    CreatureMessageBuilder::BuildStatBaseDelta(this, 2, stat);
}

void Creature::DeductStatBase(Stat stat, uint32_t value)
{
    stat_base_list_[stat] -= value;
    CreatureMessageBuilder::BuildStatBaseDelta(this, 2, stat);
}

void Creature::AddSkill(std::string skill)
{
    if(std::find(skill_list_.begin(), skill_list_.end(), skill) == skill_list_.end())
    {
        skill_list_.push_back(skill);
        CreatureMessageBuilder::BuildSkillDelta(this, 1, skill);
    }
}

void Creature::RemoveSkill(std::string skill)
{
    if(std::find(skill_list_.begin(), skill_list_.end(), skill) != skill_list_.end())
    {
        skill_list_.remove(skill);
        CreatureMessageBuilder::BuildSkillDelta(this, 0, skill);
    }
}

bool Creature::HasSkill(std::string skill)
{
    if(std::find(skill_list_.begin(), skill_list_.end(), skill) != skill_list_.end())
    {
        return true;
    }

    return false;
}

void Creature::SetPosture(Posture posture)
{
    posture_ = posture;
    CreatureMessageBuilder::BuildPostureDelta(this);
}

void Creature::SetFactionRank(uint8_t faction_rank)
{
    faction_rank_ = faction_rank;
    CreatureMessageBuilder::BuildFactionRankDelta(this);
}

void Creature::SetOwnerId(uint64_t owner_id)
{
    owner_id_ = owner_id;
    CreatureMessageBuilder::BuildOwnerIdDelta(this);
}

void Creature::SetScale(float scale)
{
    scale_ = scale;
    CreatureMessageBuilder::BuildScaleDelta(this);
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue)
{
    battle_fatigue_ = battle_fatigue;
    CreatureMessageBuilder::BuildBattleFatigueDelta(this);
}

void Creature::SetStateBitmask(uint64_t state_bitmask)
{
    state_bitmask_ = state_bitmask;
    CreatureMessageBuilder::BuildStateBitmaskDelta(this);
}

void Creature::SetStatWound(Stat stat, uint32_t value)
{
    stat_wound_list_[stat] = value;
    CreatureMessageBuilder::BuildStatWoundDelta(this, 2, stat);
}

void Creature::AddStatWound(Stat stat, uint32_t value)
{
    stat_wound_list_[stat] += value;
    CreatureMessageBuilder::BuildStatWoundDelta(this, 2, stat);
}

void Creature::DeductStatWound(Stat stat, uint32_t value)
{
    stat_wound_list_[stat] -= value;
    CreatureMessageBuilder::BuildStatWoundDelta(this, 2, stat);
}

void Creature::SetAccelerationMultiplierBase(float acceleration_multiplier_base)
{
    acceleration_multiplier_base_ = acceleration_multiplier_base;
    CreatureMessageBuilder::BuildAccelerationMultiplierBaseDelta(this);
}

void Creature::SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier)
{
    acceleration_multiplier_modifier_ = acceleration_multiplier_modifier;
    CreatureMessageBuilder::BuildAccelerationMultiplierModifierDelta(this);
}

void Creature::SetStatEncumberance(Stat stat, uint32_t value)
{
    stat_encumberance_list_[stat] = value;
    CreatureMessageBuilder::BuildStatEncumberanceDelta(this, 2, stat);
}

void Creature::AddStatEncumberance(Stat stat, uint32_t value)
{
    stat_encumberance_list_[stat] += value;
    CreatureMessageBuilder::BuildStatEncumberanceDelta(this, 2, stat);
}

void Creature::DeductStatEncumberance(Stat stat, uint32_t value)
{
    stat_encumberance_list_[stat] -= value;
    CreatureMessageBuilder::BuildStatEncumberanceDelta(this, 2, stat);
}

void Creature::AddSkillMod(Creature::SkillMod mod)
{
    if(std::find_if(skill_mod_list_.begin(), skill_mod_list_.end(), [=](Creature::SkillMod x)->bool {
        if(x.indenfitier == mod.indenfitier)
            return true;
        else
            return false;
    }) == skill_mod_list_.end())
    {
        skill_mod_list_.push_back(mod);
        CreatureMessageBuilder::BuildSkillModDelta(this, 1, mod);
    }
}

void Creature::RemoveSkillMod(std::string identifier)
{

    auto iter = std::find_if(skill_mod_list_.begin(), skill_mod_list_.end(), [=](Creature::SkillMod x)->bool {
        if(x.indenfitier == identifier)
            return true;
        else
            return false;
    });

    if(iter != skill_mod_list_.end())
    {
        skill_mod_list_.erase(iter);
        CreatureMessageBuilder::BuildSkillModDelta(this, 0, *iter);
    }
}

void Creature::SetSpeedMultiplierBase(float speed_multiplier_base)
{
    speed_multiplier_base_ = speed_multiplier_base;
    CreatureMessageBuilder::BuildSpeedMultiplierBaseDelta(this);
}

void Creature::SetSpeedMultiplierModifier(float speed_multiplier_modifier)
{
    speed_multiplier_modifier_ = speed_multiplier_modifier;
    CreatureMessageBuilder::BuildSpeedMultiplierModifierDelta(this);
}

void Creature::SetListenToId(uint64_t listen_to_id)
{
    listen_to_id_ = listen_to_id;
    CreatureMessageBuilder::BuildListenToIdDelta(this);
}

void Creature::SetRunSpeed(float run_speed)
{
    run_speed_ = run_speed;
    CreatureMessageBuilder::BuildRunSpeedDelta(this);
}

void Creature::SetSlopeModifierAngle(float slope_modifier_angle)
{
    slope_modifier_angle_ = slope_modifier_angle;
    CreatureMessageBuilder::BuildSlopeModifierAngleDelta(this);
}

void Creature::SetSlopeModifierPercent(float slope_modifier_percent)
{
    slope_modifier_percent_ = slope_modifier_percent;
    CreatureMessageBuilder::BuildSlopeModifierPercentDelta(this);
}

void Creature::SetTurnRadius(float turn_radius)
{
    turn_radius_ = turn_radius;
    CreatureMessageBuilder::BuildTurnRadiusDelta(this);
}

void Creature::SetWalkingSpeed(float walking_speed)
{
    walking_speed_ = walking_speed;
    CreatureMessageBuilder::BuildWalkingSpeedDelta(this);
}

void Creature::SetWaterModifierPercent(float water_modifier_percent)
{
    water_modifier_percent_ = water_modifier_percent;
    CreatureMessageBuilder::BuildWaterModifierPrecentDelta(this);
}

void Creature::AddMissionCriticalObject(MissionCriticalObject& object)
{
    auto iter = std::find_if(mission_critical_object_list_.begin(), mission_critical_object_list_.end(), [=](MissionCriticalObject x)->bool {
        if(object.critical_object_id_ != x.critical_object_id_)
            return false;

        if(object.mission_owner_id_ != x.mission_owner_id_)
            return false;

        return true;
    });

    if(iter == mission_critical_object_list_.end())
    {
        mission_critical_object_list_.push_back(object);
        CreatureMessageBuilder::BuildMissionCriticalObjectDelta(this, 1, object);
    }
}

void Creature::RemoveMissionCriticalObject(MissionCriticalObject& object)
{
    auto iter = std::find_if(mission_critical_object_list_.begin(), mission_critical_object_list_.end(), [=](MissionCriticalObject x)->bool {
        if(object.critical_object_id_ != x.critical_object_id_)
            return false;

        if(object.mission_owner_id_ != x.mission_owner_id_)
            return false;

        return true;
    });

    if(iter != mission_critical_object_list_.end())
    {
        mission_critical_object_list_.erase(iter);
        CreatureMessageBuilder::BuildMissionCriticalObjectDelta(this, 0, object);
    }
}

Creature::MissionCriticalObject Creature::GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner)
{
    auto iter = std::find_if(mission_critical_object_list_.begin(), mission_critical_object_list_.end(), [=](MissionCriticalObject x)->bool {
        if(object_id != x.critical_object_id_)
            return false;

        if(mission_owner != x.mission_owner_id_)
            return false;

        return true;
    });

    if(iter != mission_critical_object_list_.end())
        return *iter;
    else
        return Creature::MissionCriticalObject(0, 0);
}

void Creature::SetCombatLevel(uint16_t combat_level)
{
    combat_level_ = combat_level;
    CreatureMessageBuilder::BuildCombatLevelDelta(this);
}

void Creature::SetAnimation(std::string animation)
{
    animation_ = animation;
    CreatureMessageBuilder::BuildAnimationDelta(this);
}

void Creature::SetMoodAnimation(std::string mood_animation)
{
    mood_animation_ = mood_animation;
    CreatureMessageBuilder::BuildMoodAnimationDelta(this);
}

void Creature::SetWeaponId(uint64_t weapon_id)
{
    weapon_id_ = weapon_id;
    CreatureMessageBuilder::BuildWeaponIdDelta(this);
}

void Creature::SetGroupId(uint64_t group_id)
{
    group_id_ = group_id;
    CreatureMessageBuilder::BuildGroupIdDelta(this);
}

void Creature::SetInviteSenderId(uint64_t invite_sender_id)
{
    invite_sender_id_ = invite_sender_id;
    CreatureMessageBuilder::BuildInviteSenderIdDelta(this);
}

void Creature::SetGuildId(uint32_t guild_id)
{
    guild_id_ = guild_id;
    CreatureMessageBuilder::BuildGuildIdDelta(this);
}

void Creature::SetTargetId(uint64_t target_id)
{
    target_id_ = target_id;
    CreatureMessageBuilder::BuildTargetIdDelta(this);
}

void Creature::SetMoodId(uint8_t mood_id)
{
    mood_id_ = mood_id;
    CreatureMessageBuilder::BuildMoodIdDelta(this);
}

void Creature::SetPerformanceId(uint32_t performance_id)
{
    performance_id_ = performance_id;
    CreatureMessageBuilder::BuildPerformanceIdDelta(this);
}

void Creature::SetStatCurrent(Stat stat, uint32_t value)
{
    stat_current_list_[stat] = value;
    CreatureMessageBuilder::BuildStatCurrentDelta(this, 2, stat);
}

void Creature::AddStatCurrent(Stat stat, uint32_t value)
{
    stat_current_list_[stat] += value;
    CreatureMessageBuilder::BuildStatCurrentDelta(this, 2, stat);
}

void Creature::DeductStatCurrent(Stat stat, uint32_t value)
{
    stat_current_list_[stat] -= value;
    CreatureMessageBuilder::BuildStatCurrentDelta(this, 2, stat);
}

void Creature::SetStatMax(Stat stat, uint32_t value)
{
    stat_max_list_[stat] = value;
    CreatureMessageBuilder::BuildStatMaxDelta(this, 2, stat);
}

void Creature::AddStatMax(Stat stat, uint32_t value)
{
    stat_max_list_[stat] += value;
    CreatureMessageBuilder::BuildStatMaxDelta(this, 2, stat);
}

void Creature::DeductStatMax(Stat stat, uint32_t value)
{
    stat_max_list_[stat] -= value;
    CreatureMessageBuilder::BuildStatMaxDelta(this, 2, stat);
}

void Creature::AddEquipmentItem(EquipmentItem& item)
{
    auto iter = std::find_if(equipment_list_.begin(), equipment_list_.end(), [=](EquipmentItem x)->bool {
        if(item.object_id != x.object_id)
            return false;
        else
            return true;
    });

    if(iter == equipment_list_.end())
    {
        equipment_list_.push_back(item);
        CreatureMessageBuilder::BuildEquipmentDelta(this, 2, item);
    }
}

void Creature::RemoveEquipmentItem(uint64_t object_id)
{
    auto iter = std::find_if(equipment_list_.begin(), equipment_list_.end(), [=](EquipmentItem x)->bool {
        if(object_id != x.object_id)
            return false;
        else
            return true;
    });

    if(iter != equipment_list_.end())
    {
        equipment_list_.erase(iter);
        CreatureMessageBuilder::BuildEquipmentDelta(this, 2, *iter);
    }
}

void Creature::SetDisguise(std::string disguise)
{
    disguise_ = disguise;
    CreatureMessageBuilder::BuildDisguiseDelta(this);
}

void Creature::SetStationary(bool stationary)
{
    stationary_ = stationary;
    CreatureMessageBuilder::BuildStationaryDelta(this);
}

boost::optional<BaselinesMessage> Creature::GetBaseline1()
{
    return move(CreatureMessageBuilder::BuildBaseline1(this));
}

boost::optional<BaselinesMessage> Creature::GetBaseline3()
{
    return move(CreatureMessageBuilder::BuildBaseline3(this));
}

boost::optional<BaselinesMessage> Creature::GetBaseline4()
{
    return move(CreatureMessageBuilder::BuildBaseline4(this));
}

boost::optional<BaselinesMessage> Creature::GetBaseline6()
{
    return move(CreatureMessageBuilder::BuildBaseline6(this));
}
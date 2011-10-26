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
, mission_critical_object_list_(swganh::messages::containers::NetworkList<MissionCriticalObject>())
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
, stat_current_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, stat_max_list_(swganh::messages::containers::NetworkArray<Stat>(9))
, equipment_list_(swganh::messages::containers::NetworkSortedList<EquipmentItem>())
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

void Creature::SetStatBase(StatIndex stat_index, uint32_t value)
{
    stat_base_list_.Update(stat_index, Stat(value));
    CreatureMessageBuilder::BuildStatBaseDelta(this);
}

void Creature::AddStatBase(StatIndex stat_index, uint32_t value)
{
    uint32_t new_stat = stat_base_list_[stat_index].value + value;
    stat_base_list_.Update(stat_index, Stat(new_stat));
    CreatureMessageBuilder::BuildStatBaseDelta(this);
}

void Creature::DeductStatBase(StatIndex stat_index, uint32_t value)
{
    uint32_t new_stat = stat_base_list_[stat_index].value - value;
    stat_base_list_.Update(stat_index, Stat(new_stat));
    CreatureMessageBuilder::BuildStatBaseDelta(this);
}

void Creature::AddSkill(std::string skill)
{
    skills_.Add(Skill(skill));
    CreatureMessageBuilder::BuildSkillDelta(this);
}

void Creature::RemoveSkill(std::string skill)
{
    auto iter = std::find_if(skills_.Begin(), skills_.End(), [=](const Skill& other_skill){
        return (skill == other_skill.name);
    });

    if(iter != skills_.End())
    {
        skills_.Remove(iter);
        CreatureMessageBuilder::BuildSkillDelta(this);
    }
}

bool Creature::HasSkill(std::string skill)
{
    auto iter = std::find_if(skills_.Begin(), skills_.End(), [=](const Skill& other_skill){
        return (skill == other_skill.name);
    });

    if(iter != skills_.End())
        return true;
    else
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

void Creature::SetStatWound(StatIndex stat_index, uint32_t value)
{
    stat_wound_list_.Update(stat_index, Stat(value));
    CreatureMessageBuilder::BuildStatWoundDelta(this);
}

void Creature::AddStatWound(StatIndex stat_index, uint32_t value)
{
    uint32_t new_stat = stat_wound_list_[stat_index].value + value;
    stat_wound_list_.Update(stat_index, Stat(new_stat));
    CreatureMessageBuilder::BuildStatWoundDelta(this);
}

void Creature::DeductStatWound(StatIndex stat_index, uint32_t value)
{
    uint32_t new_stat = stat_wound_list_[stat_index].value - value;
    stat_wound_list_.Update(stat_index, Stat(new_stat));
    CreatureMessageBuilder::BuildStatWoundDelta(this);
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

void Creature::SetStatEncumberance(StatIndex stat_index, uint32_t value)
{
    stat_encumberance_list_.Update(stat_index, Stat(value));
    CreatureMessageBuilder::BuildStatEncumberanceDelta(this);
}

void Creature::AddStatEncumberance(StatIndex stat_index, uint32_t value)
{
    uint32_t new_stat = stat_encumberance_list_[stat_index].value + value;
    stat_encumberance_list_.Update(stat_index, Stat(new_stat));
    CreatureMessageBuilder::BuildStatEncumberanceDelta(this);
}

void Creature::DeductStatEncumberance(StatIndex stat_index, uint32_t value)
{
    uint32_t new_stat = stat_encumberance_list_[stat_index].value - value;
    stat_encumberance_list_.Update(stat_index, Stat(new_stat));
    CreatureMessageBuilder::BuildStatEncumberanceDelta(this);
}

void Creature::AddSkillMod(Creature::SkillMod mod)
{
    skill_mod_list_.Add(mod.identifier, mod);
    CreatureMessageBuilder::BuildSkillModDelta(this);
}

void Creature::RemoveSkillMod(std::string identifier)
{
    auto iter = std::find_if(skill_mod_list_.Begin(), skill_mod_list_.End(), [=](std::pair<std::string, SkillMod> pair)->bool {
        return (identifier == pair.first);
    });

    if(iter != skill_mod_list_.End())
    {
        skill_mod_list_.Remove(iter);
        CreatureMessageBuilder::BuildSkillModDelta(this);
    }
}

void Creature::SetSkillMod(SkillMod mod)
{
    skill_mod_list_.Update(mod.identifier, mod);
    CreatureMessageBuilder::BuildSkillModDelta(this);
}

void Creature::ClearSkillMods(void)
{
    skill_mod_list_.Clear();
    CreatureMessageBuilder::BuildSkillModDelta(this);
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
    mission_critical_object_list_.Add(object);
    CreatureMessageBuilder::BuildMissionCriticalObjectDelta(this);
}

void Creature::RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id)
{
    auto iter = std::find_if(mission_critical_object_list_.Begin(), mission_critical_object_list_.End(), [=](const MissionCriticalObject& obj)->bool {
        if(mission_owner != obj.mission_owner_id_)
            return false;

        if(object_id != obj.critical_object_id_)
            return false;

        return true;
    });

    if(iter != mission_critical_object_list_.End())
    {
        mission_critical_object_list_.Remove(iter);
        CreatureMessageBuilder::BuildMissionCriticalObjectDelta(this);
    }
}

Creature::MissionCriticalObject Creature::GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner)
{
    return MissionCriticalObject(0,0);//return mission_critical_object_list_.Find(MissionCriticalObject(mission_owner, object_id));
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

void Creature::SetStatCurrent(StatIndex stat_index, uint32_t value)
{
    stat_current_list_.Update(stat_index, Stat(value));
    CreatureMessageBuilder::BuildStatCurrentDelta(this);
}

void Creature::AddStatCurrent(StatIndex stat_index, uint32_t value)
{
    uint32_t new_value = stat_current_list_[stat_index].value + value;
    stat_current_list_.Update(stat_index, Stat(new_value));
    CreatureMessageBuilder::BuildStatCurrentDelta(this);
}

void Creature::DeductStatCurrent(StatIndex stat_index, uint32_t value)
{
    uint32_t new_value = stat_current_list_[stat_index].value - value;
    stat_current_list_.Update(stat_index, Stat(new_value));
    CreatureMessageBuilder::BuildStatCurrentDelta(this);
}

void Creature::SetStatMax(StatIndex stat_index, uint32_t value)
{
    stat_max_list_.Update(stat_index, Stat(value));
    CreatureMessageBuilder::BuildStatMaxDelta(this);
}

void Creature::AddStatMax(StatIndex stat_index, uint32_t value)
{
    stat_max_list_.Update(stat_index, Stat(stat_max_list_.At(stat_index).value + value));
    CreatureMessageBuilder::BuildStatMaxDelta(this);
}

void Creature::DeductStatMax(StatIndex stat_index, uint32_t value)
{
    stat_max_list_.Update(stat_index, Stat(stat_max_list_.At(stat_index).value - value));
    CreatureMessageBuilder::BuildStatMaxDelta(this);
}

void Creature::AddEquipmentItem(EquipmentItem& item)
{
    equipment_list_.Add(item);
    CreatureMessageBuilder::BuildEquipmentDelta(this);
}

void Creature::RemoveEquipmentItem(uint64_t object_id)
{
    auto iter = std::find_if(equipment_list_.Begin(), equipment_list_.End(), [=](std::pair<uint16_t, EquipmentItem> item)->bool {
        return (object_id == item.second.object_id);
    });

    if(iter != equipment_list_.End())
    {
        equipment_list_.Remove(iter);
        CreatureMessageBuilder::BuildEquipmentDelta(this);
    }
}

void Creature::UpdateEquipmentItem(EquipmentItem& item)
{
    equipment_list_.Update(equipment_list_.Find(item)->first, item);
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
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "creature.h"

#include "swganh/crc.h"

#include "swganh_core/object/object_events.h"
#include "swganh_core/object/player/player.h"
#include "creature_message_builder.h"


using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::containers;
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Bank", static_pointer_cast<Creature>(shared_from_this())));
}

uint32_t Creature::GetBankCredits(void)
{
    return bank_credits_;
}

void Creature::SetCashCredits(uint32_t cash_credits)
{
    cash_credits = cash_credits;
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Cash",static_pointer_cast<Creature>(shared_from_this())));
}

uint32_t Creature::GetCashCredits(void)
{
    return cash_credits_;
}

void Creature::SetStatBase(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_base_list_.Update(stat_index, Stat(value));
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatBase",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::AddStatBase(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        uint32_t new_stat = stat_base_list_[stat_index].value + value;
        stat_base_list_.Update(stat_index, Stat(new_stat));
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatBase",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatBase",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkArray<Stat> Creature::GetBaseStats(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_base_list_;
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Skill",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Skill",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkList<Skill> Creature::GetSkills(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return skills_;
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Posture",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::FactionRank",static_pointer_cast<Creature>(shared_from_this())));
}

uint8_t Creature::GetFactionRank(void)
{
    return faction_rank_;
}

void Creature::SetOwnerId(uint64_t owner_id)
{
    owner_id_ = owner_id;
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::OwnerId",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Scale",static_pointer_cast<Creature>(shared_from_this())));
}

float Creature::GetScale(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return scale_;
}

void Creature::SetBattleFatigue(uint32_t battle_fatigue)
{
    battle_fatigue_ = battle_fatigue;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::BattleFatigue",static_pointer_cast<Creature>(shared_from_this())));
}
void Creature::AddBattleFatigue(uint32_t battle_fatigue)
{
    battle_fatigue += battle_fatigue;
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::BattleFatigue",static_pointer_cast<Creature>(shared_from_this())));
}
uint32_t Creature::GetBattleFatigue(void)
{
    return battle_fatigue_;
}

void Creature::SetStateBitmask(uint64_t state_bitmask)
{
    state_bitmask_ = state_bitmask;
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StateBitmask",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StateBitmask",static_pointer_cast<Creature>(shared_from_this())));
}
void Creature::ToggleStateOff(uint64_t state)
{
    state_bitmask_ = ( state_bitmask_ & ~ state);

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StateBitmask",static_pointer_cast<Creature>(shared_from_this())));
}
void Creature::ToggleStateBitmask(uint64_t state_bitmask)
{
    state_bitmask_ = (state_bitmask_ ^ state_bitmask);

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StateBitmask",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::SetStatWound(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_wound_list_.Update(stat_index, Stat(value));
    }

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatWound",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::AddStatWound(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t new_stat = stat_wound_list_[stat_index].value + value;
        stat_wound_list_.Update(stat_index, Stat(new_stat));
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatWound",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatWound",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkArray<Stat> Creature::GetStatWounds(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_wound_list_;
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::AccelerationMultiplierBase",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::AccelerationMultiplierModifier",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatEncumberance",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::AddStatEncumberance(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t new_stat = stat_encumberance_list_[stat_index].value + value;
        stat_encumberance_list_.Update(stat_index, Stat(new_stat));
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatEncumberance",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatEncumberance",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkArray<Stat> Creature::GetStatEncumberances(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_encumberance_list_;
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SkillMod",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SkillMod",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::SetSkillMod(SkillMod mod)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        skill_mod_list_.Update(mod.identifier, mod);
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SkillMod",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::ClearSkillMods(void)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        skill_mod_list_.Clear();
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SkillMod",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkMap<std::string, SkillMod> Creature::GetSkillMods(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return skill_mod_list_;
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SpeedMultiplierBase",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SpeedMultiplierModifer",static_pointer_cast<Creature>(shared_from_this())));
}

float Creature::GetSpeedMultiplierModifier(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return speed_multiplier_modifier_;
}

void Creature::SetListenToId(uint64_t listen_to_id)
{
    listen_to_id_ = listen_to_id;

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::ListenToId",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::RunSpeed",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SlopeModifierAngle",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::SlopeModifierPercent",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::TurnRadius",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::WalkingSpeed",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::WaterModifierPercent",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::MissionCriticalObject",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::MissionCriticalObject",static_pointer_cast<Creature>(shared_from_this())));
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

NetworkList<MissionCriticalObject> Creature::GetMissionCriticalObjects(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mission_critical_object_list_;
}

void Creature::SetCombatLevel(uint16_t combat_level)
{
    combat_level_ = combat_level;

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::CombatLevel",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Animation",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::MoodAnimation",static_pointer_cast<Creature>(shared_from_this())));
}

std::string Creature::GetMoodAnimation(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return mood_animation_;
}

void Creature::SetWeaponId(uint64_t weapon_id)
{
    weapon_id_ = weapon_id;

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::WeaponId",static_pointer_cast<Creature>(shared_from_this())));
}

uint64_t Creature::GetWeaponId(void)
{
    return weapon_id_;
}

void Creature::SetGroupId(uint64_t group_id)
{
    group_id_ = group_id;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::GroupId",static_pointer_cast<Creature>(shared_from_this())));
}

uint64_t Creature::GetGroupId(void)
{
    return group_id_;
}

void Creature::SetInviteSenderId(uint64_t invite_sender_id)
{
    invite_sender_id_ = invite_sender_id;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::InviteSenderId",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::GuildId",static_pointer_cast<Creature>(shared_from_this())));
}

uint32_t Creature::GetGuildId(void)
{
    return guild_id_;
}

void Creature::SetTargetId(uint64_t target_id)
{
    target_id_ = target_id;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::TargetId",static_pointer_cast<Creature>(shared_from_this())));
}

uint64_t Creature::GetTargetId(void)
{
    return target_id_;
}

void Creature::SetMoodId(uint8_t mood_id)
{
    mood_id_ = mood_id;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::MoodId",static_pointer_cast<Creature>(shared_from_this())));
}

uint8_t Creature::GetMoodId(void)
{
    return mood_id_;
}

void Creature::SetPerformanceId(uint32_t performance_id)
{
    performance_id_ = performance_id;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::PerformanceId",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatCurrent",static_pointer_cast<Creature>(static_pointer_cast<Creature>(shared_from_this()))));
}

void Creature::AddStatCurrent(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        int32_t new_value = stat_current_list_[stat_index].value + value;
        stat_current_list_.Update(stat_index, Stat(new_value));
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatCurrent",static_pointer_cast<Creature>(static_pointer_cast<Creature>(shared_from_this()))));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatCurrent",static_pointer_cast<Creature>(static_pointer_cast<Creature>(shared_from_this()))));
}

NetworkArray<Stat> Creature::GetCurrentStats(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_current_list_;
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatMax",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::AddStatMax(StatIndex stat_index, int32_t value)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        stat_max_list_.Update(stat_index, Stat(stat_max_list_.At(stat_index).value + value));
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatMax",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::StatMax",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkArray<Stat> Creature::GetMaxStats(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return stat_max_list_;
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

    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::EquipmentItem",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::EquipmentItem",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::UpdateEquipmentItem(EquipmentItem& item)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        auto iter = equipment_list_.Find(item);
        if(iter != end(equipment_list_))
            equipment_list_.Update(iter->first, item);
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::EquipmentItem",static_pointer_cast<Creature>(shared_from_this())));
}

NetworkSortedList<EquipmentItem> Creature::GetEquipment(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return equipment_list_;
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
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Disguise",static_pointer_cast<Creature>(shared_from_this())));
}

std::string Creature::GetDisguise(void)
{
    boost::lock_guard<boost::mutex> lock(object_mutex_);
    return disguise_;
}

void Creature::SetStationary(bool stationary)
{
    stationary_ = stationary;
    
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::Stationary",static_pointer_cast<Creature>(shared_from_this())));
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
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::PvPStatus",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::TogglePvpStateOn(PvpStatus state)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = static_cast<PvpStatus>(pvp_status_ | state);
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::PvPStatus",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::TogglePvpStateOff(PvpStatus state)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = static_cast<PvpStatus>(pvp_status_ & ~state);
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::PvPStatus",static_pointer_cast<Creature>(shared_from_this())));
}

void Creature::TogglePvpState(PvpStatus state)
{
    {
        boost::lock_guard<boost::mutex> lock(object_mutex_);
        pvp_status_ = static_cast<PvpStatus>(pvp_status_ ^ state);
    }
    GetEventDispatcher()->Dispatch(make_shared<CreatureEvent>
        ("Creature::PvPStatus",static_pointer_cast<Creature>(shared_from_this())));
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

    GetEventDispatcher()->Dispatch(make_shared<swganh::object::ObserverEvent>
        ("Creature::Baselines", shared_from_this(), observer));
}


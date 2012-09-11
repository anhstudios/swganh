// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>
#include <list>

#include <boost/thread/mutex.hpp>

#include "swganh_core/object/tangible/tangible.h"

#include "swganh_core/messages/containers/network_array.h"
#include "swganh_core/messages/containers/network_sorted_list.h"
#include "swganh_core/messages/containers/network_list.h"
#include "swganh_core/messages/containers/network_map.h"

namespace swganh {
namespace object {


/**
 * Represents a Stats index.
 */
enum StatIndex : uint32_t
{
    HEALTH = 0,
    STRENGTH,
    CONSTITUTION,
    ACTION,
    QUICKNESS,
    STAMINA,
    MIND,
    FOCUS,
    WILLPOWER
};

/**
 * Represents the id offset of various "linked" items.
 */
enum CreatureOffSet
{
	PLAYER_OFFSET = 1,
	DATAPAD_OFFSET,
	INVENTORY_OFFSET,
	BANK_OFFSET,
	MISSION_OFFSET,
	HAIR_OFFSET,
	EQUIPED_OFFSET
};

enum Posture : uint8_t
{
	INVALID = 0xFF,
	UPRIGHT = 0,
	CROUCHED = 1,
	PRONE,
	SNEAKING,
	BLOCKING,
	CLIMBING,
	FLYING,
	LYING_DOWN,
	SITTING,
	SKILL_ANIMATING,
	DRIVING_VEHICLE,
	RIDING_CREATURE,
	KNOCKED_DOWN,
	INCAPACITATED,
	DEAD
};

/**
 *
 */
//enum Locomotion : uint8_t
//{
//    STANDING = 1,
//    SNEAKING,
//    WALKING,
//    RUNNING,
//    KNEELING,
//    CROUCH_SNEAKING,
//    CROUCH_WALKING,
//    PRONE,
//    SKILL_ANIMATING,
//    CRAWLING,
//    CLIMBING_STATIONARY,
//    CLIMBING,
//    HOVERING,
//    FLYING,
//    LYING_DOWN,
//    SITTING,
//    DRIVING_VEHICLE,
//    RIDING_CREATURE,
//    KNOCKED_DOWN,
//    INCAPACITATED,
//    DEAD,
//    BLOCKING
//};

/**
 *
 */
enum State : int64_t
{
	NONE = 0x00,
    COVER = 0x01,
    COMBAT = 0x02,
    PEACE = 0x04,
    AIMING = 0x08,
    ALERT = 0x10,
    BERSERK = 0x20,
    FEIGN_DEATH = 0x40,
    COMBAT_ATTITUDE_EVASIVE = 0x80,
    COMBAT_ATTITUDE_NORMAL = 0x100,
    COMBAT_ATTITUDE_AGGRESSIVE = 0x200,
    TUMBLING = 0x400,
    RALLIED = 0x800,
    STUNNED = 0x1000,
    BLINDED = 0x2000,
    DIZZY = 0x4000,
    INTIMIDATED = 0x8000,
    IMMOBILIZED = 0x10000,
    FROZEN = 0x20000,
    SWIMMING = 0x40000,
    SITTING_ON_CHAIR = 0x80000,
    CRAFTING = 0x100000,
    GLOWING_JEDI = 0x200000,
    MASK_SCENT = 0x400000,
    POISONED = 0x800000,
    BLEEDING = 0x1000000,
    DISEASED = 0x2000000,
    ON_FIRE = 0x4000000,
    RIDING_MOUNT = 0x8000000,
    MOUNTED_CREATURE = 0x10000000,
    PILOTING_SHIP = 0x20000000,
    SHIP_OPERATIONS = 0x40000000,
    SHIP_GUNNER = 0x80000000,
    SHIP_INTERIOR = 0x100000000,
    PILOTING_POB_SHIP = 0x200000000
};

enum PvpStatus
{
    PvPStatus_None          = 0x00000000,
    PvPStatus_Attackable    = 0x00000001,
    PvPStatus_Aggressive    = 0x00000002,
    PvPStatus_Overt         = 0x00000004,
    PvPStatus_Tef           = 0x00000008,
    PvPStatus_Player        = 0x00000010,
    PvPStatus_Enemy         = 0x00000020,
    PvPStatus_Duel          = 0x00000040
};

/**
 *
 */
struct SkillMod
{
    SkillMod()
    {}

    SkillMod(std::string identifier_, uint32_t base_, uint32_t modifier_)
        : identifier(identifier_)
        , base(base_)
        , modifier(modifier_)
    {}

    ~SkillMod(void)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint8_t>(0);                 // Unused
        message.data.write<std::string>(identifier);    // Identifier
        message.data.write<uint32_t>(base);             // Base
        message.data.write<uint32_t>(modifier);         // Modifier
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(identifier);    // Identifier
        message.data.write<uint32_t>(base);             // Base
        message.data.write<uint32_t>(modifier);         // Modifier
    }

    bool operator==(const SkillMod& other)
    {
        return (identifier == other.identifier);
    }

    std::string identifier;
    uint32_t base;
    uint32_t modifier;
};

/**
 *
 */
struct Stat
{
    Stat(void)
        : value(0)
    {}

    Stat(int32_t value_)
        : value(value_)
    {}

    ~Stat()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<int32_t>(value);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<int32_t>(value);
    }

    int32_t value;
};

/**
 *
 */
struct EquipmentItem
{
    EquipmentItem()
    {
    }

    EquipmentItem(uint64_t object_id_, uint32_t template_crc_ = 0, std::string customization_ = std::string(""), uint32_t containment_type_ = 4)
        : customization(customization_)
        , containment_type(containment_type_)
        , object_id(object_id_)
        , template_crc(template_crc_)
    {
    }

    ~EquipmentItem()
    {
    }

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<std::string>(customization);
        message.data.write<uint32_t>(containment_type);
        message.data.write<uint64_t>(object_id);
        message.data.write<uint32_t>(template_crc);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(customization);
        message.data.write<uint32_t>(containment_type);
        message.data.write<uint64_t>(object_id);
        message.data.write<uint32_t>(template_crc);
    }

    bool operator==(const EquipmentItem& other)
    {
        return (object_id != other.object_id);
    }

    std::string customization;
    uint32_t containment_type;
    uint64_t object_id;
    uint32_t template_crc;
};

/**
 *
 */
struct MissionCriticalObject
{
    MissionCriticalObject(uint64_t mission_owner_id, uint16_t critical_object_id)
        : mission_owner_id_(mission_owner_id)
        , critical_object_id_(critical_object_id)
    {}

    ~MissionCriticalObject(void)
    {}

    uint64_t mission_owner_id_;
    uint64_t critical_object_id_;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(mission_owner_id_);
        message.data.write<uint64_t>(critical_object_id_);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(mission_owner_id_);
        message.data.write<uint64_t>(critical_object_id_);
    }

    bool operator==(const MissionCriticalObject& object)
    {
        if(mission_owner_id_ != object.mission_owner_id_)
            return false;

        if(critical_object_id_ != object.critical_object_id_)
            return false;

        return true;
    }
};

/**
 *
 */
struct Skill
{
    Skill(std::string name_)
        : name(name_)
    {}

    ~Skill()
    {}

    std::string name;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<std::string>(name);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(name);
    }

    bool operator==(const Skill& other)
    {
        return (name == other.name);
    }
};
/**
 *
 */
struct SkillCommands
{

};

class CreatureFactory;
class CreatureMessageBuilder;

/**
 *
 */
class Creature : public swganh::object::Tangible
{
public:
    typedef CreatureFactory FactoryType;
    typedef CreatureMessageBuilder MessageBuilderType;

    Creature();
    ~Creature();

    // CREO
    uint32_t GetType() const;
    const static uint32_t type = 0x4352454F;

    // Bank Credits
    void SetBankCredits(uint32_t bank_credits);
    uint32_t GetBankCredits(void);

    // Cash Credits
    void SetCashCredits(uint32_t cash_credits);
    uint32_t GetCashCredits(void);

    // Stat Base
    void SetStatBase(StatIndex stat_index, int32_t value);
    void AddStatBase(StatIndex stat_index, int32_t value);
    void DeductStatBase(StatIndex stat_index, int32_t value);
    swganh::messages::containers::NetworkArray<Stat> GetBaseStats(void);
    int32_t GetStatBase(StatIndex stat_index);

    // Skills
    void AddSkill(std::string skill);
    void RemoveSkill(std::string skill);
    swganh::messages::containers::NetworkList<Skill> GetSkills(void);
    bool HasSkill(std::string skill);

    // Skill Commands
    std::map<uint32_t, std::string> GetSkillCommands();
    bool HasSkillCommand(std::string skill);
    void AddSkillCommand(std::pair<uint32_t, std::string> skill_command);
    void RemoveSkillCommand(std::string skill_command);

    // Posture
    void SetPosture(Posture posture);
    Posture GetPosture(void);

    // IsDead
    bool IsDead();
    // IsIncapacitated
    bool IsIncapacitated();
    // Faction Rank
    void SetFactionRank(uint8_t faction_rank);
    uint8_t GetFactionRank(void);

    // Owner Id
    void SetOwnerId(uint64_t owner_id);
    uint64_t GetOwnerId(void);

    // Scale
    void SetScale(float scale);
    float GetScale(void);

    // Battle Fatigue
    void AddBattleFatigue(uint32_t battle_fatigue);
    void SetBattleFatigue(uint32_t battle_fatigue);
    uint32_t GetBattleFatigue(void);

    // State Bitmask
    void SetStateBitmask(uint64_t state_bitmask);
    void ToggleStateBitmask(uint64_t state_bitmask);
    void ToggleStateOn(uint64_t state);
    void ToggleStateOff(uint64_t state);
    uint64_t GetStateBitmask(void);
    bool HasState(uint64_t state);

    // Wounds
    void DeductStatWound(StatIndex stat_index, int32_t value);
    void AddStatWound(StatIndex stat_index, int32_t value);
    void SetStatWound(StatIndex stat_index, int32_t value);
    swganh::messages::containers::NetworkArray<Stat> GetStatWounds(void);
    int32_t GetStatWound(StatIndex stat_index);

    // Acceleration Multiplier Base
    void SetAccelerationMultiplierBase(float acceleration_multiplier_base);
    float GetAccelerationMultiplierBase(void);

    // Acceleration Multiplier Modifier
    void SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier);
    float GetAccelerationMultiplierModifier(void);

    // Stat Encumberance
    void AddStatEncumberance(StatIndex stat_index, int32_t value);
    void DeductStatEncumberance(StatIndex stat_index, int32_t value);
    void SetStatEncumberance(StatIndex stat_index, int32_t value);
    swganh::messages::containers::NetworkArray<Stat> GetStatEncumberances(void);
    int32_t GetStatEncumberance(StatIndex stat_index);

    // Skill Mods
    void AddSkillMod(SkillMod mod);
    void RemoveSkillMod(std::string identifier);
    void SetSkillMod(SkillMod mod);
    void ClearSkillMods(void);
    swganh::messages::containers::NetworkMap<std::string, SkillMod> GetSkillMods(void);
    SkillMod GetSkillMod(std::string identifier);

    // Speed Multiplier Base
    void SetSpeedMultiplierBase(float speed_multiplier_base);
    float GetSpeedMultiplierBase(void);

    // Speed Multiplier Modifier
    void SetSpeedMultiplierModifier(float speed_multiplier_modifier);
    float GetSpeedMultiplierModifier(void);

    // Listen To Id
    void SetListenToId(uint64_t listen_to_id);
    uint64_t GetListenToId(void);

    // Run Speed
    void SetRunSpeed(float run_speed);
    float GetRunSpeed(void);

    // Slop Modifier Angle
    void SetSlopeModifierAngle(float slope_modifier_angle);
    float GetSlopeModifierAngle(void);

    // Slope Modifier Percent
    void SetSlopeModifierPercent(float slope_modifier_percent);
    float GetSlopeModifierPercent(void);

    // Turn Radius
    void SetTurnRadius(float turn_radius);
    float GetTurnRadius(void);

    // Walking Speed
    void SetWalkingSpeed(float walking_speed);
    float GetWalkingSpeed(void);

    // Water Modifier Percent
    void SetWaterModifierPercent(float water_modifier_percent);
    float GetWaterModifierPercent(void);

    // Mission Critical Objects
    void AddMissionCriticalObject(MissionCriticalObject& object);
    void RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id);
    MissionCriticalObject GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner);
    swganh::messages::containers::NetworkList<MissionCriticalObject> GetMissionCriticalObjects(void);

    // Combat Level
    void SetCombatLevel(uint16_t);
    uint16_t GetCombatLevel(void);

    // Animation
    void SetAnimation(std::string animation);
    std::string GetAnimation(void);

    // Mood Animation
    void SetMoodAnimation(std::string mood_animation);
    std::string GetMoodAnimation(void);

    // Weapon Id
    void SetWeaponId(uint64_t weapon_id);
    uint64_t GetWeaponId(void);

    // Group Id
    void SetGroupId(uint64_t group_id);
    uint64_t GetGroupId(void);

    // Invite Sender Id
    void SetInviteSenderId(uint64_t invite_sender_id);
    uint64_t GetInviteSenderId(void);

    // Invite Counter
    void SetInviteCounter(uint64_t invite_counter);
    uint64_t IncrementInviteCounter();
    uint64_t GetInviteCounter(void) const;

    // Guild Id
    void SetGuildId(uint32_t guild_id);
    uint32_t GetGuildId(void);

    // Target Id
    void SetTargetId(uint64_t target_id);
    uint64_t GetTargetId(void);

    // Mood Id
    void SetMoodId(uint8_t mood_id);
    uint8_t GetMoodId(void) ;

    // Performance Id
    void SetPerformanceId(uint32_t performance_id);
    uint32_t GetPerformanceId(void);

    // Performance Counter
    void SetPerformanceCounter(uint32_t performance_counter);
    uint32_t IncrementPerformanceCounter();
    uint32_t GetPerformanceCounter(void) const;

    // Current Stats
    void SetStatCurrent(StatIndex stat_index, int32_t value);
    void AddStatCurrent(StatIndex stat_index, int32_t value);
    void DeductStatCurrent(StatIndex stat_index, int32_t value);
    swganh::messages::containers::NetworkArray<Stat> GetCurrentStats(void);
    int32_t GetStatCurrent(StatIndex stat_index);

    // Max Stats
    void SetStatMax(StatIndex stat_index, int32_t value);
    void AddStatMax(StatIndex stat_index, int32_t value);
    void DeductStatMax(StatIndex stat_index, int32_t value);
    swganh::messages::containers::NetworkArray<Stat> GetMaxStats(void);
    int32_t GetStatMax(StatIndex stat_index);

    // Equipment List
    void AddEquipmentItem(EquipmentItem& item);
    void RemoveEquipmentItem(uint64_t object_id);
    void UpdateEquipmentItem(EquipmentItem& item);
    swganh::messages::containers::NetworkSortedList<EquipmentItem> GetEquipment();
    EquipmentItem GetEquipmentItem(uint64_t object_id);

    // Disguise
    void SetDisguise(std::string disguise);
    std::string GetDisguise(void);

    // Stationary
    void SetStationary(bool stationary);
    bool IsStationary(void);

    PvpStatus GetPvpStatus() const;
    void SetPvPStatus(PvpStatus status);
    void TogglePvpStateOn(PvpStatus state);
    void TogglePvpStateOff(PvpStatus state);
    void TogglePvpState(PvpStatus state);
    bool CheckPvpState(PvpStatus state) const;
    bool CanAttack(Creature* creature);

    void AddToDuelList(uint64_t id);
    void RemoveFromDuelList(uint64_t id);
    bool InDuelList(uint64_t id);
    std::vector<uint64_t> GetDuelList();

    // Baselines
    virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	
    typedef swganh::ValueEvent<std::shared_ptr<Creature>> CreatureEvent;
private:
    std::atomic<uint32_t>    bank_credits_;                                                             // update 1 variable 0
    std::atomic<uint32_t>    cash_credits_;                                                             // update 1 variable 1
    swganh::messages::containers::NetworkArray<Stat> stat_base_list_;                                   // update 1 variable 2
    swganh::messages::containers::NetworkList<Skill> skills_;                                           // update 1 variable 3
    std::map<uint32_t, std::string> skill_commands_;
    std::atomic<uint32_t>    posture_;                                                                   // update 3 variable 11
    std::atomic<uint8_t>     faction_rank_;                                                              // update 3 variable 12
    std::atomic<uint64_t>    owner_id_;                                                                  // update 3 variable 13
    float       scale_;                                                                                  // update 3 variable 14
    std::atomic<uint32_t>    battle_fatigue_;                                                            // update 3 variable 15
    std::atomic<uint64_t>    state_bitmask_;                                                             // update 3 variable 16
    swganh::messages::containers::NetworkArray<Stat> stat_wound_list_;                                   // update 3 variable 17
    float       acceleration_multiplier_base_;                                                           // update 4 variable 0
    float       acceleration_multiplier_modifier_;                                                       // update 4 variable 1
    swganh::messages::containers::NetworkArray<Stat> stat_encumberance_list_;                            // update 4 variable 2
    swganh::messages::containers::NetworkMap<std::string, SkillMod> skill_mod_list_;                     // update 4 variable 3
    float       speed_multiplier_base_;                                                                  // update 4 variable 4
    float       speed_multiplier_modifier_;                                                              // update 4 variable 5
    std::atomic<uint64_t>    listen_to_id_;                                                              // update 4 variable 6
    float       run_speed_;                                                                              // update 4 variable 7
    float       slope_modifier_angle_;                                                                   // update 4 variable 8
    float       slope_modifier_percent_;                                                                 // update 4 variable 9
    float       turn_radius_;                                                                            // update 4 variable 10
    float       walking_speed_;                                                                          // update 4 variable 11
    float       water_modifier_percent_;                                                                 // update 4 variable 12
    swganh::messages::containers::NetworkList<MissionCriticalObject> mission_critical_object_list_;     // update 4 variable 13
    std::atomic<uint16_t>    combat_level_;                                                              // update 6 variable 2
    std::string animation_;                                                                              // update 6 variable 3
    std::string mood_animation_;                                                                         // update 6 variable 4
    std::atomic<uint64_t>    weapon_id_;                                                                  // update 6 variable 5
    std::atomic<uint64_t>    group_id_;                                                                   // update 6 variable 6
    std::atomic<uint64_t>    invite_sender_id_;                                                           // update 6 variable 7
    std::atomic<uint64_t>    invite_counter_;                                                             // update 6 variable 7
    std::atomic<uint32_t>    guild_id_;                                                                   // update 6 variable 8
    std::atomic<uint64_t>    target_id_;                                                                  // update 6 variable 9
    std::atomic<uint8_t>     mood_id_;                                                                    // update 6 variable 10
    std::atomic<uint32_t>    performance_counter_;                                                        // update 6 variable 11
    std::atomic<uint32_t>    performance_id_;                                                             // update 6 variable 12
    swganh::messages::containers::NetworkArray<Stat> stat_current_list_;                                  // update 6 variable 13
    swganh::messages::containers::NetworkArray<Stat> stat_max_list_;                                      // update 6 variable 14
    swganh::messages::containers::NetworkSortedList<EquipmentItem> equipment_list_;                       // update 6 variable 15
    std::string disguise_;                                                                                // update 6 variable 16
    std::atomic<bool> stationary_;                                                                        // update 6 variable 17
    PvpStatus pvp_status_;
    std::vector<uint64_t> duel_list_;
};

}}  // namespace swganh::object

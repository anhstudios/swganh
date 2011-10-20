/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_CREATURE_H_
#define SWGANH_OBJECT_CREATURE_H_

#include <list>

#include "swganh/object/tangible/base_tangible.h"

namespace swganh {
namespace object {
namespace creature {

/**
 *
 */
class Creature : public swganh::object::tangible::BaseTangible
{
public:
    Creature();
    ~Creature();

    enum Stat
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

    enum Posture
    {
        STANDING = 1,
        SNEAKING,
        WALKING,
        RUNNING,
        KNEELING,
        CROUCH_SNEAKING,
        CROUCH_WALKING,
        PRONE,
        SKILL_ANIMATING,
        CRAWLING,
        CLIMBING_STATIONARY,
        CLIMBING,
        HOVERING,
        FLING,
        LYING_DOWN,
        SITTING,
        DRIVING_VEHICLE,
        RIDING_CREATURE,
        KNOCKED_DOWN,
        INCAPACITATED,
        DEAD,
        BLOCKING
    };

    enum State
    {
        COVER = 1,
        COMBAT,
        PEACE,
        AIMING,
        ALERT,
        BERSERK,
        FEIGN_DEATH,
        COMBAT_ATTITUDE_EVASIVE,
        COMBAT_ATTITUDE_NORMAL,
        COMBAT_ATTITUDE_AGGRESSIVE,
        TUMBLING,
        RALLIED,
        STUNNED,
        BLINDED,
        DIZZY,
        INTIMIDATED,
        IMMOBILIZED,
        FROZEN,
        SWIMMING,
        SITTING_ON_CHAIR,
        CRAFTING,
        GLOWING_JEDI,
        MASK_SCENT,
        POISONED,
        BLEEDING,
        DISEASED,
        ON_FIRE,
        RIDING_MOUNT,
        MOUNTED_CREATURE,
        PILOTING_SHIP,
        SHIP_OPERATIONS,
        SHIP_GUNNER,
        SHIP_INTERIOR,
        PILOTING_POB_SHIP
    };

    struct SkillMod
    {
        std::string indenfitier;
        uint32_t base;
        uint32_t modifier;
    };
    
    struct EquipmentItem
    {
        EquipmentItem()
            : customization("")
            , containment_type(4)
            , object_id(0)
            , template_crc(0)
        {
        }

        std::string customization;
        uint32_t containment_type;
        uint64_t object_id;
        uint32_t template_crc;
    };

    struct MissionCriticalObject
    {
        MissionCriticalObject(uint64_t mission_owner_id, uint16_t critical_object_id)
            : mission_owner_id_(mission_owner_id)
            , critical_object_id_(critical_object_id)
        {
        }

        uint64_t mission_owner_id_;
        uint64_t critical_object_id_;
    };

    // CREO
    uint32_t GetType() const;
    const static uint32_t type = 0x4352454F;

    // Bank Credits
    void SetBankCredits(uint32_t bank_credits);
    uint32_t GetBankCredits(void) const { return bank_credits_; }

    // Cash Credits
    void SetCashCredits(uint32_t cash_credits);
    uint32_t GetCashCredits(void) const { return cash_credits_; }

    // Stat Base
    void SetStatBase(Stat stat, uint32_t value);
    void AddStatBase(Stat stat, uint32_t value);
    void DeductStatBase(Stat stat, uint32_t value);
    std::vector<uint32_t> GetBaseStats(void) const { return stat_base_list_; }
    uint32_t GetStatBase(Stat stat) { return stat_base_list_[stat]; }

    // Skills
    void AddSkill(std::string skill);
    void RemoveSkill(std::string skill);
    std::list<std::string> GetSkills(void) { return skill_list_; }
    bool HasSkill(std::string skill);

    // Posture
    void SetPosture(Posture posture);
    Posture GetPosture(void) const { return (Posture)posture_; }

    // Faction Rank
    void SetFactionRank(uint8_t faction_rank);
    uint8_t GetFactionRank(void) const { return faction_rank_; }

    // Owner Id
    void SetOwnerId(uint64_t owner_id);
    uint64_t GetOwnerId(void) const { return owner_id_; }

    // Scale
    void SetScale(float scale);
    float GetScale(void) const { return scale_; }

    // Battle Fatigue
    void SetBattleFatigue(uint32_t battle_fatigue);
    uint32_t GetBattleFatigue(void) const { return battle_fatigue_; }

    // State Bitmask
    void SetStateBitmask(uint64_t state_bitmask);
    void ToggleStateBitmask(uint64_t state_bitmask);
    uint64_t GetStateBitmask(void) const { return state_bitmask_; }

    // Wounds
    void DeductStatWound(Stat stat, uint32_t value);
    void AddStatWound(Stat stat, uint32_t value);
    void SetStatWound(Stat stat, uint32_t value);
    std::vector<uint32_t> GetStatWounds(void) const { return stat_wound_list_; }

    // Acceleration Multiplier Base
    void SetAccelerationMultiplierBase(float acceleration_multiplier_base);
    float GetAccelerationMultiplierBase(void) const { return acceleration_multiplier_base_; }

    // Acceleration Multiplier Modifier
    void SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier);
    float GetAccelerationMultiplierModifier(void) const { return acceleration_multiplier_modifier_; }

    // Stat Encumberance
    void AddStatEncumberance(Stat stat, uint32_t value);
    void DeductStatEncumberance(Stat stat, uint32_t value);
    void SetStatEncumberance(Stat stat, uint32_t value);
    std::vector<uint32_t> GetStatEncumberances(void) const { return stat_encumberance_list_; }
    uint32_t GetStatEncumberance(Stat stat) const { return stat_encumberance_list_[stat]; }

    // Skill Mods
    void AddSkillMod(SkillMod mod);
    void RemoveSkillMod(std::string identifier);
    std::list<SkillMod> GetSkillMods(void) const { return skill_mod_list_; }
    SkillMod GetSkillMod(std::string identifier);

    // Speed Multiplier Base
    void SetSpeedMultiplierBase(float speed_multiplier_base);
    float GetSpeedMultiplierBase(void) const { return speed_multiplier_base_; }

    // Speed Multiplier Modifier
    void SetSpeedMultiplierModifier(float speed_multiplier_modifier);
    float GetSpeedMultiplierModifier(void) const { return speed_multiplier_modifier_; }

    // Listen To Id
    void SetListenToId(uint64_t listen_to_id);
    uint64_t GetListenToId(void) const { return listen_to_id_; }

    // Run Speed
    void SetRunSpeed(float run_speed);
    float GetRunSpeed(void) const { return run_speed_; }

    // Slop Modifier Angle
    void SetSlopeModifierAngle(float slope_modifier_angle);
    float GetSlopeModifierAngle(void) const { return slope_modifier_angle_; }

    // Slope Modifier Percent
    void SetSlopeModifierPercent(float slope_modifier_percent);
    float GetSlopeModifierPercent(void) const { return slope_modifier_percent_; }

    // Turn Radius
    void SetTurnRadius(float turn_radius);
    float GetTurnRadius(void) const { return turn_radius_; }

    // Walking Speed
    void SetWalkingSpeed(float walking_speed);
    float GetWalkingSpeed(void) const { return walking_speed_; }

    // Water Modifier Percent
    void SetWaterModifierPercent(float water_modifier_percent);
    float GetWaterModifierPercent(void) const { return water_modifier_percent_; }

    // Mission Critical Objects
    void AddMissionCriticalObject(MissionCriticalObject& object);
    void RemoveMissionCriticalObject(MissionCriticalObject& object);
    MissionCriticalObject GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner);
    std::list<MissionCriticalObject> GetMissionCriticalObjects(void) const { return mission_critical_object_list_; }

    // Combat Level
    void SetCombatLevel(uint16_t);
    uint16_t GetCombatLevel(void) const { return combat_level_; }

    // Animation
    void SetAnimation(std::string animation);
    std::string GetAnimation(void) const { return animation_; }

    // Mood Animation
    void SetMoodAnimation(std::string mood_animation);
    std::string GetMoodAnimation(void) const { return mood_animation_; }

    // Weapon Id
    void SetWeaponId(uint64_t weapon_id);
    uint64_t GetWeaponId(void) const { return weapon_id_; }

    // Group Id
    void SetGroupId(uint64_t group_id);
    uint64_t GetGroupId(void) const { return group_id_; }

    // Invite Sender Id
    void SetInviteSenderId(uint64_t invite_sender_id);
    uint64_t GetInviteSenderId(void) const { return invite_sender_id_; }

    // Guild Id
    void SetGuildId(uint32_t guild_id);
    uint32_t GetGuildId(void) const { return guild_id_; }

    // Target Id
    void SetTargetId(uint64_t target_id); 
    uint64_t GetTargetId(void) const { return target_id_; }

    // Mood Id
    void SetMoodId(uint8_t mood_id);
    uint8_t GetMoodId(void) const { return mood_id_; }

    // Performance Id
    void SetPerformanceId(uint32_t performance_id);
    uint32_t GetPerformanceId(void) const { return performance_id_; }

    // Current Stats
    void SetStatCurrent(Stat stat, uint32_t value);
    void AddStatCurrent(Stat stat, uint32_t value);
    void DeductStatCurrent(Stat stat, uint32_t value);
    std::vector<uint32_t> GetCurrentStats(void) const { return stat_current_list_; }
    uint32_t GetStatCurrent(Stat stat) const { return stat_current_list_[stat]; }

    // Max Stats
    void SetStatMax(Stat stat, uint32_t value);
    void AddStatMax(Stat stat, uint32_t value);
    void DeductStatMax(Stat stat, uint32_t value);
    std::vector<uint32_t> GetMaxStats(void) const { return stat_max_list_; }
    uint32_t GetStatMax(Stat stat) const { return stat_max_list_[stat]; }

    // Equipment List
    void AddEquipmentItem(EquipmentItem& item);
    void RemoveEquipmentItem(uint64_t object_id);
    std::list<EquipmentItem> GetEquipment() { return equipment_list_; }
    EquipmentItem GetEquipmentItem(uint64_t object_id);

    // Disguise
    void SetDisguise(std::string disguise);
    std::string GetDisguise(void) const { return disguise_; }

    // Stationary
    void SetStationary(bool stationary);
    bool IsStationary(void) const { return stationary_; }

    // Baselines
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline1();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline4();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();

private:
    friend class CreatureMessageBuilder;

    uint32_t    bank_credits_;                                  // update 1 variable 0
    uint32_t    cash_credits_;                                  // update 1 variable 1
    uint32_t    stat_base_list_counter_;                        // update 1 variable 2
    std::vector<uint32_t> stat_base_list_;                      // update 1 variable 2
    uint32_t    skill_list_counter_;                            // update 1 variable 3
    std::list<std::string> skill_list_;                         // update 1 variable 3
    uint32_t    posture_;                                       // update 3 variable 11
    uint8_t     faction_rank_;                                  // update 3 variable 12
    uint64_t    owner_id_;                                      // update 3 variable 13
    float       scale_;                                         // update 3 variable 14
    uint32_t    battle_fatigue_;                                // update 3 variable 15
    uint64_t    state_bitmask_;                                 // update 3 variable 16
    uint32_t    stat_wound_list_counter_;                       // update 3 variable 17
    std::vector<uint32_t> stat_wound_list_;                     // update 3 variable 17
    float       acceleration_multiplier_base_;                  // update 4 variable 0
    float       acceleration_multiplier_modifier_;              // update 4 variable 1
    uint32_t    stat_encumberance_list_counter_;                // update 4 variable 2
    std::vector<uint32_t> stat_encumberance_list_;              // update 4 variable 2
    uint32_t    skill_mod_list_counter_;                        // update 4 variable 3
    std::list<SkillMod> skill_mod_list_;                        // update 4 variable 3
    float       speed_multiplier_base_;                         // update 4 variable 4
    float       speed_multiplier_modifier_;                     // update 4 variable 5
    uint64_t    listen_to_id_;                                  // update 4 variable 6
    float       run_speed_;                                     // update 4 variable 7
    float       slope_modifier_angle_;                          // update 4 variable 8
    float       slope_modifier_percent_;                        // update 4 variable 9
    float       turn_radius_;                                   // update 4 variable 10
    float       walking_speed_;                                 // update 4 variable 11
    float       water_modifier_percent_;                        // update 4 variable 12
    uint32_t    mission_critical_object_list_counter_;          // update 4 variable 13
    std::list<MissionCriticalObject> mission_critical_object_list_;// update 4 variable 13
    uint16_t    combat_level_;                                  // update 6 variable 2
    std::string animation_;                                     // update 6 variable 3
    std::string mood_animation_;                                // update 6 variable 4
    uint64_t    weapon_id_;                                     // update 6 variable 5
    uint64_t    group_id_;                                      // update 6 variable 6
    uint64_t    invite_sender_id_;                              // update 6 variable 7
    uint64_t    invite_counter_;                                // update 6 variable 7
    uint32_t    guild_id_;                                      // update 6 variable 8
    uint64_t    target_id_;                                     // update 6 variable 9
    uint8_t     mood_id_;                                       // update 6 variable 10
    uint32_t    performance_counter_;                           // update 6 variable 11
    uint32_t    performance_id_;                                // update 6 variable 12
    uint32_t    stat_current_list_counter_;                     // update 6 variable 13
    std::vector<uint32_t> stat_current_list_;                   // update 6 variable 13
    uint32_t    stat_max_list_counter_;                         // update 6 variable 14
    std::vector<uint32_t> stat_max_list_;                       // update 6 variable 14
    uint32_t    equipment_list_counter_;                        // update 6 variable 15
    std::list<EquipmentItem> equipment_list_;                   // update 6 variable 15
    std::string disguise_;                                      // update 6 variable 16
    bool        stationary_;                                    // update 6 variable 17

};

}}}  // namespace swganh::object::creature

#endif // SWGANH_OBJECT_CREATURE_CREATURE_H_

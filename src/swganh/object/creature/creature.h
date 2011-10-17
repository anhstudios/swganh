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
    enum HAM
    {
        HEALTH,
        ACTION,
        MIND
    };

    enum POSTURE
    {
        STANDING = 0,
        SNEAKING,
        WALKING,
        RUNNING,
        KNEELING,
        CROUCH_SNEAKING,
        CROUCH_WALKING,
        PRONE,
        CRAWLING,
        CLIMBING_STATIONARY,
        CLIMBING,
        HOVERING,
        FLING,
        LYING_DOWN,
        SITTING,
        SKILL_ANIMATING,
        DRIVING_VEHICLE,
        RIDING_CREATURE,
        KNOCKED_DOWN,
        INCAPACITATED,
        DEAD,
        BLOCKING
    };

    enum STATE
    {
        COVER = 0,
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

    // CREO
    uint32_t GetType() const;
    const static uint32_t type = 0x4352454F;

    void SetBankCredits(uint32_t bank_credits);
    void SetCashCredits(uint32_t cash_credits);
    void AddSkill(std::string skill);
    void RemoveSkill(std::string skill);
    void SetPosture(POSTURE posture);
    void SetFactionRank(uint8_t faction_rank);
    void SetOwnerId(uint64_t owner_id);
    void SetScale(float scale);
    void SetBattleFatigue(uint32_t battle_fatigue);
    void SetStateBitmask(uint64_t state_bitmask);
    void SetAccelerationMultiplierBase(float acceleration_multiplier_base);
    void SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier);
    void SetSpeedMultiplierBase(float speed_multiplier_base);
    void SetSpeedMultiplierModifier(float speed_multiplier_modifier);
    void SetListenToId(uint64_t listen_to_id);
    void SetRunSpeed(float run_speed);
    void SetSlopeModifierAngle(float slope_modifier_angle);
    void SetSlopeModifierPercent(float slope_modifier_percent);
    void SetTurnRadius(float turn_radius);
    void SetWalkingSpeed(float walking_speed);
    void SetWaterModifierPercent(float water_modifier_percent);

    uint32_t GetBankCredits(void) const { return bank_credits_; }
    uint32_t GetCashCredits(void) const { return cash_credits_; }
    std::list<std::string>& GetSkillList(void) { return skill_list_; }
    uint8_t GetPosture(void) const { return posture_; }
    uint8_t GetFactionRank(void) const { return faction_rank_; }
    uint64_t GetOwnerId(void) const { return owner_id_; }
    float GetScale(void) const { return scale_; }
    uint32_t GetBattleFatigue(void) const { return battle_fatigue_; }
    uint64_t GetStateBitmask(void) const { return state_bitmask_; }
    float GetAccelerationMultiplierBase(void) const { return acceleration_multiplier_base_; }
    float GetAccelerationMultiplierModifier(void) const { return acceleration_multiplier_modifier_; }
    float GetSpeedMultiplierBase(void) const { return speed_multiplier_base_; }
    float GetSpeedMultiplierModifier(void) const { return speed_multiplier_modifier_; }
    uint64_t GetListenToId(void) const { return listen_to_id_; }
    float GetRunSpeed(void) const { return run_speed_; }
    float GetSlopeModifierAngle(void) const { return slope_modifier_angle_; }
    float GetSlopeModifierPercent(void) const { return slope_modifier_percent_; }
    float GetTurnRadius(void) const { return turn_radius_; }
    float GetWalkingSpeed(void) const { return walking_speed_; }
    float GetWaterModifierPercent(void) const { return water_modifier_percent_; }
    uint16_t GetCombatLevel(void) const { return combat_level_; }
    std::string GetAnimation(void) const { return animation_; }
    std::string GetMoodAnimation(void) const { return mood_animation_; }
    uint64_t GetWeaponId(void) const { return weapon_id_; }
    uint64_t GetGroupId(void) const { return group_id_; }
    uint64_t GetInviteSenderId(void) const { return invite_sender_id_; }
    uint32_t GetGuildId(void) const { return guild_id_; }
    uint64_t GetTargetId(void) const { return target_id_; }
    uint8_t GetMoodId(void) const { return mood_id_; }
    uint32_t GetPerformanceId(void) const { return performance_id_; }
    bool GetStationary(void) const { return stationary_; }

    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline1();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline4();
    virtual boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();

private:
    friend class CreatureMessageBuilder;

    uint32_t    bank_credits_;                          // update 1 variable 0
    uint32_t    cash_credits_;                          // update 1 variable 1
    uint32_t    skill_list_counter_;                    // update 1 variable 3
    std::list<std::string> skill_list_;                 // update 1 variable 3
    uint32_t    posture_;                               // update 3 variable 11
    uint8_t     faction_rank_;                          // update 3 variable 12
    uint64_t    owner_id_;                              // update 3 variable 13
    float       scale_;                                 // update 3 variable 14
    uint32_t    battle_fatigue_;                        // update 3 variable 15
    uint64_t    state_bitmask_;                         // update 3 variable 16
    float       acceleration_multiplier_base_;          // update 4 variable 0
    float       acceleration_multiplier_modifier_;      // update 4 variable 1
    float       speed_multiplier_base_;                 // update 4 variable 4
    float       speed_multiplier_modifier_;             // update 4 variable 5
    uint64_t    listen_to_id_;                          // update 4 variable 6
    float       run_speed_;                             // update 4 variable 7
    float       slope_modifier_angle_;                  // update 4 variable 8
    float       slope_modifier_percent_;                // update 4 variable 9
    float       turn_radius_;                           // update 4 variable 10
    float       walking_speed_;                         // update 4 variable 11
    float       water_modifier_percent_;                // update 4 variable 12
    uint16_t    combat_level_;                          // update 6 variable 2
    std::string animation_;                             // update 6 variable 3
    std::string mood_animation_;                        // update 6 variable 4
    uint64_t    weapon_id_;                             // update 6 variable 5
    uint64_t    group_id_;                              // update 6 variable 6
    uint64_t    invite_sender_id_;                      // update 6 variable 7
    uint64_t    invite_counter_;                        // update 6 variable 7
    uint32_t    guild_id_;                              // update 6 variable 8
    uint64_t    target_id_;                             // update 6 variable 9
    uint8_t     mood_id_;                               // update 6 variable 10
    uint32_t    performance_counter_;                   // update 6 variable 11
    uint32_t    performance_id_;                        // update 6 variable 12
    bool        stationary_;                            // update 6 variable 17

};

}}}  // namespace swganh::object::creature

#endif // SWGANH_OBJECT_CREATURE_CREATURE_H_

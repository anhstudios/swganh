// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <atomic>
#include <list>

#include <boost/thread/mutex.hpp>

#include "swganh_core/object/tangible/tangible.h"

#include "swganh_core/messages/containers/network_map.h"

#include "creature_offset.h"
#include "equipment_item.h"
#include "mission_critical_object.h"
#include "posture.h"
#include "pvp_status.h"
#include "skill_mod.h"
#include "stat_index.h"
#include "state.h"

namespace swganh {
namespace combat
{
	class BuffInterface;
}

namespace object {

typedef std::multimap<boost::posix_time::ptime, std::shared_ptr<swganh::combat::BuffInterface>> BuffMap;
typedef std::function<void(BuffMap::value_type)> BuffIterator;

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
    virtual ~Creature();

    // CREO
    uint32_t GetType() const;
    const static uint32_t type = 0x4352454F;

    // Bank Credits
    void SetBankCredits(uint32_t bank_credits);
	void SetBankCredits(uint32_t bank_credits, boost::unique_lock<boost::mutex>& lock);

    uint32_t GetBankCredits();
	uint32_t GetBankCredits(boost::unique_lock<boost::mutex>& lock);

    // Cash Credits
    void SetCashCredits(uint32_t cash_credits);
	void SetCashCredits(uint32_t cash_credits, boost::unique_lock<boost::mutex>& lock);

    uint32_t GetCashCredits();
	uint32_t GetCashCredits(boost::unique_lock<boost::mutex>& lock);

	// Helper to set all stats to a given value
	void SetAllStats(int32_t value);
	void SetAllStats(int32_t value, boost::unique_lock<boost::mutex>& lock);

    // Stat Base
    void SetStatBase(uint16_t stat_index, int32_t value);
	void SetStatBase(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void AddStatBase(uint16_t stat_index, int32_t value);
	void AddStatBase(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void DeductStatBase(uint16_t stat_index, int32_t value);
	void DeductStatBase(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

	std::vector<int32_t> GetBaseStats();
	std::vector<int32_t> GetBaseStats(boost::unique_lock<boost::mutex>& lock);

    int32_t GetStatBase(uint16_t stat_index);
	int32_t GetStatBase(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock);

	void SerializeBaseStats(swganh::messages::BaseSwgMessage* message);
	void SerializeBaseStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Skills
    void AddSkill(std::string skill);
	void AddSkill(std::string skill, boost::unique_lock<boost::mutex>& lock);

    void RemoveSkill(std::string skill);
	void RemoveSkill(std::string skill, boost::unique_lock<boost::mutex>& lock);

    std::set<std::string> GetSkills();
	std::set<std::string> GetSkills(boost::unique_lock<boost::mutex>& lock);

    bool HasSkill(std::string skill);
	bool HasSkill(std::string skill, boost::unique_lock<boost::mutex>& lock);

	void SerializeSkills(swganh::messages::BaseSwgMessage* message);
	void SerializeSkills(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Skill Commands
    std::map<uint32_t, std::string> GetSkillCommands();
	std::map<uint32_t, std::string> GetSkillCommands(boost::unique_lock<boost::mutex>& lock);

    bool HasSkillCommand(std::string skill);
	bool HasSkillCommand(std::string skill, boost::unique_lock<boost::mutex>& lock);

    void AddSkillCommand(std::pair<uint32_t, std::string> skill_command);
	void AddSkillCommand(std::pair<uint32_t, std::string> skill_command, boost::unique_lock<boost::mutex>& lock);

    void RemoveSkillCommand(std::string skill_command);
	void RemoveSkillCommand(std::string skill_command, boost::unique_lock<boost::mutex>& lock);

    // Posture
    void SetPosture(Posture posture);
	void SetPosture(Posture posture, boost::unique_lock<boost::mutex>& lock);

    Posture GetPosture();
	Posture GetPosture(boost::unique_lock<boost::mutex>& lock);

    // IsDead
    bool IsDead();
	bool IsDead(boost::unique_lock<boost::mutex>& lock);

    // IsIncapacitated
    bool IsIncapacitated();
	bool IsIncapacitated(boost::unique_lock<boost::mutex>& lock);

    // Faction Rank
    void SetFactionRank(uint8_t faction_rank);
	void SetFactionRank(uint8_t faction_rank, boost::unique_lock<boost::mutex>& lock);

    uint8_t GetFactionRank();
	uint8_t GetFactionRank(boost::unique_lock<boost::mutex>& lock);

    // Owner Id
    void SetOwnerId(uint64_t owner_id);
	void SetOwnerId(uint64_t owner_id, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetOwnerId();
	uint64_t GetOwnerId(boost::unique_lock<boost::mutex>& lock);

    // Scale
    void SetScale(float scale);
	void SetScale(float scale, boost::unique_lock<boost::mutex>& lock);

    float GetScale();
	float GetScale(boost::unique_lock<boost::mutex>& lock);

    // Battle Fatigue
    void AddBattleFatigue(uint32_t battle_fatigue);
	void AddBattleFatigue(uint32_t battle_fatigue, boost::unique_lock<boost::mutex>& lock);

    void SetBattleFatigue(uint32_t battle_fatigue);
	void SetBattleFatigue(uint32_t battle_fatigue, boost::unique_lock<boost::mutex>& lock);

    uint32_t GetBattleFatigue();
	uint32_t GetBattleFatigue(boost::unique_lock<boost::mutex>& lock);

    // State Bitmask
    void SetStateBitmask(uint64_t state_bitmask);
	void SetStateBitmask(uint64_t state_bitmask, boost::unique_lock<boost::mutex>& lock);

    void ToggleStateBitmask(uint64_t state_bitmask);
	void ToggleStateBitmask(uint64_t state_bitmask, boost::unique_lock<boost::mutex>& lock);

    void ToggleStateOn(uint64_t state);
	void ToggleStateOn(uint64_t state, boost::unique_lock<boost::mutex>& lock);

    void ToggleStateOff(uint64_t state);
	void ToggleStateOff(uint64_t state, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetStateBitmask();
	uint64_t GetStateBitmask(boost::unique_lock<boost::mutex>& lock);

    bool HasState(uint64_t state);
	bool HasState(uint64_t state, boost::unique_lock<boost::mutex>& lock);

    // Wounds
    void DeductStatWound(uint16_t stat_index, int32_t value);
	void DeductStatWound(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void AddStatWound(uint16_t stat_index, int32_t value);
	void AddStatWound(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void SetStatWound(uint16_t stat_index, int32_t value);
	void SetStatWound(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

	std::vector<int32_t> GetStatWounds();
	std::vector<int32_t> GetStatWounds(boost::unique_lock<boost::mutex>& lock);

    int32_t GetStatWound(uint16_t stat_index);
	int32_t GetStatWound(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock);

	void SerializeStatWounds(swganh::messages::BaseSwgMessage* message);
	void SerializeStatWounds(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Acceleration Multiplier Base
    void SetAccelerationMultiplierBase(float acceleration_multiplier_base);
	void SetAccelerationMultiplierBase(float acceleration_multiplier_base, boost::unique_lock<boost::mutex>& lock);

    float GetAccelerationMultiplierBase();
	float GetAccelerationMultiplierBase(boost::unique_lock<boost::mutex>& lock);

    // Acceleration Multiplier Modifier
    void SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier);
	void SetAccelerationMultiplierModifier(float acceleration_multiplier_modifier, boost::unique_lock<boost::mutex>& lock);

    float GetAccelerationMultiplierModifier();
	float GetAccelerationMultiplierModifier(boost::unique_lock<boost::mutex>& lock);

    // Stat Encumberance
    void AddStatEncumberance(uint16_t stat_index, int32_t value);
	void AddStatEncumberance(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void DeductStatEncumberance(uint16_t stat_index, int32_t value);
	void DeductStatEncumberance(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void SetStatEncumberance(uint16_t stat_index, int32_t value);
	void SetStatEncumberance(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

	std::vector<int32_t> GetStatEncumberances();
	std::vector<int32_t> GetStatEncumberances(boost::unique_lock<boost::mutex>& lock);

    int32_t GetStatEncumberance(uint16_t stat_index);
	int32_t GetStatEncumberance(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock);

	void SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message);
	void SerializeStatEncumberances(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Skill Mods
    void AddSkillMod(SkillMod mod);
	void AddSkillMod(SkillMod mod, boost::unique_lock<boost::mutex>& lock);

    void RemoveSkillMod(std::string identifier);
	void RemoveSkillMod(std::string identifier, boost::unique_lock<boost::mutex>& lock);

    void SetSkillMod(SkillMod mod);
	void SetSkillMod(SkillMod mod, boost::unique_lock<boost::mutex>& lock);

	std::map<std::string, SkillMod> GetSkillMods();
	std::map<std::string, SkillMod> GetSkillMods(boost::unique_lock<boost::mutex>& lock);

    SkillMod GetSkillMod(std::string identifier);
	SkillMod GetSkillMod(std::string identifier, boost::unique_lock<boost::mutex>& lock);

	void SerializeSkillMods(swganh::messages::BaseSwgMessage* message);
	void SerializeSkillMods(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Speed Multiplier Base
    void SetSpeedMultiplierBase(float speed_multiplier_base);
	void SetSpeedMultiplierBase(float speed_multiplier_base, boost::unique_lock<boost::mutex>& lock);

    float GetSpeedMultiplierBase();
	float GetSpeedMultiplierBase(boost::unique_lock<boost::mutex>& lock);

    // Speed Multiplier Modifier
    void SetSpeedMultiplierModifier(float speed_multiplier_modifier);
	void SetSpeedMultiplierModifier(float speed_multiplier_modifier, boost::unique_lock<boost::mutex>& lock);

    float GetSpeedMultiplierModifier();
	float GetSpeedMultiplierModifier(boost::unique_lock<boost::mutex>& lock);

    // Listen To Id
    void SetListenToId(uint64_t listen_to_id);
	void SetListenToId(uint64_t listen_to_id, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetListenToId();
	uint64_t GetListenToId(boost::unique_lock<boost::mutex>& lock);

    // Run Speed
    void SetRunSpeed(float run_speed);
	void SetRunSpeed(float run_speed, boost::unique_lock<boost::mutex>& lock);

    float GetRunSpeed();
	float GetRunSpeed(boost::unique_lock<boost::mutex>& lock);

    // Slop Modifier Angle
    void SetSlopeModifierAngle(float slope_modifier_angle);
	void SetSlopeModifierAngle(float slope_modifier_angle, boost::unique_lock<boost::mutex>& lock);

    float GetSlopeModifierAngle();
	float GetSlopeModifierAngle(boost::unique_lock<boost::mutex>& lock);

    // Slope Modifier Percent
    void SetSlopeModifierPercent(float slope_modifier_percent);
	void SetSlopeModifierPercent(float slope_modifier_percent, boost::unique_lock<boost::mutex>& lock);

    float GetSlopeModifierPercent();
	float GetSlopeModifierPercent(boost::unique_lock<boost::mutex>& lock);

    // Turn Radius
    void SetTurnRadius(float turn_radius);
	void SetTurnRadius(float turn_radius, boost::unique_lock<boost::mutex>& lock);

    float GetTurnRadius();
	float GetTurnRadius(boost::unique_lock<boost::mutex>& lock);

    // Walking Speed
    void SetWalkingSpeed(float walking_speed);
	void SetWalkingSpeed(float walking_speed, boost::unique_lock<boost::mutex>& lock);

    float GetWalkingSpeed();
	float GetWalkingSpeed(boost::unique_lock<boost::mutex>& lock);

    // Water Modifier Percent
    void SetWaterModifierPercent(float water_modifier_percent);
	void SetWaterModifierPercent(float water_modifier_percent, boost::unique_lock<boost::mutex>& lock);

    float GetWaterModifierPercent();
	float GetWaterModifierPercent(boost::unique_lock<boost::mutex>& lock);

    // Mission Critical Objects
    void AddMissionCriticalObject(MissionCriticalObject& object);
	void AddMissionCriticalObject(MissionCriticalObject& object, boost::unique_lock<boost::mutex>& lock);

    void RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id);
    void RemoveMissionCriticalObject(uint64_t mission_owner, uint64_t object_id, boost::unique_lock<boost::mutex>& lock);

    MissionCriticalObject GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner);
	MissionCriticalObject GetMissionCriticalObject(uint64_t object_id, uint64_t mission_owner, boost::unique_lock<boost::mutex>& lock);

    std::set<MissionCriticalObject> GetMissionCriticalObjects();
	std::set<MissionCriticalObject> GetMissionCriticalObjects(boost::unique_lock<boost::mutex>& lock);

	void SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message);
	void SerializeMissionCriticalObjects(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Combat Level
    void SetCombatLevel(uint16_t);
	void SetCombatLevel(uint16_t, boost::unique_lock<boost::mutex>& lock);

    uint16_t GetCombatLevel();
	uint16_t GetCombatLevel(boost::unique_lock<boost::mutex>& lock);

    // Animation
    void SetAnimation(std::string animation);
	void SetAnimation(std::string animation, boost::unique_lock<boost::mutex>& lock);

    std::string GetAnimation();
	std::string GetAnimation(boost::unique_lock<boost::mutex>& lock);

    // Mood Animation
    void SetMoodAnimation(std::string mood_animation);
	void SetMoodAnimation(std::string mood_animation, boost::unique_lock<boost::mutex>& lock);

    std::string GetMoodAnimation();
	std::string GetMoodAnimation(boost::unique_lock<boost::mutex>& lock);

    // Weapon Id
    void SetWeaponId(uint64_t weapon_id);
	void SetWeaponId(uint64_t weapon_id, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetWeaponId();
	uint64_t GetWeaponId(boost::unique_lock<boost::mutex>& lock);

    // Group Id
    void SetGroupId(uint64_t group_id);
	void SetGroupId(uint64_t group_id, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetGroupId();
	uint64_t GetGroupId(boost::unique_lock<boost::mutex>& lock);

    // Invite Sender Id
    void SetInviteSenderId(uint64_t invite_sender_id);
	void SetInviteSenderId(uint64_t invite_sender_id, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetInviteSenderId();
	uint64_t GetInviteSenderId(boost::unique_lock<boost::mutex>& lock);

    // Invite Counter
    void SetInviteCounter(uint64_t invite_counter);
	void SetInviteCounter(uint64_t invite_counter, boost::unique_lock<boost::mutex>& lock);

    uint64_t IncrementInviteCounter();
	uint64_t IncrementInviteCounter(boost::unique_lock<boost::mutex>& lock);

    uint64_t GetInviteCounter() const;
	uint64_t GetInviteCounter(boost::unique_lock<boost::mutex>& lock) const;

    // Guild Id
    void SetGuildId(uint32_t guild_id);
	void SetGuildId(uint32_t guild_id, boost::unique_lock<boost::mutex>& lock);

    uint32_t GetGuildId();
	uint32_t GetGuildId(boost::unique_lock<boost::mutex>& lock);

    // Target Id
    void SetTargetId(uint64_t target_id);
	void SetTargetId(uint64_t target_id, boost::unique_lock<boost::mutex>& lock);

    uint64_t GetTargetId();
	uint64_t GetTargetId(boost::unique_lock<boost::mutex>& lock);

    // Mood Id
    void SetMoodId(uint8_t mood_id);
	void SetMoodId(uint8_t mood_id, boost::unique_lock<boost::mutex>& lock);

    uint8_t GetMoodId() ;
	uint8_t GetMoodId(boost::unique_lock<boost::mutex>& lock) ;

    // Performance Id
    void SetPerformanceId(uint32_t performance_id);
	void SetPerformanceId(uint32_t performance_id, boost::unique_lock<boost::mutex>& lock);

    uint32_t GetPerformanceId();
	uint32_t GetPerformanceId(boost::unique_lock<boost::mutex>& lock);

    // Performance Counter
    void SetPerformanceCounter(uint32_t performance_counter);
	void SetPerformanceCounter(uint32_t performance_counter, boost::unique_lock<boost::mutex>& lock);

    uint32_t IncrementPerformanceCounter();
	uint32_t IncrementPerformanceCounter(boost::unique_lock<boost::mutex>& lock);

    uint32_t GetPerformanceCounter() const;
	uint32_t GetPerformanceCounter(boost::unique_lock<boost::mutex>& lock) const;

    // Current Stats
    void SetStatCurrent(uint16_t stat_index, int32_t value);
	void SetStatCurrent(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void AddStatCurrent(uint16_t stat_index, int32_t value);
	void AddStatCurrent(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void DeductStatCurrent(uint16_t stat_index, int32_t value);
	void DeductStatCurrent(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

	std::vector<int32_t> GetCurrentStats();
	std::vector<int32_t> GetCurrentStats(boost::unique_lock<boost::mutex>& lock);

    int32_t GetStatCurrent(uint16_t stat_index);
	int32_t GetStatCurrent(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock);

	void SerializeCurrentStats(swganh::messages::BaseSwgMessage* message);
	void SerializeCurrentStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Max Stats
    void SetStatMax(uint16_t stat_index, int32_t value);
	void SetStatMax(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void AddStatMax(uint16_t stat_index, int32_t value);
	void AddStatMax(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

    void DeductStatMax(uint16_t stat_index, int32_t value);
	void DeductStatMax(uint16_t stat_index, int32_t value, boost::unique_lock<boost::mutex>& lock);

	std::vector<int32_t> GetMaxStats();
	std::vector<int32_t> GetMaxStats(boost::unique_lock<boost::mutex>& lock);

	int32_t GetStatMax(uint16_t stat_index);
	int32_t GetStatMax(uint16_t stat_index, boost::unique_lock<boost::mutex>& lock);

	void SerializeMaxStats(swganh::messages::BaseSwgMessage* message);
	void SerializeMaxStats(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Equipment List
    void AddEquipmentItem(EquipmentItem& item);
	void AddEquipmentItem(EquipmentItem& item, boost::unique_lock<boost::mutex>& lock);

    void RemoveEquipmentItem(uint64_t object_id);
	void RemoveEquipmentItem(uint64_t object_id, boost::unique_lock<boost::mutex>& lock);

    void UpdateEquipmentItem(EquipmentItem& item);
	void UpdateEquipmentItem(EquipmentItem& item, boost::unique_lock<boost::mutex>& lock);

    std::vector<EquipmentItem> GetEquipment();
	std::vector<EquipmentItem> GetEquipment(boost::unique_lock<boost::mutex>& lock);

    EquipmentItem GetEquipmentItem(uint64_t object_id);
	EquipmentItem GetEquipmentItem(uint64_t object_id, boost::unique_lock<boost::mutex>& lock);

	void SerializeEquipment(swganh::messages::BaseSwgMessage* message);
	void SerializeEquipment(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    // Disguise
    void SetDisguise(std::string disguise);
	void SetDisguise(std::string disguise, boost::unique_lock<boost::mutex>& lock);
    
	std::string GetDisguise();
	std::string GetDisguise(boost::unique_lock<boost::mutex>& lock);

    // Stationary
    void SetStationary(bool stationary);
	void SetStationary(bool stationary, boost::unique_lock<boost::mutex>& lock);

    bool IsStationary();
	bool IsStationary(boost::unique_lock<boost::mutex>& lock);

    PvpStatus GetPvpStatus() const;
	PvpStatus GetPvpStatus(boost::unique_lock<boost::mutex>& lock) const;

    void SetPvPStatus(PvpStatus status);
	void SetPvPStatus(PvpStatus status, boost::unique_lock<boost::mutex>& lock);

    void TogglePvpStateOn(PvpStatus state);
	void TogglePvpStateOn(PvpStatus state, boost::unique_lock<boost::mutex>& lock);

    void TogglePvpStateOff(PvpStatus state);
	void TogglePvpStateOff(PvpStatus state, boost::unique_lock<boost::mutex>& lock);

    void TogglePvpState(PvpStatus state);
	void TogglePvpState(PvpStatus state, boost::unique_lock<boost::mutex>& lock);
	
    bool CheckPvpState(PvpStatus state) const;
	bool CheckPvpState(PvpStatus state, boost::unique_lock<boost::mutex>& lock) const;

    bool CanAttack(Creature* creature);
	bool CanAttack(Creature* creature, boost::unique_lock<boost::mutex>& lock);

    void AddToDuelList(uint64_t id);
	void AddToDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock);

    void RemoveFromDuelList(uint64_t id);
	void RemoveFromDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock);

	bool InDuelList(uint64_t id);
    bool InDuelList(uint64_t id, boost::unique_lock<boost::mutex>& lock);
    
	std::vector<uint64_t> GetDuelList();
	std::vector<uint64_t> GetDuelList(boost::unique_lock<boost::mutex>& lock);

	//Buffs
	bool HasBuff(std::string buff_name);
	bool HasBuff(std::string buff_name, boost::unique_lock<boost::mutex>& lock);

	void AddBuff(std::string buff_name, uint32_t duration=0);
	void AddBuff(std::string buff_name, boost::unique_lock<boost::mutex>& lock,  uint32_t duration=0);

	void __AddBuffInternal(boost::posix_time::ptime time, std::shared_ptr<swganh::combat::BuffInterface> buff, uint32_t duration);

	void RemoveBuff(std::string name);
	void RemoveBuff(std::string name, boost::unique_lock<boost::mutex>& lock);

	void ClearBuffs();
	void ClearBuffs(boost::unique_lock<boost::mutex>& lock);

	void ClearBuffs(boost::posix_time::ptime current_time);
	void ClearBuffs(boost::posix_time::ptime current_time, boost::unique_lock<boost::mutex>& lock);

	void CleanUpBuffs();
	void CleanUpBuffs(boost::unique_lock<boost::mutex>& lock);

	void ViewBuffs(BuffIterator functor);
	void ViewBuffs(BuffIterator functor, boost::unique_lock<boost::mutex>& lock);

    // Baselines
    virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);
	
    typedef swganh::ValueEvent<std::shared_ptr<Creature>> CreatureEvent;

private:
    uint32_t    bank_credits_;
    uint32_t    cash_credits_;
    swganh::containers::NetworkVector<int32_t> stat_base_list_;
    swganh::containers::NetworkSet<std::string> skills_;
    std::map<uint32_t, std::string> skill_commands_;
    uint32_t    posture_;
    uint8_t     faction_rank_;
    uint64_t    owner_id_;
    float       scale_;
    uint32_t    battle_fatigue_;
    uint64_t    state_bitmask_;
    swganh::containers::NetworkVector<int32_t> stat_wound_list_;
    float       acceleration_multiplier_base_;
    float       acceleration_multiplier_modifier_;
    swganh::containers::NetworkVector<int32_t> stat_encumberance_list_;
    swganh::containers::NetworkMap<std::string, SkillMod> skill_mod_list_;
    float       speed_multiplier_base_;
    float       speed_multiplier_modifier_;
    uint64_t    listen_to_id_;
    float       run_speed_;
    float       slope_modifier_angle_;
    float       slope_modifier_percent_;
    float       turn_radius_;
    float       walking_speed_;
    float       water_modifier_percent_;
    swganh::containers::NetworkSet<MissionCriticalObject> mission_critical_object_list_;
    uint16_t    combat_level_;
    std::string animation_;
    std::string mood_animation_;
    uint64_t    weapon_id_;
    uint64_t    group_id_;
    uint64_t    invite_sender_id_;
    uint64_t    invite_counter_;
    uint32_t    guild_id_;
    uint64_t    target_id_;
    uint8_t     mood_id_;
    uint32_t    performance_counter_;
    uint32_t    performance_id_;
    swganh::containers::NetworkVector<int32_t> stat_current_list_;
    swganh::containers::NetworkVector<int32_t> stat_max_list_;
    swganh::containers::NetworkVector<EquipmentItem> equipment_list_;
    std::string disguise_;
    bool stationary_;
    PvpStatus pvp_status_;
    std::vector<uint64_t> duel_list_;

	BuffMap buffs_;
};

}}  // namespace swganh::object

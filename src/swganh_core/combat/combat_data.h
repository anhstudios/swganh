// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <unordered_map>

#include <swganh/hash_string.h>
#include "swganh_core/command/command_properties.h"
#include "swganh/scripting/utilities.h"
#include <boost/python.hpp>

namespace swganh {
namespace combat {
    
/**
* Data used in combat
*/
typedef std::map<std::string, std::pair<uint32_t, uint32_t>>SkillModsMap;

struct CombatData : public swganh::command::CommandProperties
{
	CombatData(swganh::command::CommandProperties properties)
		: swganh::command::CommandProperties(properties)
		, min_damage(0)
		, max_damage(0)
		, damage_multiplier(1.0f)
		, accuracy_bonus(0)
		, weapon_accuracy(0)
		, speed_multiplier(0.0f)
		, attack_speed(0.0f)
		, pool(0)
		, attack_delay_chance(0)
		, state_duration(0)
		, dot_duration(0)
		, dot_type(0)
		, dot_pool(0)
		, dot_strength(0)
		, dot_potency(0.0f)
		, range(0)
		, cone_angle(0)
		, area_range(0)
		, animation_crc("")
		, combat_spam("")
		, health_hit_chance(55.0f)
		, action_hit_chance(32.0f)
		, mind_hit_chance(13.0f)
	{}
	
    int min_damage;
    int max_damage;

    float damage_multiplier;
    int accuracy_bonus;
	int weapon_accuracy;
    float speed_multiplier;
	float attack_speed;
    int pool;
    
    int attack_delay_chance;
    int state_duration;

    // DoTs
    uint32_t dot_duration;
    uint64_t dot_type;
    uint8_t dot_pool;
    uint32_t dot_strength;
    float dot_potency;

    int range;
    int cone_angle;
    int area_range;

    swganh::HashString animation_crc;
    
    std::string combat_spam;

    float health_hit_chance;
    float action_hit_chance;
    float mind_hit_chance;

	SkillModsMap attacker_skill_mods;
	SkillModsMap target_skill_mods;

    void GetPythonData(boost::python::object global);

    bool IsRandomPool();
    int GetDamagingPool();
	static std::string COMBAT_EFFECTS() { return "@combat_effects:"; }
    static std::string HIT_spam() { return "hit"; }
    static std::string BLOCK_spam() { return "block"; }
    static std::string DODGE_spam() { return "evade"; }
    static std::string COUNTER_spam() { return "counter"; }
    static std::string MISS_spam() { return "miss"; }
        
    const static uint32_t DefaultAttacks[9];
};

}} // swganh::combat

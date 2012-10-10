// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <unordered_map>

#include <swganh/hash_string.h>
#include "swganh/command/command_properties.h"
#include "swganh/scripting/utilities.h"
#include <boost/python.hpp>

namespace swganh {
namespace combat {
    
/**
* Data used in combat
*/
struct CombatData : public swganh::command::CommandProperties
{
	CombatData(swganh::command::CommandProperties properties)
		: swganh::command::CommandProperties(properties)
		, min_damage(0)
		, max_damage(0)
		, damage_multiplier(0.0f)
		, accuracy_bonus(0)
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
		, health_hit_chance(0.0f)
		, action_hit_chance(0.0f)
		, mind_hit_chance(0.0f)
	{}
	
    int min_damage;
    int max_damage;

    float damage_multiplier;
    int accuracy_bonus;
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

    void GetPythonData(boost::python::object global);

    bool IsRandomPool();
    int GetDamagingPool();
    
    static std::string HIT_spam() { return "_hit"; }
    static std::string BLOCK_spam() { return "_block"; }
    static std::string DODGE_spam() { return "_evade"; }
    static std::string COUNTER_spam() { return "_counter"; }
    static std::string MISS_spam() { return "_miss"; }
        
    const static uint32_t DefaultAttacks[9];
};

}} // swganh::combat

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
namespace command {
    class BaseCombatCommand;
}
}

namespace swganh {
namespace combat {
    
struct CombatData : swganh::command::CommandProperties
{
    // TEMP
    class StateEffect;
    class DotEffect;
    CombatData(swganh::command::CommandProperties&& properties) 
        : swganh::command::CommandProperties(properties)
        , min_damage(0)
        , max_damage(0)
        , damage_multiplier(0)
        , accuracy_bonus(0)
        , speed_multiplier(0)
        , pool(0)
        , attack_delay_chance(0)
        , state_duration(0)
        , dot_duration(0)
        , dot_type(0)
        , dot_pool(0)
        , dot_strength(0)
        , dot_potency(0)
        , range (0)
        , cone_angle(0)
        , area_range(0)
        , animation_crc(swganh::HashString(""))
    {
    }

    explicit CombatData(swganh::command::BaseCombatCommand* command);
    CombatData(boost::python::object p_object, swganh::command::CommandProperties& properties );

    int min_damage;
    int max_damage;

    float damage_multiplier;
    int accuracy_bonus;
    float speed_multiplier;
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
    
    template <typename T>
    void ExtractData(boost::python::object& p_object, std::string key, T& extract_value)
    {
        swganh::scripting::ScopedGilLock lock;
        if (p_object.contains(key))
        {
            boost::python::extract<T> tmp_x(p_object[key]);
            if (tmp_x.check())
                extract_value = tmp_x();
        }
    }
    
    static std::string HIT_spam() { return "_hit"; }
    static std::string BLOCK_spam() { return "_block"; }
    static std::string DODGE_spam() { return "_evade"; }
    static std::string COUNTER_spam() { return "_counter"; }
    static std::string MISS_spam() { return "_miss"; }
        
    const static uint32_t DefaultAttacks[9];
};

}} // swganh::combat

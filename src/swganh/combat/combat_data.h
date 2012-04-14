#ifndef SWGANH_COMBAT_COMBAT_DATA_H_
#define SWGANH_COMBAT_COMBAT_DATA_H_

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <unordered_map>

#include <anh/hash_string.h>
#include "swganh/command/command_properties.h"

#include <boost/python.hpp>

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
        , animation_crc(anh::HashString(""))
    {
    }
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

    anh::HashString animation_crc;

    void GetPythonData(boost::python::object global);

    bool IsRandomPool();
    int GetDamagingPool();
    
    template <typename T>
    void ExtractData(boost::python::object& p_object, std::string key, T& extract_value)
    {
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

#endif  // SWGANH_COMBAT_COMBAT_DATA_H_

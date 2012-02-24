#include <cstdint>
#include <unordered_map>

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
    {
    }
    CombatData(boost::python::object p_object, swganh::command::CommandProperties& properties )
        : swganh::command::CommandProperties(properties) 
    {
        GetPythonData(p_object);
    }

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

    uint32_t animation_crc;

    void GetPythonData(boost::python::object global);

    //std::unordered_map<uint64_t, StateEffect> state_effects;
    //std::unordered_map<uint64_t, DotEffect> dot_effects;
    
    static std::string HIT_spam() { return "_hit"; }
    static std::string BLOCK_spam() { return "_block"; }
    static std::string DODGE_spam() { return "_evade"; }
    static std::string COUNTER_spam() { return "_counter"; }
    static std::string MISS_spam() { return "_miss"; }
        
    const static uint32_t DefaultAttacks[9];
};

}} // swganh::combat
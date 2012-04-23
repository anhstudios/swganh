#ifndef SWGANH_COMBAT_COMBAT_DATA_H_
#define SWGANH_COMBAT_COMBAT_DATA_H_

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <unordered_map>

#include <anh/hash_string.h>
#include "swganh/command/command.h"

#include <boost/python.hpp>

namespace swganh {
namespace combat {

struct CombatCommand : swganh::command::Command
{
    // TEMP
    class StateEffect;
    class DotEffect;

    explicit CombatCommand(swganh::command::Command& command) ;
    CombatCommand();

    uint32_t min_damage;
    uint32_t max_damage;

    float damage_multiplier;
    uint32_t accuracy_bonus;
    float speed_multiplier;
    int8_t pool;

    std::string combat_spam;
    uint8_t trail1;
    uint8_t trail2;
    float health_hit_chance;
    float action_hit_chance;
    float mind_hit_chance;
    float knockdown_hit_chance;
    float dizzy_hit_chance;
    float blind_chance;
    float stun_chance;
    float intimidate_chance;
    float posture_down_chance;
    float extended_range;
    float cone_angle;
    float range;
    float area_range;

    uint32_t attack_delay_chance;
    uint32_t state_duration;

    // DoTs
    uint32_t dot_duration;
    uint64_t dot_type;
    uint8_t dot_pool;
    uint32_t dot_strength;
    float dot_potency;

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

#endif  // SWGANH_COMBAT_COMBAT_DATA_H_

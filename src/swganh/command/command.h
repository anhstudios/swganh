#ifndef SWGANH_COMMAND_COMMAND_H_
#define SWGANH_COMMAND_COMMAND_H_

#include <cstdint>
#include <string>
#include <vector>
#include <map>

namespace swganh {
namespace command {

class Command
{
public:
    Command(void)
        : name("")
        , name_crc(0)
        , ability("")
        , ability_crc(0)
        , allow_in_states(0)
        , skill_mods(std::vector<std::string>())
        , script_hook("")
        , fail_script_hook("")
        , admin_level(0)
        , default_time(0.0f)
        , default_priority(0)
        , command_group(0)
        , max_range_to_target(0)
        , add_to_combat_queue(0)
        , target_type(0)
        , health_cost(0)
        , health_cost_multiplier(0)
        , action_cost(0)
        , action_cost_multiplier(0)
        , mind_cost(0)
        , mind_cost_multiplier(0)
        , delay_multiplier(0)
        , force_cost(0)
        , force_cost_multiplier(0)
        , animation_crc(0)
        , required_weapon_group(0)
        , deny_in_locomotion(0)
        , allow_in_posture(0)
    {}

    virtual void Setup(){}

    virtual void Run(){}

    virtual void Callback(){}

    std::string name;
    uint32_t name_crc;
    std::string ability;
    uint32_t ability_crc;
    uint64_t allow_in_states;
    std::vector<std::string> skill_mods;
    std::string script_hook;
    std::string fail_script_hook;
    uint16_t admin_level;
    float default_time;
    uint8_t default_priority;
    int32_t command_group;
    float max_range_to_target;
    uint8_t add_to_combat_queue;
    uint8_t target_type;
    uint32_t health_cost;
    uint32_t health_cost_multiplier;
    uint32_t action_cost;
    uint32_t action_cost_multiplier;
    uint32_t mind_cost;
    uint32_t mind_cost_multiplier;
    float delay_multiplier;
    uint32_t force_cost;
    uint32_t force_cost_multiplier;
    uint32_t animation_crc;
    uint32_t required_weapon_group;
    uint64_t deny_in_locomotion;
    uint64_t allow_in_posture;

    static uint64_t BuildBitmask(std::vector<int> bits)
    {
        uint64_t bitmask = 0;
        int counter = 0;
        for (auto& bit : bits)
        {
            bitmask += bit << counter++;
        }
        return bitmask;
    }
    template <typename T, typename Q>
    void MergeValue(bool condition, T& out_value, Q& in_value)
    {
        if (condition)
        {
            out_value = in_value;
        }
    }

    void MergeCommand(Command& command)
    {
        // Only some things make sense to merge
        MergeValue(command.ability.length() > 0, ability, command.ability);
        MergeValue(command.skill_mods.size() > 0, skill_mods, command.skill_mods);
        MergeValue(command.allow_in_states > 0, allow_in_states, command.allow_in_states);
        MergeValue(command.allow_in_posture > 0, allow_in_posture, command.allow_in_posture);
        MergeValue(command.deny_in_locomotion > 0, deny_in_locomotion, command.deny_in_locomotion);
        MergeValue(command.admin_level > 0, admin_level, command.admin_level);
        MergeValue(command.default_time > 0.0f, default_time, command.default_time);
        MergeValue(command.default_priority > 0, default_priority, command.default_priority);
        MergeValue(command.command_group > 0, command_group, command.command_group);
        MergeValue(command.max_range_to_target > 0.0f, max_range_to_target, command.max_range_to_target);
        MergeValue(command.target_type > 0, target_type, command.target_type);
        MergeValue(command.health_cost > 0, health_cost, command.health_cost);
        MergeValue(command.health_cost_multiplier > 0, health_cost_multiplier, command.health_cost_multiplier);
        MergeValue(command.action_cost > 0, action_cost, command.action_cost);
        MergeValue(command.action_cost_multiplier > 0, action_cost_multiplier, command.action_cost_multiplier);
        MergeValue(command.mind_cost > 0, mind_cost, command.mind_cost);
        MergeValue(command.mind_cost_multiplier > 0, mind_cost_multiplier, command.mind_cost_multiplier);
        MergeValue(command.delay_multiplier > 0.0f, delay_multiplier, command.delay_multiplier);
        MergeValue(command.force_cost > 0, force_cost, command.force_cost);
        MergeValue(command.force_cost_multiplier > 0, force_cost_multiplier, command.force_cost_multiplier);
        MergeValue(command.animation_crc > 0, animation_crc, command.animation_crc);
        MergeValue(command.required_weapon_group > 0, required_weapon_group, command.required_weapon_group);
    }

};

}} // swganh::command
#endif //  SWGANH_COMMAND_COMMAND_H_
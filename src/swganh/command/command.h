#ifndef SWGANH_COMMAND_COMMAND_H_
#define SWGANH_COMMAND_COMMAND_H_

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

#include "anh/hash_string.h"

namespace swganh {

//forward declare
namespace app {
    class SwganhKernel;
}
namespace object {
namespace tangible {
    class Tangible;
}
namespace creature {
	class Creature;
}} // namespace object::creature
namespace messages {
namespace controllers {
class CommandQueueEnqueue;
}}
namespace command {

class Command
{

public:
    Command(void);
    virtual void Setup();

    virtual void Run(swganh::app::SwganhKernel* kernel,
		const std::shared_ptr<swganh::object::creature::Creature>& actor, // creature object
		const std::shared_ptr<swganh::object::tangible::Tangible>& target,	// target object
        const std::string& command_string);

    anh::HashString name;
    anh::HashString ability;
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
    anh::HashString animation;
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
    void MergeValue(bool condition,  T& local_value, Q& in_value)
    {
        if (condition)
        {
            local_value = in_value;
        }
    }
    // Merging from Client Data
    void MergeCommand(Command& command);

    std::function<void (
        swganh::app::SwganhKernel*,
		const std::shared_ptr<swganh::object::creature::Creature>&, // creature object
		const std::shared_ptr<swganh::object::tangible::Tangible>&,	// target object
        const swganh::messages::controllers::CommandQueueEnqueue&)> GetHandler();
};

}} // swganh::command
#endif //  SWGANH_COMMAND_COMMAND_H_
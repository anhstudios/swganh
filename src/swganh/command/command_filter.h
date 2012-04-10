
#ifndef SWGANH_COMMAND_COMMAND_FILTER_H_
#define SWGANH_COMMAND_COMMAND_FILTER_H_

#include <tuple>
#include <memory>
#include <cstdint>

namespace swganh {
namespace messages {
namespace controllers {
class CommandQueueEnqueue;
}} // messages::controllers
namespace object {
class Object;
namespace tangible {
    class Tangible;
}
namespace creature {
class Creature;
}} // object::creature
namespace command {

struct CommandProperties;

enum ERROR
{
	CANNOT_WHILE_IN_POSTURE = 1,
	INSUFFICIENT_ABILITY = 2,
	INVALID_TARGET = 3,
	OUT_OF_RANGE = 4,
	CANNOT_WHILE_IN_STATE = 5,
	GOD_LEVEL = 6
};

struct CommandFilters {

static std::tuple<bool, uint32_t, uint32_t> TargetCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::tangible::Tangible>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> PostureCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::tangible::Tangible>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> StateCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::tangible::Tangible>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> AbilityCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::tangible::Tangible>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> CombatTargetCheckFilter(
        const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::tangible::Tangible>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

    /**
    * gets the lowest common bit from two bit masks.
    *
    * it does this by checking if each bit from the playerMask to see if it
    * matches the cmdPropertiesMask. The cmdPropertiesMask is generally imported
    * from the database, but it could be any mask you want to use. It then
    * returns the first bit that was matched.
    *
    * @param creature_mask
    *   creature_mask is the first mask, which is to be checked against the second.
    * @param cmd_properties_mask
    *   cmdPropertiesMask is the mask the first mask checks against.
    *
    * @return first_common_bit
    *   This returns the the first common bit in between the two masks.
    */
    static uint32_t  GetLowestCommonBit(uint64_t creature_mask, uint64_t cmd_properties_mask);
};

}} // swganh::command

#endif // SWGANH_COMMAND_COMMAND_FILTER_H_
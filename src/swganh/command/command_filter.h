
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
		const std::shared_ptr<swganh::object::Object>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> PostureCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::Object>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> StateCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::Object>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);

static std::tuple<bool, uint32_t, uint32_t> AbilityCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::Object>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties);
};

}} // swganh::command

#endif // SWGANH_COMMAND_COMMAND_FILTER_H_
#include "command_filter.h"
#include "command_properties.h"
#include "command_service.h"
#include "swganh/object/creature/creature.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

using namespace swganh::command;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::messages::controllers;
using namespace std;


std::tuple<bool, uint32_t, uint32_t> CommandFilters::RangeCheckFilter(
		const std::shared_ptr<swganh::object::creature::Creature>& actor, 
		const std::shared_ptr<swganh::object::Object>& target, 
		const swganh::messages::controllers::CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties)
{
	bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
	// range check
	if (actor->InRange(target->GetPosition(), command_properties.max_range_to_target))
	{
		check_passed = true;
	}
	else
	{
		error = OUT_OF_RANGE;
	}
	return tie(check_passed, error, action);
}
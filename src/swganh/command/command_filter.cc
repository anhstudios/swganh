#include "command_filter.h"
#include "command_properties.h"
#include "command_service.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/player/player.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

using namespace swganh::command;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;
using namespace swganh::messages::controllers;
using namespace std;

tuple<bool, uint32_t, uint32_t> CommandFilters::TargetCheckFilter(
		const shared_ptr<Creature>& actor, 
		const shared_ptr<Tangible>& target, 
		const CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties)
{
	bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
	// does command require target?
	if (target != nullptr)
    {
        // range check
		if (command_properties.max_range_to_target <= 0.0f || actor->InRange(target->GetPosition(), command_properties.max_range_to_target))
		{
			check_passed = true;
		}
		else
		{
			error = OUT_OF_RANGE;
		}
    }
	// for now if no target return true
	else
	{
		check_passed = true;
	}
	return tie(check_passed, error, action);
}

tuple<bool, uint32_t, uint32_t> CommandFilters::PostureCheckFilter(
		const shared_ptr<Creature>& actor, 
		const shared_ptr<Tangible>& target, 
		const CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties)
{
	bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
	uint32_t current_posture = actor->GetPosture();
	if ((current_posture & command_properties.allow_in_posture) == current_posture)
	{
		check_passed = true;
	}
	else
	{
		error = CANNOT_WHILE_IN_POSTURE;
		action = current_posture;
	}
	return tie (check_passed, error, action);
}

tuple<bool, uint32_t, uint32_t> CommandFilters::StateCheckFilter(
		const shared_ptr<Creature>& actor, 
		const shared_ptr<Tangible>& target, 
		const CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties)
{
	bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;

    /// @TODO This is not the correct way to get a state out of the bitmask, revisit later.
	uint32_t current_state = static_cast<uint32_t>(actor->GetStateBitmask());
	if (command_properties.deny_in_states == 0 ||
        (current_state & command_properties.deny_in_states) != command_properties.deny_in_states)
	{
		check_passed = true;
	}
	else
	{
		error = CANNOT_WHILE_IN_STATE;
		action = current_state;
	}
	return tie (check_passed, error, action);
}

tuple<bool, uint32_t, uint32_t> CommandFilters::AbilityCheckFilter(
		const shared_ptr<Creature>& actor, 
		const shared_ptr<Tangible>& target, 
		const CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties)
{
    bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
	//uint32_t current_state = static_cast<uint32_t>(actor->GetStateBitmask());
	// check to see if this command requires an ability
	if (command_properties.ability.length() > 0)
	{
        if (actor->HasSkillCommand(command_properties.ability))
		{
			check_passed = true;
		}
		else
		{
			error = INSUFFICIENT_ABILITY;
		}
    }
	// doesn't require ability
	else
		check_passed = true;

	return tie (check_passed, error, action);
}
std::tuple<bool, uint32_t, uint32_t> CommandFilters::CombatTargetCheckFilter(
		const shared_ptr<Creature>& actor, 
		const shared_ptr<Tangible>& target, 
		const CommandQueueEnqueue& command_queue_enqueue,
        const CommandProperties& command_properties)
{
    bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
    // Command groups 1 and 2 are combat commands
    if (command_properties.command_group != 0 && target != nullptr) 
    {
        if (target->GetType() == Creature::type)
        {
            auto creature = static_pointer_cast<Creature>(target);
	        if (actor->CanAttack(creature.get()))
	        {
	            check_passed = true;
	        }
            else
	        {
		        error = INVALID_TARGET;
            }
        }
    }
    else
    {
        // Action Check?
        check_passed = true;
    }
    return tie (check_passed, error, action);
}
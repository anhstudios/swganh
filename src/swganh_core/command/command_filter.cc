// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_filter.h"

#include "swganh_core/command/base_swg_command.h"
#include "swganh_core/command/command_interface.h"
#include "swganh_core/command/command_properties.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"

using swganh::command::CommandFilters;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandInterface;

using namespace swganh::object;


std::tuple<bool, uint32_t, uint32_t> CommandFilters::TargetCheckFilter(CommandInterface* command)
{
    BaseSwgCommand* swg_command = static_cast<BaseSwgCommand*>(command);

	bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
	// does command require target?
    if (swg_command->GetTargetRequiredType() == 1 )
    {
		if (swg_command->GetTarget() != nullptr)
		{
			// range check
			if (swg_command->GetMaxRangeToTarget() <= 0.0f || swg_command->GetActor()->InRange(swg_command->GetTarget()->GetPosition(), swg_command->GetMaxRangeToTarget()))
			{
				check_passed = true;
			}
			else
			{
				error = OUT_OF_RANGE;
			}
		}
		else
			error = INVALID_TARGET;
    }
	// for now if no target return true
	else
	{
		check_passed = true;
	}
	return std::tie(check_passed, error, action);
}

std::tuple<bool, uint32_t, uint32_t> CommandFilters::PostureCheckFilter(CommandInterface* command)
{
    BaseSwgCommand* swg_command = static_cast<BaseSwgCommand*>(command);
	std::shared_ptr<Creature> creature;
	if (swg_command->GetActor()->GetType() == Creature::type)
		creature = std::static_pointer_cast<Creature>(swg_command->GetActor());
	
	bool check_passed = true;
	uint32_t error = 0;
	uint32_t action = 0;
	if (creature)
	{
		uint32_t current_posture = creature->GetPosture();
		if ((current_posture & swg_command->GetAllowedLocomotionBitmask()) == current_posture)
		{
			check_passed = true;
		}
		else
		{
			error = CANNOT_WHILE_IN_POSTURE;
			action = current_posture;
		}
	}
	return std::tie (check_passed, error, action);
}

std::tuple<bool, uint32_t, uint32_t> CommandFilters::StateCheckFilter(CommandInterface* command)
{
    BaseSwgCommand* swg_command = static_cast<BaseSwgCommand*>(command);

	bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;

	auto actor = std::static_pointer_cast<Creature>(swg_command->GetActor());
	if(actor)
	{
		uint64_t current_state = actor->GetStateBitmask();
		if (!actor->HasState(swg_command->GetAllowedStateBitmask()))
		{
			check_passed = true;
		}
		else
		{
			error = CANNOT_WHILE_IN_STATE;
			action = GetLowestCommonBit(current_state, swg_command->GetAllowedStateBitmask());
		}
	}
	return std::tie (check_passed, error, action);
}

std::tuple<bool, uint32_t, uint32_t> CommandFilters::AbilityCheckFilter(CommandInterface* command)
{
    BaseSwgCommand* swg_command = static_cast<BaseSwgCommand*>(command);

    bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;
	// check to see if this command requires an ability

    auto required_ability = swg_command->GetRequiredAbility();

    if (required_ability.length() > 0)
	{
		auto actor = std::static_pointer_cast<Creature>(swg_command->GetActor());
        if (actor != nullptr && actor->HasSkillCommand(required_ability))
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

	return std::tie (check_passed, error, action);
}

std::tuple<bool, uint32_t, uint32_t> CommandFilters::CombatTargetCheckFilter(CommandInterface* command)
{
    BaseSwgCommand* swg_command = static_cast<BaseSwgCommand*>(command);

    bool check_passed = false;
	uint32_t error = 0;
	uint32_t action = 0;

    auto target = swg_command->GetTarget();
    if (swg_command->GetCommandGroup() != 0 && target != nullptr) 
    {
        if (target->GetType() == Creature::type)
        {
            auto creature = std::static_pointer_cast<Creature>(target);
            // @TODO: Fix this, we need more checking in here
            // target type of 2 seems to suggest the action NEEDs a target and 1 that it CAN have a target
            switch(swg_command->GetTargetRequiredType())
            {
                case 0:
                case 1:
                    check_passed = true;
                    break;
                case 2:
                    if (creature->CanAttack(creature.get()))
                        check_passed = true;
                    break;
            }
        }
    }
    else
    {
        // Action Check?
        check_passed = true;
    }
    if (!check_passed) error = INVALID_TARGET;

    return std::tie (check_passed, error, action);
}

uint32_t CommandFilters::GetLowestCommonBit(uint64_t creature_mask, uint64_t command_properties_mask)
{
    // checks each bit and returns the value
    bool found = false;
    uint32_t i = 0;
    for (; i < 64 && !found; ++i) {
        found = (creature_mask & (command_properties_mask << i)) != 0;
    }
    if (found) {
        return i;
    }
    return 0;
}
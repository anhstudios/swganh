// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_swg_command.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

using swganh::command::BaseSwgCommand;
using swganh::command::CommandProperties;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::creature::Creature;
using swganh::object::tangible::Tangible;

BaseSwgCommand::BaseSwgCommand(
    swganh::app::SwganhKernel* kernel,
    const CommandProperties& properties,
    const std::shared_ptr<Creature>& actor,
    const std::shared_ptr<Tangible>& target,
    const CommandQueueEnqueue& command_request)
    : kernel_(kernel)
    , properties_(properties)
    , actor_(actor)
    , target_(target)
    , command_request_(command_request)
{}

BaseSwgCommand::~BaseSwgCommand()
{}

swganh::app::SwganhKernel* BaseSwgCommand::GetKernel() const
{
    return kernel_;
}

const CommandProperties& BaseSwgCommand::GetProperties() const
{
    return properties_;
}
        
std::string BaseSwgCommand::GetCommandName() const
{
    return properties_.command_name.ident_string();
}

uint32_t BaseSwgCommand::GetActionCounter() const
{
    return command_request_.action_counter;
}

uint32_t BaseSwgCommand::GetPriority() const
{
    return properties_.default_priority;
}

uint32_t BaseSwgCommand::GetCommandGroup() const
{
    return properties_.command_group;
}

uint32_t BaseSwgCommand::GetTargetRequiredType() const
{
    return properties_.target_type;
}

uint64_t BaseSwgCommand::GetAllowedStateBitmask() const
{
    return properties_.allow_in_state;
}

float BaseSwgCommand::GetMaxRangeToTarget() const
{
    return properties_.max_range_to_target;
}

float BaseSwgCommand::GetDefaultTime() const
{
    return properties_.default_time;
}
        
std::string BaseSwgCommand::GetRequiredAbility() const
{
    return properties_.character_ability.ident_string();
}

bool BaseSwgCommand::IsQueuedCommand() const
{
    return properties_.add_to_combat_queue != 0;
}

const std::shared_ptr<Creature>& BaseSwgCommand::GetActor() const
{
    return actor_;
}

const std::shared_ptr<Tangible>& BaseSwgCommand::GetTarget() const
{
    return target_;
}
        
const std::wstring& BaseSwgCommand::GetCommandString() const
{
    return command_request_.command_options;
}

const CommandQueueEnqueue& BaseSwgCommand::GetCommandRequest() const
{
    return command_request_;
}

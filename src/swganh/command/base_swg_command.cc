// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_swg_command.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

using swganh::command::BaseSwgCommand;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::creature::Creature;
using swganh::object::tangible::Tangible;

BaseSwgCommand::BaseSwgCommand(
    swganh::app::SwganhKernel* kernel,
    const std::shared_ptr<Creature>& actor,
    const std::shared_ptr<Tangible>& target,
    const CommandQueueEnqueue& command_request)
    : kernel_(kernel)
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

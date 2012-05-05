// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_factory.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/command/command_interface.h"
#include "swganh/command/command_properties.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

using pub14_core::command::CommandFactory;
using swganh::app::SwganhKernel;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::messages::controllers::CommandQueueEnqueue;

CommandFactory::~CommandFactory()
{}

void AddCommandCreator(anh::HashString command, swganh::command::CommandCreator&& creator)
{}

void RemoveCommandCreator(anh::HashString command)
{}

std::unique_ptr<CommandInterface> CreateCommand(
    SwganhKernel* kernel,
    const CommandProperties& properties,
    const CommandQueueEnqueue& command_request)
{
    return nullptr;
}

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

void CommandFactory::AddCommandCreator(anh::HashString command, swganh::command::CommandCreator&& creator)
{
    boost::lock_guard<boost::mutex> lg(creators_mutex_);
    command_creators_[command] = std::move(creator);
}

void CommandFactory::RemoveCommandCreator(anh::HashString command)
{
    boost::lock_guard<boost::mutex> lg(creators_mutex_);
    command_creators_.erase(command);
}

std::unique_ptr<CommandInterface> CommandFactory::CreateCommand(
    SwganhKernel* kernel,
    const CommandProperties& properties,
    const CommandQueueEnqueue& command_request)
{
    boost::lock_guard<boost::mutex> lg(creators_mutex_);

    std::unique_ptr<CommandInterface> command = nullptr;

    auto creators_iter = command_creators_.find(command_request.command_crc);
    if (creators_iter != command_creators_.end())
    {
        command = creators_iter->second(kernel, properties, command_request);
    }

    return command;
}

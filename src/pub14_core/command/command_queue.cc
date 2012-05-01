// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue.h"

using pub14_core::command::CommandQueue;
using swganh::command::v2::CommandInterface;

CommandQueue::CommandQueue()
{}

CommandQueue::~CommandQueue()
{}

void CommandQueue::SetQueueOwner(const std::shared_ptr<swganh::object::ObjectController>& queue_owner)
{
    queue_owner_ = queue_owner;
}

const std::shared_ptr<swganh::object::ObjectController>& CommandQueue::GetQueueOwner() const
{
    return queue_owner_;
}

void CommandQueue::EnqueueCommand(std::unique_ptr<CommandInterface> command)
{
    
}

void CommandQueue::SetAutoCommand(std::unique_ptr<CommandInterface> command)
{
    auto_command_ = std::move(command);
}

void CommandQueue::RemoveAutoCommand()
{
    auto_command_.release();
}
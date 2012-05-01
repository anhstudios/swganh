// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue_manager.h"

#include "swganh/simulation/simulation_service.h"

using pub14_core::command::CommandQueue;
using pub14_core::command::CommandQueueManager;
using swganh::command::v2::CommandInterface;
using swganh::command::v2::CommandQueueInterface;
using swganh::object::ObjectController;

CommandQueueManager::CommandQueueManager()
{}

CommandQueueManager::~CommandQueueManager()
{}

CommandQueue* CommandQueueManager::CreateCommandQueue(uint64_t object_id)
{
    auto command_queue = FindCommandQueue(object_id);

    if (!command_queue)
    {
        std::unique_ptr<CommandQueue> new_queue(new CommandQueue);
        command_queue = new_queue.get();

        command_queue_map_.insert(std::make_pair(object_id, std::move(new_queue)));
    }

    return command_queue;
}

void CommandQueueManager::DestroyCommandQueue(uint64_t object_id)
{
    command_queue_map_.erase(object_id);
}

CommandQueue* CommandQueueManager::FindCommandQueue(uint64_t object_id)
{
    CommandQueue* command_queue = nullptr;

    auto find_iter = command_queue_map_.find(object_id);
    if (find_iter != command_queue_map_.end())
    {
        command_queue = find_iter->second.get();
    }

    return command_queue;
}

void CommandQueueManager::EnqueueCommand(std::unique_ptr<swganh::command::v2::CommandInterface> command)
{
    auto command_queue = FindCommandQueue(command->GetController()->GetId());

    if (command_queue)
    {
        command_queue->EnqueueCommand(std::move(command));
    }
}

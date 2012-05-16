// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue_manager.h"

#include "swganh/command/command_interface.h"
#include "swganh/command/command_queue_interface.h"

#include "swganh/object/object_controller.h"

using pub14_core::command::CommandQueueManager;
using swganh::command::CommandInterface;
using swganh::command::CommandQueueInterface;

CommandQueueManager::~CommandQueueManager()
{}

void CommandQueueManager::EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    
    auto find_iter = queue_map_.find(command->GetController()->GetId());
    if (find_iter != queue_map_.end() )
    {
        find_iter->second->EnqueueCommand(command);
    }
}

void CommandQueueManager::AddQueue(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandQueueInterface>& command_queue)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        throw std::runtime_error("Queue already exists for user with id: " + queue_owner_id);
    }

    queue_map_.insert(std::make_pair(queue_owner_id, command_queue));
}

void CommandQueueManager::RemoveQueue(uint64_t queue_owner_id)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    queue_map_.erase(queue_owner_id);
}
        
void CommandQueueManager::SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        find_iter->second->SetDefaultCommand(command);
    }
}

void CommandQueueManager::ClearDefaultCommand(uint64_t queue_owner_id)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        find_iter->second->ClearDefaultCommand();
    }
}

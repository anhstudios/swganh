// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue_manager.h"

#include <boost/thread/lock_guard.hpp>

#include "swganh_core/command/command_interface.h"
#include "swganh_core/command/command_queue_interface.h"

#include "swganh/observer/observer_interface.h"

using swganh::command::CommandQueueManager;
using swganh::command::CommandInterface;
using swganh::command::CommandQueueInterface;

void CommandQueueManager::EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command)
{
    CommandQueueInterface* queue = nullptr;

	{
		boost::unique_lock<boost::mutex> lg(queue_map_mutex_);
    
		auto find_iter = queue_map_.find(command->GetController()->GetId());
		if (find_iter == queue_map_.end())
		{
			return;
		}

		queue = find_iter->second.get();
	}

    queue->EnqueueCommand(command);
}

void CommandQueueManager::AddQueue(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandQueueInterface>& command_queue)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        throw std::runtime_error("Queue already exists for user");
    }

    queue_map_.insert(std::make_pair(queue_owner_id, command_queue));
}

void CommandQueueManager::RemoveQueue(uint64_t queue_owner_id)
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    queue_map_.erase(queue_owner_id);
}
        
void CommandQueueManager::ClearQueues()
{
    boost::lock_guard<boost::mutex> lg(queue_map_mutex_);
    queue_map_.clear();
}
        
void CommandQueueManager::SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command)
{
    CommandQueueInterface* queue = nullptr;
    boost::unique_lock<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        queue = find_iter->second.get();
        lg.unlock();

        queue->SetDefaultCommand(command);
    }
}

void CommandQueueManager::ClearDefaultCommand(uint64_t queue_owner_id)
{
    CommandQueueInterface* queue = nullptr;
    boost::unique_lock<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        queue = find_iter->second.get();
        lg.unlock();

        queue->ClearDefaultCommand();
    }
}

bool CommandQueueManager::HasDefaultCommand(uint64_t queue_owner_id)
{
    CommandQueueInterface* queue = nullptr;
    boost::unique_lock<boost::mutex> lg(queue_map_mutex_);
    auto find_iter = queue_map_.find(queue_owner_id);
    if (find_iter != queue_map_.end())
    {
        queue = find_iter->second.get();
        lg.unlock();

        queue->HasDefaultCommand();
    }

    return false;
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include <boost/thread/mutex.hpp>

#include "swganh_core/command/command_queue_manager_interface.h"

namespace swganh {
namespace command {
    
	/**
	* Manages command queues for all objects that require them.
	*/
    class CommandQueueManager : public swganh::command::CommandQueueManagerInterface
    {
    public:

		/**
		* Enqueues a command for the proper object
		* @param command the command to enqueue
		*/
        virtual void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);

		/**
		* Sets an an object's queue.
		* @param queue_owner_id the id of the queue's owner
		* @param command_queue the command queue to add
		*/
        virtual void AddQueue(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandQueueInterface>& command_queue);

		/**
		* Removes an object's queue.
		* @param queue_owner_id the id of the owner to disassociate
		*/
        virtual void RemoveQueue(uint64_t queue_owner_id);
        
		/**
		* Clears all queues for all owners
		*/
        virtual void ClearQueues();

		/**
		* Sets the default command for a particular owner
		* @param the queue owner to set the default command for.
		*/
        virtual void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command);

		/**
		* Clears the default command for a particular owner
		* @param queue_owner_id the object to clear the default command for
		*/
        virtual void ClearDefaultCommand(uint64_t queue_owner_id);

		/*
		* @param queue_owner_id the object to check
		* @return true if the given object has a default command
		*/
        virtual bool HasDefaultCommand(uint64_t queue_owner_id);

    private:        
        typedef std::map<uint64_t,std::shared_ptr<swganh::command::CommandQueueInterface>> CommandQueueMap;

        boost::mutex queue_map_mutex_;
        CommandQueueMap queue_map_;
    };

}}

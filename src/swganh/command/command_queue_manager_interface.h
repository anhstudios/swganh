// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

namespace swganh {
namespace command {
    
    class CommandInterface;
    class CommandQueueInterface;

    /**
     * Manages a collection of command queues.
     */
    class CommandQueueManagerInterface
    {
    public:
        virtual ~CommandQueueManagerInterface() {}

        /**
         * Enqueues a command for processing.
         */
        virtual void EnqueueCommand(const std::shared_ptr<CommandInterface>& command) = 0;

        /**
         * Adds a new queue to be managed.
         *
         * @param queue_owner_id The id of the object that owns the queue to be added.
         * @param command_queue The new queue to add.
         */
        virtual void AddQueue(uint64_t queue_owner_id, const std::shared_ptr<CommandQueueInterface>& command_queue) = 0;

        /**
         * Removes a queue from management.
         *
         * @param queue_owner_id The id of the object that owns the queue to be removed.
         */
        virtual void RemoveQueue(uint64_t queue_owner_id) = 0;

        virtual void ClearQueues() = 0;
        
        virtual void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command) = 0;

        virtual void ClearDefaultCommand(uint64_t queue_owner_id) = 0;
        
        virtual bool HasDefaultCommand(uint64_t queue_owner_id) = 0;
    };

}}

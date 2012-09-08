// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include <boost/thread/mutex.hpp>

#include "swganh/command/command_queue_manager_interface.h"

namespace swganh {
namespace command {
    
    class CommandQueueManager : public swganh::command::CommandQueueManagerInterface
    {
    public:
        virtual ~CommandQueueManager();

        virtual void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);

        virtual void AddQueue(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandQueueInterface>& command_queue);

        virtual void RemoveQueue(uint64_t queue_owner_id);
        
        virtual void ClearQueues();

        virtual void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command);

        virtual void ClearDefaultCommand(uint64_t queue_owner_id);

        virtual bool HasDefaultCommand(uint64_t queue_owner_id);

    private:        
        typedef std::map<
            uint64_t,
            std::shared_ptr<swganh::command::CommandQueueInterface>
        > CommandQueueMap;

        boost::mutex queue_map_mutex_;
        CommandQueueMap queue_map_;
    };

}}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_QUEUE_MANAGER_INTERFACE_H_
#define SWGANH_COMMAND_COMMAND_QUEUE_MANAGER_INTERFACE_H_

#include <memory>

namespace swganh {
namespace command {
    
    class CommandInterface;
    class CommandQueueInterface;

    class CommandQueueManagerInterface
    {
    public:
        virtual ~CommandQueueManagerInterface() {}

        virtual void EnqueueCommand(std::unique_ptr<CommandInterface> command) = 0;

        virtual void AddQueue(uint64_t queue_owner_id, const std::shared_ptr<CommandQueueInterface>& command_queue) = 0;

        virtual void RemoveQueue(uint64_t queue_owner_id) = 0;
    };

}}

#endif  // SWGANH_COMMAND_COMMAND_QUEUE_MANAGER_INTERFACE_H_

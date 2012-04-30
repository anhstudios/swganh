// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_V2_COMMAND_QUEUE_MANAGER_INTERFACE_H_
#define SWGANH_COMMAND_V2_COMMAND_QUEUE_MANAGER_INTERFACE_H_

#include <cstdint>
#include <memory>
#include <tuple>

namespace swganh {
namespace command {
namespace v2 {

    class CommandInterface;
    class CommandQueueInterface;

    /**
     */
    class CommandQueueManagerInterface
    {
    public:
        virtual ~CommandQueueManagerInterface() {}
        
        virtual CommandQueueInterface* CreateCommandQueue(uint64_t object_id) = 0;

        virtual void DestroyCommandQueue(uint64_t object_id) = 0;
        
        virtual CommandQueueInterface* FindCommandQueue(uint64_t object_id) = 0;

        virtual void EnqueueCommand(std::unique_ptr<swganh::command::v2::CommandInterface> command) = 0;
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_MANAGER_INTERFACE_H_

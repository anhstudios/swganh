// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_
#define PUB14_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/command/v2/command_queue_manager_interface.h"

#include "command_queue.h"

namespace pub14_core {
namespace command {
    
    class CommandQueueManager : public swganh::command::v2::CommandQueueManagerInterface
    {
    public:
        CommandQueueManager();
        ~CommandQueueManager();

        CommandQueue* CreateCommandQueue(uint64_t object_id);

        void DestroyCommandQueue(uint64_t object_id);
        
        CommandQueue* FindCommandQueue(uint64_t object_id);

        void EnqueueCommand(std::unique_ptr<swganh::command::v2::CommandInterface> command);
                
    private:
        typedef std::map<
            uint64_t, 
            std::unique_ptr<CommandQueue>
        > CommandQueueMap;

        CommandQueueMap command_queue_map_;
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_

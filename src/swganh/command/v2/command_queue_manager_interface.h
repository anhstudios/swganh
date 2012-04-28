// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_V2_COMMAND_QUEUE_MANAGER_INTERFACE_H_
#define SWGANH_COMMAND_V2_COMMAND_QUEUE_MANAGER_INTERFACE_H_

#include <cstdint>

#include "swganh/app/swganh_kernel.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/messages/controllers/command_queue_remove.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

#include "command_interface.h"
#include "command_properties.h"

namespace swganh {
namespace command {
namespace v2 {

    class CommandQueueInterface;

    typedef std::function<std::unique_ptr<swganh::command::v2::CommandInterface>(
        swganh::command::v2::CommandProperties* properties)
    > CommandCreator;

    /**
     */
    class CommandQueueManagerInterface
    {
    public:
        virtual ~CommandQueueManagerInterface() {}
        
        virtual void EnqueueCommand(
            const std::shared_ptr<object::ObjectController>& controller,
            messages::controllers::CommandQueueEnqueue message) = 0;

        CommandQueueInterface* FindCommandQueue(uint64_t object_id);

        std::unique_ptr<CommandInterface> CreateCommand(
            anh::HashString command);
        
        void AddCommandCreator(anh::HashString command, CommandCreator&& creator);
        
        void RemoveCommandCreator(anh::HashString command);
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_MANAGER_INTERFACE_H_

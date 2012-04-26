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
    
    using messages::controllers::CommandQueueRemove;
    using messages::controllers::CommandQueueEnqueue;
    
    typedef std::function<void (
        swganh::app::SwganhKernel*,
		const std::shared_ptr<object::creature::Creature>&, // creature object
		const std::shared_ptr<object::tangible::Tangible>&,	// target object
        const CommandQueueEnqueue&,
        const CommandProperties&)
    > CommandHandler;

    typedef std::function<std::tuple<bool, uint32_t, uint32_t> (
        const std::shared_ptr<object::creature::Creature>&, // creature object
		const std::shared_ptr<object::tangible::Tangible>&, // target object
        const CommandQueueEnqueue&,
        const CommandProperties&)	// action
    > CommandFilter;
    
    /**
     */
    class CommandQueueManagerInterface
    {
    public:
        virtual ~CommandQueueManagerInterface() {}
        
        virtual void AddEnqueueFilter(CommandFilter filter) = 0;
        
        virtual void AddProcessFilter(CommandFilter filter) = 0;
        
        virtual void AddAutoCommand(uint64_t object_id, std::unique_ptr<CommandInterface> command) = 0;
        
        virtual void RemoveAutoCommand(uint64_t object_id) = 0;
        
        virtual void EnqueueCommand(
            const std::shared_ptr<object::creature::Creature>&,
            const std::shared_ptr<object::tangible::Tangible>&,
            CommandQueueEnqueue message) = 0;
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_MANAGER_INTERFACE_H_

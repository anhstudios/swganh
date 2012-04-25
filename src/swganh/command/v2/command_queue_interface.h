// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_V2_COMMAND_PROCESSOR_INTERFACE_H_
#define SWGANH_COMMAND_V2_COMMAND_PROCESSOR_INTERFACE_H_

#include "command_interface.h"

namespace swganh {
namespace command {
namespace v2 {
    
    /**
     */
    class CommandQueueInterface
    {
    public:
        virtual ~CommandQueueInterface() {}
        
        virtual void SetQueueOwner(const std::shared_ptr<swganh::object::ObjectController>& owner) = 0;
        virtual const std::shared_ptr<swganh::object::ObjectController>& GetQueueOwner() const = 0;
        
        virtual void EnqueueCommand(std::unique_ptr<CommandInterface> command) = 0;
        virtual void ProcessCommand(std::unique_ptr<CommandInterface> command) = 0;
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_PROCESSOR_INTERFACE_H_

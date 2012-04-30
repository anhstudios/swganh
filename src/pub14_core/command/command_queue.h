// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_QUEUE_H_
#define PUB14_CORE_COMMAND_COMMAND_QUEUE_H_

#include <memory>

#include "swganh/command/v2/command_interface.h"
#include "swganh/command/v2/command_queue_interface.h"
#include "swganh/object/object_controller.h"

namespace pub14_core {
namespace command {

    class CommandQueue : public swganh::command::v2::CommandQueueInterface
    {
    public:
        CommandQueue();
        ~CommandQueue();
        
        void SetQueueOwner(const std::shared_ptr<swganh::object::ObjectController>& queue_owner);
        const std::shared_ptr<swganh::object::ObjectController>& GetQueueOwner() const;
        
        void EnqueueCommand(std::unique_ptr<swganh::command::v2::CommandInterface> command);

        void SetAutoCommand(std::unique_ptr<swganh::command::v2::CommandInterface> command);
        void RemoveAutoCommand();

    private:
        std::shared_ptr<swganh::object::ObjectController> queue_owner_;
    };

}}  // namespace swganh_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_QUEUE_H_

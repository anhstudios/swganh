// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_BASE_SWG_COMMAND_H_
#define SWGANH_COMMAND_BASE_SWG_COMMAND_H_

#include <memory>

#include "swganh/messages/controllers/command_queue_enqueue.h"

#include "command_interface.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}

namespace object {
namespace creature {
    class Creature;
}  // namespace creature
namespace tangible {
    class Tangible;
}}  // namespace object::tangible

namespace messages {
namespace controllers {
    class CommandQueueEnqueue;
}}  // namespace messages::controllers

namespace command {

    class BaseSwgCommand : public CommandInterface
    {
    public:
        BaseSwgCommand(
            swganh::app::SwganhKernel* kernel,
            const std::shared_ptr<object::creature::Creature>& actor,
            const std::shared_ptr<object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command_request);

        ~BaseSwgCommand();
        
        swganh::app::SwganhKernel* GetKernel() const;

        const std::shared_ptr<object::creature::Creature>& GetActor() const;

        const std::shared_ptr<object::tangible::Tangible>& GetTarget() const;

        const std::wstring& GetCommandString() const;

        const swganh::messages::controllers::CommandQueueEnqueue& GetCommandRequest() const;

    private:    
        swganh::app::SwganhKernel* kernel_;
        std::shared_ptr<object::creature::Creature> actor_;
        std::shared_ptr<object::tangible::Tangible> target_;
        swganh::messages::controllers::CommandQueueEnqueue command_request_;
    };

}}

#endif  // SWGANH_COMMAND_BASE_SWG_COMMAND_H_

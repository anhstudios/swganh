// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_BASE_SWG_COMMAND_H_
#define SWGANH_COMMAND_BASE_SWG_COMMAND_H_

#include <memory>

#include "swganh/messages/controllers/command_queue_enqueue.h"

#include "command_interface.h"
#include "command_properties.h"

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
            const CommandProperties& properties,
            const std::shared_ptr<object::creature::Creature>& actor,
            const std::shared_ptr<object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command_request);

        ~BaseSwgCommand();
        
        swganh::app::SwganhKernel* GetKernel() const;

        std::string GetCommandName() const;

        uint32_t GetActionCounter() const;

        uint32_t GetPriority() const;

        uint32_t GetCommandGroup() const;
        
        uint32_t GetTargetRequiredType() const;

        uint64_t GetAllowedStateBitmask() const;
        
        float GetMaxRangeToTarget() const;
        
        float GetDefaultTime() const;

        std::string GetRequiredAbility() const;

        bool IsQueuedCommand() const;

        const std::shared_ptr<object::creature::Creature>& GetActor() const;

        const std::shared_ptr<object::tangible::Tangible>& GetTarget() const;

        const std::wstring& GetCommandString() const;

    private:    
        swganh::app::SwganhKernel* kernel_;
        const CommandProperties& properties_;
        std::shared_ptr<object::creature::Creature> actor_;
        std::shared_ptr<object::tangible::Tangible> target_;
        swganh::messages::controllers::CommandQueueEnqueue command_request_;
    };

}}

#endif  // SWGANH_COMMAND_BASE_SWG_COMMAND_H_

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include "pub14_core/messages/controllers/command_queue_enqueue.h"

#include "command_interface.h"
#include "command_properties.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}

namespace object {
    class ObjectController;
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
            const CommandProperties& properties);

        ~BaseSwgCommand();

        virtual const std::shared_ptr<object::ObjectController>& GetController() const;

        virtual void SetController(const std::shared_ptr<object::ObjectController>& controller);

        virtual bool Validate();
        
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

		const std::shared_ptr<object::creature::Creature>& GetTargetCreature() const;

        const std::wstring& GetCommandString() const;

        void SetCommandProperties(const CommandProperties& properties);
        
        const swganh::messages::controllers::CommandQueueEnqueue& GetCommandRequest() const;

        void SetCommandRequest(swganh::messages::controllers::CommandQueueEnqueue command_request);
    private:    
        swganh::app::SwganhKernel* kernel_;
        const CommandProperties* properties_;
        std::shared_ptr<object::ObjectController> controller_;
        mutable std::shared_ptr<object::creature::Creature> actor_;
        mutable std::shared_ptr<object::tangible::Tangible> target_;
		mutable std::shared_ptr<object::creature::Creature> creature_target_;
        swganh::messages::controllers::CommandQueueEnqueue command_request_;
    };

}}

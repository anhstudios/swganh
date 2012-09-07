// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#include "swganh_core/messages/controllers/command_queue_enqueue.h"

#include "command_interface.h"
#include "command_properties.h"

namespace swganh {
namespace observer {
	class ObserverInterface;
}
}

namespace swganh {
namespace app {
    class SwganhKernel;
}

namespace object {
	class Object;
}

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

        const std::shared_ptr<swganh::observer::ObserverInterface> GetController() const;
		void SetController(std::shared_ptr<swganh::observer::ObserverInterface> controller);

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

        const std::shared_ptr<object::Object>& GetActor() const;
		void SetActor(std::shared_ptr<object::Object> object);

        const std::shared_ptr<object::Object>& GetTarget() const;
		void SetTarget(std::shared_ptr<object::Object> target);

        const std::wstring& GetCommandString() const;

        void SetCommandProperties(const CommandProperties& properties);
        
        const swganh::messages::controllers::CommandQueueEnqueue& GetCommandRequest() const;

        void SetCommandRequest(swganh::messages::controllers::CommandQueueEnqueue command_request);
    private:    
        swganh::app::SwganhKernel* kernel_;
        const CommandProperties* properties_;
        std::shared_ptr<swganh::observer::ObserverInterface> controller_;
		mutable std::shared_ptr<object::Object> actor_;
        mutable std::shared_ptr<object::Object> target_;
        swganh::messages::controllers::CommandQueueEnqueue command_request_;
    };

}}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include "swganh/event_dispatcher.h"

#include "swganh/command/command_service_interface.h"

namespace swganh {
namespace observer {
	class ObserverInterface;
}
}

namespace swganh {
namespace command {

    class CommandService: public swganh::command::CommandServiceInterface
    {
    public:
        explicit CommandService(swganh::app::SwganhKernel* kernel);
        ~CommandService();

        swganh::service::ServiceDescription GetServiceDescription();

        void AddCommandEnqueueFilter(swganh::command::CommandFilter&& filter);
        
        void AddCommandProcessFilter(swganh::command::CommandFilter&& filter);
        
        void AddCommandCreator(swganh::HashString command, swganh::command::CommandCreator&& creator);

        void RemoveCommandCreator(swganh::HashString command);
        
        std::shared_ptr<swganh::command::CommandInterface> CreateCommand(swganh::HashString command);

        void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);

        void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::Object>& object,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action);
        
        std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(swganh::command::CommandInterface* command);
        
        std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(swganh::command::CommandInterface* command);
        
        swganh::command::CommandPropertiesMap LoadCommandPropertiesMap();

        boost::optional<const swganh::command::CommandProperties&> FindPropertiesForCommand(swganh::HashString command);

        void Startup();
        
        void EnqueueCommandRequest(
            const std::shared_ptr<swganh::object::Object>& controller,
            swganh::messages::controllers::CommandQueueEnqueue* message);
        
        
        void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command);

        void ClearDefaultCommand(uint64_t queue_owner_id);

        bool HasDefaultCommand(uint64_t queue_owner_id);

    private:
        void SubscribeObjectReadyEvent(swganh::EventDispatcher* dispatcher);

        void SubscribeObjectRemovedEvent(swganh::EventDispatcher* dispatcher);
                        
        swganh::app::SwganhKernel* kernel_;
        std::shared_ptr<swganh::command::CommandFactoryInterface> command_factory_impl_;
        std::shared_ptr<swganh::command::CommandPropertiesManagerInterface> command_properties_manager_impl_;
        std::shared_ptr<swganh::command::CommandQueueManagerInterface> command_queue_manager_impl_;
        std::shared_ptr<swganh::command::CommandValidatorInterface> command_validator_impl_;
        swganh::simulation::SimulationServiceInterface* simulation_service_;
        std::string script_prefix_;
        swganh::CallbackId obj_ready_id_;
        swganh::CallbackId obj_removed_id_;
    };

}}  // namespace swganh::command

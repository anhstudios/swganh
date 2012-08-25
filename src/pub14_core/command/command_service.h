// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include "anh/event_dispatcher.h"

#include "swganh/command/command_service_interface.h"

namespace pub14_core {
namespace command {

    class CommandService: public swganh::command::CommandServiceInterface
    {
    public:
        explicit CommandService(swganh::app::SwganhKernel* kernel);
        ~CommandService();

        anh::service::ServiceDescription GetServiceDescription();

        void AddCommandEnqueueFilter(swganh::command::CommandFilter&& filter);
        
        void AddCommandProcessFilter(swganh::command::CommandFilter&& filter);
        
        void AddCommandCreator(anh::HashString command, swganh::command::CommandCreator&& creator);

        void RemoveCommandCreator(anh::HashString command);
        
        std::shared_ptr<swganh::command::CommandInterface> CreateCommand(anh::HashString command);

        void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);

        void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action);
        
        std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(swganh::command::CommandInterface* command);
        
        std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(swganh::command::CommandInterface* command);
        
        swganh::command::CommandPropertiesMap LoadCommandPropertiesMap();

        boost::optional<const swganh::command::CommandProperties&> FindPropertiesForCommand(anh::HashString command);

        void Startup();
        
        void EnqueueCommandRequest(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            swganh::messages::controllers::CommandQueueEnqueue message);
        
        
        void SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command);

        void ClearDefaultCommand(uint64_t queue_owner_id);

        bool HasDefaultCommand(uint64_t queue_owner_id);

    private:
        void SubscribeObjectReadyEvent(anh::EventDispatcher* dispatcher);

        void SubscribeObjectRemovedEvent(anh::EventDispatcher* dispatcher);
                        
        swganh::app::SwganhKernel* kernel_;
        std::shared_ptr<swganh::command::CommandFactoryInterface> command_factory_impl_;
        std::shared_ptr<swganh::command::CommandPropertiesManagerInterface> command_properties_manager_impl_;
        std::shared_ptr<swganh::command::CommandQueueManagerInterface> command_queue_manager_impl_;
        std::shared_ptr<swganh::command::CommandValidatorInterface> command_validator_impl_;
        swganh::simulation::SimulationServiceInterface* simulation_service_;
        std::string script_prefix_;
        anh::CallbackId obj_ready_id_;
        anh::CallbackId obj_removed_id_;
    };

}}  // namespace pub14_core::command

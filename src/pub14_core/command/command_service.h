// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_SERVICE_H_
#define PUB14_CORE_COMMAND_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>

#include "swganh/command/v2/command_service_interface.h"

#include "anh/observer/observer_interface.h"
#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/object_controller.h"
#include "swganh/simulation/simulation_service.h"

#include "swganh/command/v2/command_factory_interface.h"
#include "swganh/command/v2/command_interface.h"
#include "swganh/command/v2/command_queue_manager_interface.h"
#include "swganh/command/v2/command_properties_loader_interface.h"

namespace pub14_core {
namespace command {
    
    /**
     */
    class CommandService : public swganh::command::v2::CommandServiceInterface
    {
    public:
        CommandService(swganh::app::SwganhKernel* kernel);

        anh::service::ServiceDescription GetServiceDescription();

        void Start();
        
        void AddAutoCommand(uint64_t object_id, std::unique_ptr<swganh::command::v2::CommandInterface> command);
        
        void RemoveAutoCommand(uint64_t object_id);
        
        void SendCommandQueueRemove(
            std::shared_ptr<anh::observer::ObserverInterface> observer,
            uint32_t action_counter,
            float timer,
            uint32_t error,
            uint32_t action);

    private:        
        std::shared_ptr<swganh::command::v2::CommandQueueManagerInterface> command_queue_manager_impl_;     
        std::shared_ptr<swganh::command::v2::CommandPropertiesLoaderInterface> command_properties_loader_impl_;
        std::shared_ptr<swganh::command::v2::CommandFactoryInterface> command_factory_impl_;
        
        void HandleCommandQueueEnqueue(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            swganh::messages::controllers::CommandQueueEnqueue message);

        swganh::app::SwganhKernel* kernel_;
        swganh::simulation::SimulationService* simulation_service_;
    };
    
}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_SERVICE_H_

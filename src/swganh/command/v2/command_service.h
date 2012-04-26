// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_V2_COMMAND_SERVICE_H_
#define SWGANH_COMMAND_V2_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>

#include "anh/observer/observer_interface.h"
#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/object_controller.h"
#include "swganh/simulation/simulation_service.h"

#include "command_interface.h"
#include "command_queue_manager_interface.h"
#include "command_properties_loader_interface.h"

namespace swganh {
namespace command {
namespace v2 {
    
    using swganh::messages::controllers::CommandQueueEnqueue;
    using swganh::messages::controllers::CommandQueueRemove;

    /**
     */
    class CommandService : public anh::service::ServiceInterface
    {
    public:
        CommandService(swganh::app::SwganhKernel* kernel);

        anh::service::ServiceDescription GetServiceDescription();

        void Start();
        
        void AddEnqueueFilter(CommandFilter&& filter);
        
        void AddProcessFilter(CommandFilter&& filter);
        
        void AddAutoCommand(uint64_t object_id, std::unique_ptr<CommandInterface> command);
        
        void RemoveAutoCommand(uint64_t object_id);
        
        void SendCommandQueueRemove(
            std::unique_ptr<anh::observer::ObserverInterface> observer,
            uint32_t action_counter,
            float timer,
            uint32_t error,
            uint32_t action);

        CommandPropertiesMap LoadCommandPropertiesMap();

    private:
        
        void HandleCommandQueueEnqueue(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            CommandQueueEnqueue message);

        std::shared_ptr<CommandQueueManagerInterface> command_queue_manager_impl_;
        std::shared_ptr<CommandPropertiesLoaderInterface> command_properties_loader_impl_;
        swganh::app::SwganhKernel* kernel_;
        swganh::simulation::SimulationService* simulation_service_;
    };
    
}}}  // namespace swganh::command::v2

#endif  // SWGANH_COMMAND_V2_COMMAND_SERVICE_H_

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_service.h"

#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/messages/controllers/command_queue_remove.h"
#include "swganh/simulation/simulation_service.h"

using anh::observer::ObserverInterface;
using anh::service::ServiceDescription;
using pub14_core::command::CommandService;
using swganh::command::v2::CommandFactoryInterface;
using swganh::command::v2::CommandInterface;
using swganh::command::v2::CommandQueueManagerInterface;
using swganh::command::v2::CommandPropertiesLoaderInterface;
using swganh::messages::controllers::CommandQueueRemove;
using swganh::simulation::SimulationService;
using swganh::object::ObjectController;

CommandService::CommandService(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
{
    // Acquire handles to resources, these resources are not guaranteed
    // to be ready until Start() is invoked.
    command_queue_manager_impl_ = kernel_->GetPluginManager()->CreateObject<CommandQueueManagerInterface>("CommandService::CommandQueueManager");
    command_properties_loader_impl_ = kernel_->GetPluginManager()->CreateObject<CommandPropertiesLoaderInterface>("CommandService::CommandPropertiesLoader");
    command_factory_impl_ = kernel_->GetPluginManager()->CreateObject<CommandFactoryInterface>("CommandService::CommandFactory");
    
    simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
}

ServiceDescription CommandService::GetServiceDescription()
{
    ServiceDescription service_description(
        "CommandService",
        "command",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

void CommandService::Start()
{
    simulation_service_->RegisterControllerHandler(
        &CommandQueueManagerInterface::EnqueueCommand, command_queue_manager_impl_.get());
}

void CommandService::AddAutoCommand(uint64_t object_id, std::unique_ptr<CommandInterface> command)
{}
        
void CommandService::RemoveAutoCommand(uint64_t object_id)
{}
        
void CommandService::SendCommandQueueRemove(
    std::unique_ptr<ObserverInterface> observer,
    uint32_t action_counter,
    float timer,
    uint32_t error,
    uint32_t action)
{
    CommandQueueRemove remove_message;
    remove_message.action_counter = action_counter;
    remove_message.timer = timer;
    remove_message.error = error;
    remove_message.action = action;
    
    observer->Notify(remove_message);
}

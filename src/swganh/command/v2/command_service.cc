// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_service.h"

#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/messages/controllers/command_queue_remove.h"
#include "swganh/simulation/simulation_service.h"

using namespace swganh::command::v2;
using anh::observer::ObserverInterface;
using anh::service::ServiceDescription;
using swganh::messages::controllers::CommandQueueRemove;
using swganh::simulation::SimulationService;
using swganh::object::ObjectController;
using swganh::object::creature::Creature;
using swganh::object::tangible::Tangible;

CommandService::CommandService(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
{
    // Acquire handles to resources, these resources are not guaranteed
    // to be ready until Start() is invoked.
    command_queue_manager_impl_ = kernel_->GetPluginManager()->CreateObject<CommandQueueManagerInterface>("CommandService::CommandQueueManager");
    command_properties_loader_impl_ = kernel_->GetPluginManager()->CreateObject<CommandPropertiesLoaderInterface>("CommandService::CommandPropertiesLoader");
    
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
    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueEnqueue, this);
}

void CommandService::AddEnqueueFilter(CommandFilter&& filter)
{
    command_queue_manager_impl_->AddEnqueueFilter(std::move(filter));
}
        
void CommandService::AddProcessFilter(CommandFilter&& filter)
{
    command_queue_manager_impl_->AddProcessFilter(std::move(filter));
}
        
void CommandService::AddAutoCommand(uint64_t object_id, std::unique_ptr<CommandInterface> command)
{
    command_queue_manager_impl_->AddAutoCommand(object_id, std::move(command));
}
        
void CommandService::RemoveAutoCommand(uint64_t object_id)
{
    command_queue_manager_impl_->RemoveAutoCommand(object_id);
}
        
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

CommandPropertiesMap CommandService::LoadCommandPropertiesMap()
{
    return command_properties_loader_impl_->LoadCommandPropertiesMap();
}

void CommandService::HandleCommandQueueEnqueue(
    const std::shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue message)
{
    auto actor = simulation_service_->GetObjectById<Creature>(message.observable_id);
	auto target = simulation_service_->GetObjectById<Tangible>(message.target_id);

    command_queue_manager_impl_->EnqueueCommand(actor, target, std::move(message));
}

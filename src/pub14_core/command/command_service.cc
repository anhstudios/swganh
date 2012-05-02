// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_service.h"

#include "anh/event_dispatcher.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/command/v2/command_interface.h"
#include "swganh/command/v2/command_queue_interface.h"
#include "pub14_core/messages/controllers/command_queue_remove.h"
#include "swganh/object/object.h"
#include "swganh/simulation/simulation_service.h"

#include "version.h"

using anh::observer::ObserverInterface;
using anh::service::ServiceDescription;
using pub14_core::command::CommandService;
using swganh::command::v2::CommandFactoryInterface;
using swganh::command::v2::CommandInterface;
using swganh::command::v2::CommandQueueManagerInterface;
using swganh::command::v2::CommandPropertiesLoaderInterface;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::messages::controllers::CommandQueueRemove;
using swganh::simulation::SimulationService;
using swganh::object::ObjectController;
using swganh::object::Object;

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
        VERSION_MAJOR + "." + VERSION_MINOR,
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

void CommandService::Start()
{
    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueEnqueue, this);
    
	auto event_dispatcher = kernel_->GetEventDispatcher();
    
    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (const std::shared_ptr<anh::EventInterface>& incoming_event)
    {
        const auto& object = std::static_pointer_cast<anh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();
        command_queue_manager_impl_->CreateCommandQueue(object->GetObjectId());
    });

    event_dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (const std::shared_ptr<anh::EventInterface>& incoming_event)
    {
        const auto& object = std::static_pointer_cast<anh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();
        command_queue_manager_impl_->DestroyCommandQueue(object->GetObjectId());
    });
}

void CommandService::AddAutoCommand(uint64_t object_id, std::unique_ptr<CommandInterface> command)
{
    auto command_queue = command_queue_manager_impl_->FindCommandQueue(object_id);
    if (command_queue)
    {
        command_queue->SetAutoCommand(std::move(command));
    }
}
        
void CommandService::RemoveAutoCommand(uint64_t object_id)
{
    auto command_queue = command_queue_manager_impl_->FindCommandQueue(object_id);
    if (command_queue)
    {
        command_queue->RemoveAutoCommand();
    }    
}
        
void CommandService::SendCommandQueueRemove(
    std::shared_ptr<ObserverInterface> observer,
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

void CommandService::HandleCommandQueueEnqueue(
    const std::shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue message)
{
    // Create a new command from the message data
    auto command = command_factory_impl_->CreateCommand(
        controller,
        message.command_crc,
        message.target_id,
        message.action_counter,
        message.command_options);
    
    if (!command)
    {
        LOG(warning) << "Invalid command requested: " << std::hex << message.command_crc;
        SendCommandQueueRemove(controller, message.action_counter, 0.0f, 0, 1);
        return;
    }

    command_queue_manager_impl_->EnqueueCommand(std::move(command));
}

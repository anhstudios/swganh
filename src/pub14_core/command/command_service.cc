// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_service.h"

#include "anh/logger.h"

#include "anh/event_dispatcher.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/messages/controllers/command_queue_remove.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

#include "swganh/simulation/simulation_service.h"

#include "swganh/command/base_swg_command.h"
#include "swganh/command/command_properties_manager_interface.h"
#include "swganh/command/command_queue_interface.h"
#include "swganh/command/command_queue_manager_interface.h"

using anh::service::ServiceDescription;
using pub14_core::command::CommandService;
using swganh::app::SwganhKernel;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCreator;
using swganh::command::CommandFactoryInterface;
using swganh::command::CommandFilter;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::command::CommandPropertiesManagerInterface;
using swganh::command::CommandQueueInterface;
using swganh::command::CommandQueueManagerInterface;
using swganh::command::CommandValidatorInterface;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::messages::controllers::CommandQueueRemove;
using swganh::object::Object;
using swganh::object::ObjectController;
using swganh::simulation::SimulationService;

CommandService::CommandService(SwganhKernel* kernel)
: kernel_(kernel)
{
    command_factory_impl_ = kernel->GetPluginManager()->CreateObject<CommandFactoryInterface>("Command::CommandFactory");
    command_properties_manager_impl_ = kernel->GetPluginManager()->CreateObject<CommandPropertiesManagerInterface>("Command::CommandPropertiesManager");
    command_queue_manager_impl_ = kernel->GetPluginManager()->CreateObject<CommandQueueManagerInterface>("Command::CommandQueueManager");
    command_validator_impl_ = kernel->GetPluginManager()->CreateObject<CommandValidatorInterface>("Command::CommandValidator");
}

CommandService::~CommandService()
{    
    auto event_dispatcher = kernel_->GetEventDispatcher();

    event_dispatcher->Unsubscribe("ObjectReadyEvent", obj_ready_id_);
    event_dispatcher->Unsubscribe("ObjectRemovedEvent", obj_removed_id_);
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

void CommandService::AddCommandEnqueueFilter(CommandFilter&& filter)
{
    command_validator_impl_->AddCommandEnqueueFilter(std::move(filter));
}

void CommandService::AddCommandProcessFilter(CommandFilter&& filter)
{
    command_validator_impl_->AddCommandProcessFilter(std::move(filter));
}

void CommandService::AddCommandCreator(anh::HashString command, CommandCreator&& creator)
{
    command_factory_impl_->AddCommandCreator(command, std::move(creator));
}

void CommandService::RemoveCommandCreator(anh::HashString command)
{
    command_factory_impl_->RemoveCommandCreator(command);
}

void CommandService::HandleCommandQueueEnqueue(
    const std::shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue command_request)
{
    auto command = command_factory_impl_->CreateCommand(controller, command_request);
    if (command)
    {
        command_queue_manager_impl_->EnqueueCommand(command);
    }
}
        
std::tuple<bool, uint32_t, uint32_t> CommandService::ValidateForEnqueue(CommandInterface* command)
{
    return command_validator_impl_->ValidateForEnqueue(command);
}

std::tuple<bool, uint32_t, uint32_t> CommandService::ValidateForProcessing(CommandInterface* command)
{
    return command_validator_impl_->ValidateForProcessing(command);
}

swganh::command::CommandPropertiesMap CommandService::LoadCommandPropertiesMap()
{
    return command_properties_manager_impl_->LoadCommandPropertiesMap();
}
 
boost::optional<const CommandProperties&> CommandService::FindPropertiesForCommand(anh::HashString command)
{
    return command_properties_manager_impl_->FindPropertiesForCommand(command);
}

void CommandService::Start()
{
    script_prefix_ = kernel_->GetAppConfig().script_directory;
    
    simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueEnqueue, this);

    auto event_dispatcher = kernel_->GetEventDispatcher();

    SubscribeObjectReadyEvent(event_dispatcher);
    SubscribeObjectRemovedEvent(event_dispatcher);
}

void CommandService::SendCommandQueueRemove(
    const std::shared_ptr<ObjectController>& controller,
    uint32_t action_counter,
    float default_time_sec,
    uint32_t error,
    uint32_t action)
{
    CommandQueueRemove remove;
    remove.action_counter = action_counter;
    remove.timer = default_time_sec;
    remove.error = error;
    remove.action = action;

    controller->Notify(remove);
}

void CommandService::SubscribeObjectReadyEvent(anh::EventDispatcher* dispatcher)
{
    obj_ready_id_ = dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (std::shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = std::static_pointer_cast<anh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();

        command_queue_manager_impl_->AddQueue(
            object->GetObjectId(), 
            kernel_->GetPluginManager()->CreateObject<CommandQueueInterface>("Command::CommandQueue"));
    });
}

void CommandService::SubscribeObjectRemovedEvent(anh::EventDispatcher* dispatcher)
{
    obj_removed_id_ = dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (std::shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = std::static_pointer_cast<anh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();
        command_queue_manager_impl_->RemoveQueue(object->GetObjectId());
    });
}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_service.h"

#include "swganh/logger.h"

#include "swganh/event_dispatcher.h"
#include "swganh/plugin/plugin_manager.h"
#include "swganh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh_core/messages/controllers/command_queue_remove.h"

#include "swganh_core/simulation/simulation_service_interface.h"

#include "swganh_core/command/base_swg_command.h"
#include "swganh_core/command/command_properties_manager_interface.h"
#include "swganh_core/command/command_queue_interface.h"
#include "swganh_core/command/command_queue_manager_interface.h"

#include "swganh_core/object/object.h"

using swganh::service::ServiceDescription;
using swganh::command::CommandService;
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
using swganh::simulation::SimulationServiceInterface;

CommandService::CommandService(SwganhKernel* kernel)
: kernel_(kernel)
{
    command_factory_impl_ = kernel->GetPluginManager()->CreateObject<CommandFactoryInterface>("Command::CommandFactory");
    command_properties_manager_impl_ = kernel->GetPluginManager()->CreateObject<CommandPropertiesManagerInterface>("Command::CommandPropertiesManager");
    command_queue_manager_impl_ = kernel->GetPluginManager()->CreateObject<CommandQueueManagerInterface>("Command::CommandQueueManager");
    command_validator_impl_ = kernel->GetPluginManager()->CreateObject<CommandValidatorInterface>("Command::CommandValidator");
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

void CommandService::AddCommandCreator(swganh::HashString command, CommandCreator&& creator)
{
    command_factory_impl_->AddCommandCreator(command, std::move(creator));
}

void CommandService::RemoveCommandCreator(swganh::HashString command)
{
    command_factory_impl_->RemoveCommandCreator(command);
}

std::shared_ptr<CommandInterface> CommandService::CreateCommand(swganh::HashString command)
{
    return command_factory_impl_->CreateCommand(command);
}

void CommandService::EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command)
{
    command_queue_manager_impl_->EnqueueCommand(command);
}

void CommandService::EnqueueCommandRequest(
    const std::shared_ptr<swganh::object::Object>& object,
    CommandQueueEnqueue* command_request)
{
    auto command = command_factory_impl_->CreateCommand(command_request->command_crc);
    if (command)
    {
        auto swg_command = std::static_pointer_cast<BaseSwgCommand>(command);

		swg_command->SetController(object->GetController());
        swg_command->SetCommandRequest(*command_request);

        EnqueueCommand(swg_command);
    }
}

void CommandService::SetDefaultCommand(uint64_t queue_owner_id, const std::shared_ptr<swganh::command::CommandInterface>& command)
{
    command_queue_manager_impl_->SetDefaultCommand(queue_owner_id, command);
}

void CommandService::ClearDefaultCommand(uint64_t queue_owner_id)
{
    command_queue_manager_impl_->ClearDefaultCommand(queue_owner_id);
}

bool CommandService::HasDefaultCommand(uint64_t queue_owner_id)
{
    return command_queue_manager_impl_->HasDefaultCommand(queue_owner_id);
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
 
boost::optional<const CommandProperties&> CommandService::FindPropertiesForCommand(swganh::HashString command)
{
    return command_properties_manager_impl_->FindPropertiesForCommand(command);
}

void CommandService::Startup()
{
    script_prefix_ = kernel_->GetAppConfig().script_directory;
    
    simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
    simulation_service_->RegisterControllerHandler(&CommandService::EnqueueCommandRequest, this);

    auto event_dispatcher = kernel_->GetEventDispatcher();

    SubscribeObjectReadyEvent(event_dispatcher);
    SubscribeObjectRemovedEvent(event_dispatcher);
}

void CommandService::SendCommandQueueRemove(
    const std::shared_ptr<swganh::object::Object>& object,
    uint32_t action_counter,
    float default_time_sec,
    uint32_t error,
    uint32_t action)
{
	auto controller = object->GetController();
	if(controller != nullptr)
	{
		CommandQueueRemove remove;
		remove.action_counter = action_counter;
		remove.timer = default_time_sec;
		remove.error = error;
		remove.action = action;
		controller->Notify(&remove);
	}
}

void CommandService::SubscribeObjectReadyEvent(swganh::EventDispatcher* dispatcher)
{
    obj_ready_id_ = dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (std::shared_ptr<swganh::EventInterface> incoming_event)
    {
        const auto& object = std::static_pointer_cast<swganh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();

        command_queue_manager_impl_->AddQueue(
            object->GetObjectId(), 
            kernel_->GetPluginManager()->CreateObject<CommandQueueInterface>("Command::CommandQueue"));
    });
}

void CommandService::SubscribeObjectRemovedEvent(swganh::EventDispatcher* dispatcher)
{
    obj_removed_id_ = dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (std::shared_ptr<swganh::EventInterface> incoming_event)
    {
        const auto& object = std::static_pointer_cast<swganh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();
        command_queue_manager_impl_->RemoveQueue(object->GetObjectId());
    });
}

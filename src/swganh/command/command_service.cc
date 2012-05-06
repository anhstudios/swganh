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

#include "base_swg_command.h"
#include "command_properties_loader_interface.h"
#include "command_queue_interface.h"

using anh::service::ServiceDescription;
using swganh::app::SwganhKernel;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCreator;
using swganh::command::CommandFactoryInterface;
using swganh::command::CommandFilter;
using swganh::command::CommandInterface;
using swganh::command::CommandPropertiesLoaderInterface;
using swganh::command::CommandService;
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
    command_properties_loader_impl_ = kernel->GetPluginManager()->CreateObject<CommandPropertiesLoaderInterface>("Command::PropertiesLoader");
    command_validator_impl_ = kernel->GetPluginManager()->CreateObject<CommandValidatorInterface>("Command::Validator");
 
    command_properties_map_ = command_properties_loader_impl_->LoadCommandPropertiesMap();    
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

void CommandService::SetCommandCreator(anh::HashString command, CommandCreator&& creator)
{
    command_factory_impl_->AddCommandCreator(command, std::move(creator));
}

void CommandService::EnqueueCommand(std::unique_ptr<CommandInterface> command)
{
    boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
    
    auto find_iter = processor_map_.find(command->GetController()->GetId());
    if (find_iter != processor_map_.end() )
    {
        find_iter->second->EnqueueCommand(std::move(command));
    }
}

void CommandService::HandleCommandQueueEnqueue(
    const std::shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue command_request)
{
    auto properties_iter = command_properties_map_.find(command_request.command_crc);
    if (properties_iter == command_properties_map_.end())
    {
        LOG(warning) << "Invalid handler requested: " << std::hex << command_request.command_crc;
        return;
    }

    auto command = command_factory_impl_->CreateCommand(kernel_, properties_iter->second, controller, command_request);
    if (command)
    {
        EnqueueCommand(std::move(command));
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
    dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (std::shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = std::static_pointer_cast<anh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_[object->GetObjectId()] = kernel_->GetPluginManager()->CreateObject<CommandQueueInterface>("Command::Queue");
    });
}

void CommandService::SubscribeObjectRemovedEvent(anh::EventDispatcher* dispatcher)
{
    dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (std::shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = std::static_pointer_cast<anh::ValueEvent<std::shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_.erase(object->GetObjectId());
    });
}

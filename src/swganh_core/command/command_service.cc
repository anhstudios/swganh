// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "command_service.h"

#include <boost/filesystem.hpp>

#include "swganh/logger.h"
#include "swganh/event_dispatcher.h"
#include "swganh/plugin/plugin_manager.h"
#include "swganh/service/service_manager.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh/scripting/python_script.h"
#include "swganh/scripting/python_instance_creator.h"
#include "swganh_core/messages/controllers/command_queue_remove.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/object/object.h"

#include "base_swg_command.h"
#include "command_filter.h"
#include "command_properties_manager_interface.h"
#include "command_queue_interface.h"
#include "command_queue_manager_interface.h"

namespace bf = boost::filesystem;

using swganh::scripting::ScopedGilLock;
using swganh::scripting::PythonScript;
using swganh::scripting::PythonInstanceCreator;
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
    SetServiceDescription(ServiceDescription(
        "CommandService",
        "command",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0));
}

CommandService::~CommandService()
{}

void CommandService::AddCommandEnqueueFilter(CommandFilter&& filter)
{
    command_validator_impl_->AddCommandEnqueueFilter(std::move(filter));
}

void CommandService::AddCommandProcessFilter(CommandFilter&& filter)
{
    command_validator_impl_->AddCommandProcessFilter(std::move(filter));
}

void CommandService::AddCommandCreator(swganh::HashString command, CommandCreator creator)
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
    else
    {
        LOG(error) << "Invalid command requested: (" << std::hex << command_request->command_crc << ")";
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

void CommandService::Initialize()
{
    command_factory_impl_ = kernel_->GetPluginManager()->CreateObject<CommandFactoryInterface>("Command::CommandFactory");
    command_factory_impl_->Initialize(kernel_);

    command_properties_manager_impl_ = kernel_->GetPluginManager()->CreateObject<CommandPropertiesManagerInterface>("Command::CommandPropertiesManager");
    command_queue_manager_impl_ = kernel_->GetPluginManager()->CreateObject<CommandQueueManagerInterface>("Command::CommandQueueManager");
    command_validator_impl_ = kernel_->GetPluginManager()->CreateObject<CommandValidatorInterface>("Command::CommandValidator");
    
    simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
}

void CommandService::Startup()
{
    // add default filters
    AddCommandEnqueueFilter(std::bind(&CommandFilters::TargetCheckFilter, std::placeholders::_1));
    AddCommandEnqueueFilter(std::bind(&CommandFilters::PostureCheckFilter, std::placeholders::_1));
    AddCommandEnqueueFilter(std::bind(&CommandFilters::StateCheckFilter, std::placeholders::_1));
    AddCommandEnqueueFilter(std::bind(&CommandFilters::AbilityCheckFilter, std::placeholders::_1));
    AddCommandEnqueueFilter(std::bind(&CommandFilters::CombatTargetCheckFilter, std::placeholders::_1));
    AddCommandProcessFilter(std::bind(&CommandFilters::TargetCheckFilter, std::placeholders::_1));
    AddCommandProcessFilter(std::bind(&CommandFilters::PostureCheckFilter, std::placeholders::_1));
    AddCommandProcessFilter(std::bind(&CommandFilters::StateCheckFilter, std::placeholders::_1));
    AddCommandProcessFilter(std::bind(&CommandFilters::AbilityCheckFilter, std::placeholders::_1));
    AddCommandProcessFilter(std::bind(&CommandFilters::CombatTargetCheckFilter, std::placeholders::_1));

    script_prefix_ = kernel_->GetAppConfig().script_directory;
    
    simulation_service_->RegisterControllerHandler(&CommandService::EnqueueCommandRequest, this);

    auto event_dispatcher = kernel_->GetEventDispatcher();

    SubscribeObjectReadyEvent(event_dispatcher);
    SubscribeObjectRemovedEvent(event_dispatcher);

    LoadPythonCommands();
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

void CommandService::LoadPythonCommands()
{
    bf::path p(kernel_->GetAppConfig().script_directory + "/commands");

    try 
    {
        if (bf::exists(p) && bf::is_directory(p))
        {
            std::for_each(bf::directory_iterator(p), bf::directory_iterator(),
                [this] (const bf::directory_entry& entry)
            {
                std::string script_path = entry.path().string();
                std::string extension = bf::extension(entry.path());

                if (extension.compare(".py") == 0)
                {
                    LoadPythonCommands(script_path);
                }
            });
        }
    }
    catch (const bf::filesystem_error& ex)
    {
        LOG(error) << ex.what();
    }
}

void CommandService::LoadPythonCommands(const std::string& command_script)
{
    PythonScript script(command_script);

    auto globals = script.GetGlobals();
        
    ScopedGilLock lock;
    try {
        auto keys = globals.keys();

        auto length = boost::python::len(keys);
        for (int i = 0; i < length; ++i)
        {
            std::string key = boost::python::extract<std::string>(keys[i]);

            if (key.find("Command") != std::string::npos && key.find("Base") == std::string::npos 
                && commands_loaded_.find(key) == commands_loaded_.end())
            {
                auto creator = PythonInstanceCreator<CommandInterface>(command_script, key);
                auto command = creator();

                commands_loaded_.insert(key);

                AddCommandCreator(command->GetCommandName(), creator);
            }
        }
    }
    catch (boost::python::error_already_set&)
    {
        ScopedGilLock lock;
		swganh::scripting::logPythonException();
    }
}

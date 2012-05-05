// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "command_service.h"

#include <cctype>

#include <boost/filesystem.hpp>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "anh/logger.h"

#include "anh/crc.h"
#include "anh/event_dispatcher.h"
#include "anh/database/database_manager_interface.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/command/command_filter.h"
#include "swganh/command/command_interface.h"

#include "swganh/messages/controllers/combat_action_message.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

#include "python_command.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/scripting/python_event.h"

#include "swganh/tre/tre_archive.h"
#include "swganh/tre/readers/datatable_reader.h"

#include "command_properties_loader_interface.h"
#include "command_queue_interface.h"
#include "base_swg_command.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;
using namespace swganh::scripting;
using namespace swganh::simulation;

using boost::asio::deadline_timer;
using boost::posix_time::milliseconds;
using swganh::app::SwganhKernel;
using swganh::tre::readers::DatatableReader;

CommandService::CommandService(SwganhKernel* kernel)
: kernel_(kernel)
{
    command_factory_impl_ = kernel->GetPluginManager()->CreateObject<CommandFactoryInterface>("Command::CommandFactory");
    command_properties_loader_impl_ = kernel->GetPluginManager()->CreateObject<CommandPropertiesLoaderInterface>("Command::PropertiesLoader");
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
	enqueue_filters_.push_back(move(filter));
}

void CommandService::AddCommandProcessFilter(CommandFilter&& filter)
{
    process_filters_.push_back(move(filter));
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
    const shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue command_request)
{
    auto properties_iter = command_properties_map_.find(command_request.command_crc);
    if (properties_iter == command_properties_map_.end())
    {
        LOG(warning) << "Invalid handler requested: " << hex << command_request.command_crc;
        return;
    }

    auto command = command_factory_impl_->CreateCommand(kernel_, properties_iter->second, controller, command_request);
    if (command)
    {
        EnqueueCommand(std::move(command));
    }
}
        
bool CommandService::ValidateCommandForEnqueue(CommandInterface* command)
{
    return ValidateCommand(static_cast<BaseSwgCommand*>(command), enqueue_filters_);
}

bool CommandService::ValidateCommandForProcessing(CommandInterface* command)
{
    return ValidateCommand(static_cast<BaseSwgCommand*>(command), process_filters_);
}

void CommandService::Start()
{
    script_prefix_ = kernel_->GetAppConfig().script_directory;
    
    command_properties_map_ = command_properties_loader_impl_->LoadCommandPropertiesMap();
    
    simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");

    simulation_service_->RegisterControllerHandler(&CommandService::HandleCommandQueueEnqueue, this);

	auto event_dispatcher = kernel_->GetEventDispatcher();
	event_dispatcher->Dispatch(
        make_shared<anh::ValueEvent<CommandPropertiesMap>>("CommandServiceReady", GetCommandProperties()));

    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_[object->GetObjectId()] = kernel_->GetPluginManager()->CreateObject<CommandQueueInterface>("Command::Queue");
    });

    event_dispatcher->Subscribe(
        "ObjectRemovedEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();

        boost::lock_guard<boost::mutex> lg(processor_map_mutex_);
        processor_map_.erase(object->GetObjectId());
    });
}

bool CommandService::ValidateCommand(
    BaseSwgCommand* command,
    const std::vector<CommandFilter>& filters)
{
	tuple<bool, uint32_t, uint32_t> result;

    bool all_run = all_of(
        begin(filters),
        end(filters),
        [&result, command] (const CommandFilter& filter)->bool
    {
        result = filter(command);
		return get<0>(result);
    });

    if (!all_run)
    {
        SendCommandQueueRemove(command->GetActor(), command->GetActionCounter(), 0.0f, get<1>(result), get<2>(result));
    }

    return all_run;
}

void CommandService::SendCommandQueueRemove(
    const shared_ptr<Creature>& actor,
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

	actor->GetController()->Notify(remove);
}

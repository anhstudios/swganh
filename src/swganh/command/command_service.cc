
#include "swganh/command/command_service.h"

#include "anh/app/kernel_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/messages/controllers/command_queue_remove.h"

#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"

#include "swganh/simulation/simulation_service.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::simulation;

CommandService::CommandService(KernelInterface* kernel)
: BaseService(kernel)
{}

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

void CommandService::AddCommandHandler(uint32_t command_crc, CommandHandler&& handler)
{
    handlers_[command_crc] = move(handler);
}

void CommandService::HandleCommandQueueEnqueue(
    const shared_ptr<ObjectController>& controller, 
    const ObjControllerMessage& message)
{
    CommandQueueEnqueue enqueue;
    enqueue.Deserialize(message.data);

    command_queues_[controller->GetObject()->GetObjectId()].push(enqueue);
}

void CommandService::HandleCommandQueueRemove(
    const shared_ptr<ObjectController>& controller, 
    const ObjControllerMessage& message)
{}

void CommandService::ProcessNextCommand()
{
    for_each(
        begin(command_queues_),
        end(command_queues_),
        [this] (CommandQueueMap::value_type& object_commands)
    {
        CommandQueueEnqueue command; 
        if (!object_commands.second.try_pop(command))
        {
            return;
        }

        auto find_iter = handlers_.find(command.command_crc);

        if (find_iter != handlers_.end())
        {
            find_iter->second(
                object_commands.first,
                command.target_id,
                command.command_options);
        }
    });
}

void CommandService::onStart()
{
    auto simulation_service = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");
    
    simulation_service->RegisterControllerHandler(0x00000116, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        HandleCommandQueueEnqueue(controller, message);
    });
    
    simulation_service->RegisterControllerHandler(0x00000117, [this] (
        const std::shared_ptr<ObjectController>& controller, 
        const swganh::messages::ObjControllerMessage& message) 
    {
        HandleCommandQueueRemove(controller, message);
    });

    active().AsyncRepeated(boost::posix_time::milliseconds(0),
        [=] ()
    {
        ProcessNextCommand();
    });
}

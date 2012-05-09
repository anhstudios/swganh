// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_factory.h"

#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/command/command_interface.h"
#include "swganh/command/command_properties.h"
#include "swganh/command/command_service.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/object/object_controller.h"

using pub14_core::command::CommandFactory;
using swganh::app::SwganhKernel;
using swganh::command::CommandCreator;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::command::CommandService;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;

CommandFactory::CommandFactory(SwganhKernel* kernel)
    : kernel_(kernel)
    , command_service_(nullptr)
{}

CommandFactory::~CommandFactory()
{}

void CommandFactory::AddCommandCreator(anh::HashString command, swganh::command::CommandCreator&& creator)
{    
    auto properties = GetCommandService()->FindPropertiesForCommand(command);

    if (properties)
    {
        boost::lock_guard<boost::mutex> lg(creators_mutex_);
        command_creators_.insert(std::make_pair(command, CreatorData(std::move(creator), *properties)));
    }
}

void CommandFactory::RemoveCommandCreator(anh::HashString command)
{
    boost::lock_guard<boost::mutex> lg(creators_mutex_);
    command_creators_.erase(command);
}

std::shared_ptr<CommandInterface> CommandFactory::CreateCommand(
    const std::shared_ptr<ObjectController>& controller,
    const CommandQueueEnqueue& command_request)
{
    std::shared_ptr<CommandInterface> command = nullptr;

    boost::lock_guard<boost::mutex> lg(creators_mutex_);

    auto creators_iter = command_creators_.find(command_request.command_crc);
    if (creators_iter != command_creators_.end())
    {
        auto& creator = creators_iter->second;
        command = creator.creator_func(kernel_, creator.properties, controller, command_request);
    }

    return command;
}

swganh::command::CommandService* CommandFactory::GetCommandService()
{
    if (!command_service_)
    {
        command_service_ = kernel_->GetServiceManager()->GetService<CommandService>("CommandService");
    }

    return command_service_;
}
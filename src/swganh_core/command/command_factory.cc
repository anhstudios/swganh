// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_factory.h"

#include "swganh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/command/command_interface.h"
#include "swganh/command/command_properties.h"
#include "swganh/command/command_service_interface.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"
#include "swganh/observer/observer_interface.h"

using swganh::command::CommandFactory;
using swganh::app::SwganhKernel;
using swganh::command::CommandCreator;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::command::CommandServiceInterface;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;

CommandFactory::CommandFactory(SwganhKernel* kernel)
    : kernel_(kernel)
    , command_service_(nullptr)
{}

CommandFactory::~CommandFactory()
{}

void CommandFactory::AddCommandCreator(swganh::HashString command, swganh::command::CommandCreator&& creator)
{    
    auto properties = GetCommandService()->FindPropertiesForCommand(command);

    if (properties)
    {
        boost::lock_guard<boost::mutex> lg(creators_mutex_);
        command_creators_.insert(std::make_pair(command, CreatorData(std::move(creator), *properties)));
    }
}

void CommandFactory::RemoveCommandCreator(swganh::HashString command)
{
    boost::lock_guard<boost::mutex> lg(creators_mutex_);
    command_creators_.erase(command);
}

std::shared_ptr<CommandInterface> CommandFactory::CreateCommand(swganh::HashString command)
{
    std::shared_ptr<CommandInterface> new_command = nullptr;

    boost::lock_guard<boost::mutex> lg(creators_mutex_);

    auto creators_iter = command_creators_.find(command);
    if (creators_iter != command_creators_.end())
    {
        auto& creator = creators_iter->second;
        new_command = creator.creator_func(kernel_, creator.properties);
    }

    return new_command;
}

swganh::command::CommandServiceInterface* CommandFactory::GetCommandService()
{
    if (!command_service_)
    {
        command_service_ = kernel_->GetServiceManager()->GetService<CommandServiceInterface>("CommandService");
    }

    return command_service_;
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_factory.h"

#include <boost/thread/lock_guard.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/service/service_manager.h"

#include "command_interface.h"
#include "command_properties.h"
#include "command_service_interface.h"

using namespace swganh::command;
using swganh::app::SwganhKernel;

CommandFactory::CommandFactory() {}

CommandFactory::~CommandFactory() {}

void CommandFactory::Initialize(SwganhKernel* kernel)
{
    kernel_ = kernel;
    command_service_ = kernel_->GetServiceManager()->GetService<CommandServiceInterface>("CommandService");
}

bool CommandFactory::IsRegistered(swganh::HashString command)
{
    return command_creators_.find(command) != command_creators_.end();
}

void CommandFactory::AddCommandCreator(swganh::HashString command, swganh::command::CommandCreator creator)
{    
    auto properties = command_service_->FindPropertiesForCommand(command);

    if (!properties)
    {        
        CommandProperties new_properties;
        new_properties.command_name = command.ident_string();
        new_properties.allow_in_locomotion = 0xffffffffffffffff;
        new_properties.allow_in_state = 0xffffffffffffffff;

        properties.reset(new_properties);
    }

    {
        boost::lock_guard<boost::mutex> lg(creators_mutex_);
        command_creators_.insert(std::make_pair(command, std::make_shared<CreatorData>(std::move(creator), *properties)));
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
        new_command = creator->creator_func();
        new_command->Initialize(kernel_, creator->properties);
    }

    return new_command;
}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_factory.h"

#include "swganh/command/v2/command_interface.h"
#include "swganh/object/object_controller.h"

using pub14_core::command::CommandFactory;
using swganh::command::v2::CommandInterface;
using swganh::object::ObjectController;

CommandFactory::CommandFactory()
{}

CommandFactory::~CommandFactory()
{}

std::unique_ptr<CommandInterface> CommandFactory::CreateCommand(
    const std::shared_ptr<ObjectController>& controller,
    uint32_t command_crc,
    uint64_t target_id,
    uint32_t action_counter,
    std::wstring command_options)
{
    std::unique_ptr<CommandInterface> command(nullptr);

    auto find_iter = command_creator_map_.find(command_crc);
    if (find_iter != command_creator_map_.end())
    {
        command = find_iter->second(
            controller, command_crc, target_id, action_counter, command_options);
    }

    return command;
}

void CommandFactory::AddCommandCreator(anh::HashString command, swganh::command::v2::CommandCreator&& creator)
{
    if (!HasCreatorForCommand(command))
    {
        command_creator_map_.insert(std::make_pair(command, std::move(creator)));
    }
}

void CommandFactory::RemoveCommandCreator(anh::HashString command)
{
    command_creator_map_.erase(command);
}

bool CommandFactory::HasCreatorForCommand(anh::HashString command)
{
    return command_creator_map_.find(command) != command_creator_map_.end();
}

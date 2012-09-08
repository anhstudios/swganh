// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_validator.h"

#include <algorithm>

#include "swganh/command/base_swg_command.h"

using swganh::command::CommandValidator;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandFilter;
using swganh::command::CommandInterface;

CommandValidator::~CommandValidator()
{}

void CommandValidator::AddCommandEnqueueFilter(swganh::command::CommandFilter&& filter)
{
	enqueue_filters_.push_back(std::move(filter));
}

void CommandValidator::AddCommandProcessFilter(swganh::command::CommandFilter&& filter)
{
    process_filters_.push_back(std::move(filter));
}

std::tuple<bool, uint32_t, uint32_t> CommandValidator::ValidateForEnqueue(swganh::command::CommandInterface* command)
{
    return ValidateCommand(command, enqueue_filters_);
}

std::tuple<bool, uint32_t, uint32_t> CommandValidator::ValidateForProcessing(swganh::command::CommandInterface* command)
{
    return ValidateCommand(command, process_filters_);
}

std::tuple<bool, uint32_t, uint32_t> CommandValidator::ValidateCommand(
    swganh::command::CommandInterface* command,
    const std::vector<CommandFilter>& filters)
{
	std::tuple<bool, uint32_t, uint32_t> result;

    std::all_of(
        begin(filters),
        end(filters),
        [&result, command] (const CommandFilter& filter)->bool
    {
        result = filter(command);
		return std::get<0>(result);
    });

    return result;
}

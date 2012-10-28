// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "request_badges_command.h"

using swganh::app::SwganhKernel;
using swganh::badge::RequestBadgesCommand;
using swganh::command::CommandProperties;
using swganh::command::CommandCallback;

RequestBadgesCommand::RequestBadgesCommand(SwganhKernel* kernel, const CommandProperties& properties)
	: BaseSwgCommand(kernel, properties)
{
}

boost::optional<std::shared_ptr<CommandCallback>> RequestBadgesCommand::Run()
{
	return boost::optional<std::shared_ptr<CommandCallback>>();
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "request_badges_command.h"

#include <swganh/app/swganh_kernel.h>
#include <swganh/service/service_manager.h>
#include "swganh_core/simulation/simulation_service.h"

#include <swganh_core/object/object.h>
#include <swganh_core/object/player/player.h>
#include <swganh_core/messages/badges_response_message.h>

using swganh::app::SwganhKernel;
using swganh::badge::RequestBadgesCommand;
using swganh::command::CommandProperties;
using swganh::command::CommandCallback;

using namespace swganh::service;
using namespace swganh::simulation;
using namespace swganh::app;

RequestBadgesCommand::RequestBadgesCommand(SwganhKernel* kernel, const CommandProperties& properties)
	: BaseSwgCommand(kernel, properties)
	, kernel_(kernel)
{
}

boost::optional<std::shared_ptr<CommandCallback>> RequestBadgesCommand::Run()
{
	// Find player object.
	auto simulation = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
	auto player = std::static_pointer_cast<swganh::object::Player>(simulation->GetObjectById(GetActor()->GetObjectId() + 1));
	
	swganh::messages::BadgesResponseMessage badges_response;
	badges_response.character_id = GetActor()->GetObjectId();
	badges_response.badge_flags = player->GetBadges();

	GetActor()->GetController()->Notify(&badges_response);

	return boost::optional<std::shared_ptr<CommandCallback>>();
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "request_badges_command.h"

#include <swganh/app/swganh_kernel.h>
#include <swganh/service/service_manager.h>
#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/equipment/equipment_service.h"

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
using namespace swganh::equipment;

uint32_t GetBadgeBitmaskIndexById2(uint32_t id)
{
	return (uint32_t)floor((double)((id)/32));
}

uint8_t GetBadgeBitmaskBitById2(uint32_t id)
{
	return (id)%32;
}

RequestBadgesCommand::RequestBadgesCommand(SwganhKernel* kernel, const CommandProperties& properties)
	: BaseSwgCommand(kernel, properties)
	, kernel_(kernel)
{
	equipment_service_ = kernel_->GetServiceManager()->GetService<EquipmentService>("EquipmentService");
}

boost::optional<std::shared_ptr<CommandCallback>> RequestBadgesCommand::Run()
{
	// Find player object.
	auto player = std::static_pointer_cast<swganh::object::Player>(equipment_service_->GetEquippedObject(GetActor(), "ghost"));

	swganh::messages::BadgesResponseMessage badges_response;
	badges_response.character_id = GetActor()->GetObjectId();

	auto badges = player->GetBadges();
	std::for_each(badges.begin(), badges.end(), [=, &badges_response](const uint32_t id) {
		badges_response.badge_flags[GetBadgeBitmaskIndexById2(id)].at(GetBadgeBitmaskBitById2(id)) = true;
	});

	GetActor()->GetController()->Notify(&badges_response);

	return boost::optional<std::shared_ptr<CommandCallback>>();
}
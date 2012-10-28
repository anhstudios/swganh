// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/badge/badge_service.h>

#include <swganh/command/command_service_interface.h>
#include <swganh/command/command_properties.h>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "swganh/database/database_manager.h"

#include <swganh_core/badge/request_badges_command.h>

#include <swganh_core/messages/system_message.h>
#include <swganh_core/messages/out_of_band.h>

#include <swganh/event_dispatcher.h>
#include <swganh/app/swganh_kernel.h>
#include <swganh/service/service_manager.h>
#include <swganh_core/object/object.h>
#include <swganh_core/object/player/player.h>

#include "badge_region.h"
#include "swganh/service/service_manager.h"
#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/simulation/scene_events.h"
#include "swganh_core/object/permissions/world_permission.h"

using namespace swganh::badge;
using namespace swganh::object;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::service;
using namespace swganh::simulation;

BadgeService::BadgeService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
}

BadgeService::~BadgeService(void)
{
}

void BadgeService::Startup()
{
	command_service_ = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");

	// Load Badges
	// Load Badge Regions
	
	// Insert our badge regions into the scene when created.
	kernel_->GetEventDispatcher()->Subscribe("SceneManager:NewScene", [&] (const std::shared_ptr<swganh::EventInterface>& newEvent)
	{

		//Temp
		auto real_event = std::static_pointer_cast<swganh::simulation::NewSceneEvent>(newEvent);

		auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");

		std::shared_ptr<swganh::object::regions::BadgeRegion> badge_region = std::make_shared<swganh::object::regions::BadgeRegion>("exp_cor_agrilat_swamp", this);
		badge_region->SetEventDispatcher(kernel_->GetEventDispatcher());
		badge_region->SetPosition(glm::vec3(1084, 0, 4176));
		badge_region->SetSceneId(real_event->scene_id);
		badge_region->SetObjectId(0xDEADBABE);
		badge_region->SetInSnapshot(true);
		badge_region->SetPermissions(std::make_shared<swganh::object::WorldPermission>());
		simulation_service->TransferObjectToScene(badge_region, real_event->scene_label);
	});
	
	// Subscribe to requestbadges CommandQueueEnqueue.
	command_service_->AddCommandCreator("requestbadges", [] (swganh::app::SwganhKernel* kernel, const CommandProperties& properties)
	{
		return std::make_shared<RequestBadgesCommand>(kernel, properties);
	});
}

swganh::service::ServiceDescription BadgeService::GetServiceDescription()
{
	return swganh::service::ServiceDescription("BadgeService", "badge", "0.1", "127.0.0.1", 0, 0, 0);
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, std::string name)
{
	if(!player->HasController())
		return;

	// Lookup badge loaded badges by name.
	// Toggle badge bitmask and commit.

	// Player system message feedback.
	SystemMessage::PlayMusic(player, "music_emperor_theme_loop", 0);
	SystemMessage::Send(player, messages::OutOfBand("badge_n", "exp_cor_agrilat_swamp"), false, false);
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, uint32_t id)
{
}
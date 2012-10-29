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
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

#include <swganh_core/badge/request_badges_command.h>

#include <swganh_core/messages/system_message.h>
#include <swganh_core/messages/play_music_message.h>
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
	// Insert our badge regions into the scene when created.
	kernel_->GetEventDispatcher()->Subscribe("SceneManager:NewScene", [&] (const std::shared_ptr<swganh::EventInterface>& newEvent)
	{
		auto real_event = std::static_pointer_cast<swganh::simulation::NewSceneEvent>(newEvent);
		auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");

		std::for_each(badge_regions_.begin(), badge_regions_.end(), [=](std::shared_ptr<swganh::badge::BadgeRegion> badge_region) {
			if(real_event->scene_id == badge_region->GetSceneId())
			{
				simulation_service->TransferObjectToScene(badge_region, real_event->scene_label);
			}
		});
	});
}

BadgeService::~BadgeService(void)
{
}

void BadgeService::Startup()
{
	command_service_ = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");
	auto conn = kernel_->GetDatabaseManager()->getConnection("swganh_static");

	// Load Badges
	try {
		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetBadges();"));

		while(result->next())
		{
			std::shared_ptr<Badge> badge = std::make_shared<Badge>();
			badge->id = result->getUInt(1);
			badge->name = result->getString(2);
			badge->type = (BadgeType)result->getUInt(3);
			badge->sound = result->getString(4);
			badges_.push_back(badge);
		}	while(statement->getMoreResults());
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}

	// Load Badge Regions
	try {
		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetBadgeRegions();"));

		while(result->next())
		{
			std::shared_ptr<swganh::badge::BadgeRegion> badge_region = std::make_shared<swganh::badge::BadgeRegion>(result->getString(2), this);
			badge_region->SetEventDispatcher(kernel_->GetEventDispatcher());
			badge_region->SetPosition(glm::vec3(result->getDouble(7), 0.0f, result->getDouble(8)));
			badge_region->SetSceneId(result->getUInt(3) + 1);
			
			auto name = result->getString(5).asStdString();
			badge_region->SetCustomName(std::wstring(name.begin(), name.end()));
			
			badge_region->SetObjectId(result->getUInt64(1));
			badge_region->SetInSnapshot(true);
			
			badge_region->SetCollisionBoxSize((float)result->getDouble(9), (float)result->getDouble(10));
			badge_region->SetPermissions(std::make_shared<swganh::object::WorldPermission>());
			
			badge_regions_.push_back(badge_region);
		} while(statement->getMoreResults());
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
	
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
	GiveBadge(player, FindBadge(name));
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, uint32_t id)
{
	GiveBadge(player, FindBadge(id));
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, std::shared_ptr<Badge> badge)
{
	if(!player->HasController())
		return;

	// Lookup badge loaded badges by name.
	if(badge == nullptr)
	{
		LOG(error) << "Badge " << badge->name << " does not exist.";
	}

	// Find player object.
	auto simulation = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
	auto play = std::static_pointer_cast<Player>(simulation->GetObjectById(player->GetObjectId() + 1));
	
	// Don't give a badge twice.
	if(play->HasBadge((uint32_t)floor((double)(badge->id/32)), badge->id%32))
		return;

	// Toggle badge bitmask and commit to Mysql.	
	play->ToggleBadge((uint32_t)floor((double)(badge->id/32)), badge->id%32);

	// Player system message feedback.
	messages::PlayMusicMessage music_message;
	music_message.music_file = badge->sound;
	music_message.channel = 0;

	player->GetController()->Notify(&music_message);
	SystemMessage::Send(player, messages::OutOfBand("badge_n", badge->name), false, false);
}

const std::shared_ptr<Badge> BadgeService::FindBadge(uint32_t id)
{
	auto i = std::find_if(badges_.begin(), badges_.end(), [=](std::shared_ptr<Badge> badge) {
		return (badge->id == id);
	});

	if(i != badges_.end())
		return (*i);
	else
		return nullptr;
}

const std::shared_ptr<Badge> BadgeService::FindBadge(std::string name)
{
	auto i = std::find_if(badges_.begin(), badges_.end(), [=](std::shared_ptr<Badge> badge) {
		return (badge->name == name);
	});

	if(i != badges_.end())
		return (*i);
	else
		return nullptr;
}
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
#include "swganh_core/equipment/equipment_service.h"
#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/simulation/scene_events.h"
#include "swganh_core/object/permissions/world_permission.h"

using namespace swganh::badge;
using namespace swganh::object;
using namespace swganh::command;
using namespace swganh::messages;
using namespace swganh::service;
using namespace swganh::simulation;
using namespace swganh::equipment;

uint32_t GetBadgeBitmaskIndexById(uint32_t id)
{
	return (uint32_t)floor((double)((id)/32));
}

uint8_t GetBadgeBitmaskBitById(uint32_t id)
{
	return (id)%32;
}

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
	equipment_service_ = kernel_->GetServiceManager()->GetService<EquipmentService>("EquipmentService");
	
	auto conn = kernel_->GetDatabaseManager()->getConnection("swganh_static");

	// Load Badges
	try {
		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetBadges();"));

		while(result->next())
		{
			std::shared_ptr<Badge> badge = std::make_shared<Badge>();
			badge->id = result->getUInt("id");
			badge->name = result->getString("name");
			badge->type = (BadgeType)result->getUInt("type");
			badge->sound = result->getString("sound");
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
			std::shared_ptr<swganh::badge::BadgeRegion> badge_region = std::make_shared<swganh::badge::BadgeRegion>(result->getString("name"), this);
			
			badge_region->SetPosition(glm::vec3(result->getDouble("x"), 0.0f, result->getDouble("z")));
			badge_region->SetSceneId(result->getUInt("planet_id") + 1);
			badge_region->SetDatabasePersisted(false);
			
			auto name = result->getString("name").asStdString();
			badge_region->SetCustomName(std::wstring(name.begin(), name.end()));
			
			badge_region->SetObjectId(result->getUInt64("id"));
			badge_region->SetInSnapshot(true);
			badge_region->SetDatabasePersisted(false);
			
			badge_region->SetCollisionBoxSize((float)result->getDouble("width"), (float)result->getDouble("length"));
			badge_region->SetPermissions(std::make_shared<swganh::object::WorldPermission>());
			badge_region->SetEventDispatcher(kernel_->GetEventDispatcher());
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
	return swganh::service::ServiceDescription(
		"BadgeService", // namve
		"badge", // type
		"0.1", // version
		"127.0.0.1", // address
		0, // tcp port
		0, // udp port
		0); // status
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, std::string name)
{
	GiveBadge(player, FindBadge(name));
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, uint32_t id)
{
	GiveBadge(player, FindBadge(id));
}

bool BadgeService::HasBadge(std::shared_ptr<Object> object, std::string name)
{
	auto badge = FindBadge(name);
	if(badge == nullptr)
		return false;
	
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(object, "ghost"));
	return player->HasBadge(badge->GetIndex(), badge->GetBit());

}

bool BadgeService::HasBadge(std::shared_ptr<Object> object, uint32_t id)
{
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(object, "ghost"));
	return player->HasBadge(GetBadgeBitmaskIndexById(id), GetBadgeBitmaskBitById(id));
}

void BadgeService::GiveBadge(std::shared_ptr<Object> player, std::shared_ptr<Badge> badge)
{
	if(!player->HasController()) {
		std::cout << "no controller" << " " << player->IsCollidable() << std::endl;
		return;
	}

	// Lookup badge loaded badges by name.
	if(badge == nullptr)
	{
		LOG(error) << "Badge does not exist.";
	}

	// Find player object.
	auto play = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(player, "ghost"));
	if(play == nullptr)
		return;

	// Don't give a badge twice.
	if(play->HasBadge(badge->GetIndex(), badge->GetBit()))
		return;

	// Toggle badge bitmask and commit to Mysql.	
	play->AddBadge(badge->id);

	try
	{
		auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
		auto statement = conn->createStatement();
	
		std::stringstream query;
		query << "INSERT INTO player_badges (`player_id`, `badge_id`) VALUES(" << play->GetObjectId() << ", " << badge->id << ");";

		statement->execute(query.str());
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}

	// Player system message feedback.
	messages::PlayMusicMessage music_message;
	music_message.music_file = badge->sound;
	music_message.channel = 0;

	player->GetController()->Notify(&music_message);
	SystemMessage::Send(player, messages::OutOfBand("badge_n", badge->name), false, false);

	CheckBadgeAccumulation(player);

	if(badge->type == EXPLORATION_DANGEROUS || badge->type == EXPLORATION_EASY || badge->type == EXPLORATION_JEDI)
		CheckExplorationBadgeAccumulation(player);
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

void BadgeService::CheckBadgeAccumulation(std::shared_ptr<Object> object)
{
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(object, "ghost"));

	uint32_t badge_count = 0;
	for(uint32_t x = 0; x < player->GetBadges().size(); x++)
		badge_count += player->GetBadges().at(x).count();

	if(badge_count >= 5 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_5), GetBadgeBitmaskBitById(COUNT_5)) == false)
		GiveBadge(object, "count_5");
	if(badge_count >= 10 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_10), GetBadgeBitmaskBitById(COUNT_10)) == false)
		GiveBadge(object, "count_10");
	if(badge_count >= 25 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_25), GetBadgeBitmaskBitById(COUNT_25)) == false)
		GiveBadge(object, "count_25");
	if(badge_count >= 50 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_50), GetBadgeBitmaskBitById(COUNT_50)) == false)
		GiveBadge(object, "count_50");
	if(badge_count >= 75 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_75), GetBadgeBitmaskBitById(COUNT_75)) == false)
		GiveBadge(object, "count_75");
	if(badge_count >= 100 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_100), GetBadgeBitmaskBitById(COUNT_100)) == false)
		GiveBadge(object, "count_100");
	if(badge_count >= 125 && player->HasBadge(GetBadgeBitmaskIndexById(COUNT_125), GetBadgeBitmaskBitById(COUNT_125)) == false)
		GiveBadge(object, "count_125");
}

void BadgeService::CheckExplorationBadgeAccumulation(std::shared_ptr<Object> object)
{
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(object, "ghost"));

	uint32_t badge_count = 0;

	// This needs further optimization.
	std::for_each(badges_.begin(), badges_.end(), [=, &badge_count](std::shared_ptr<Badge> badge) {
		if(badge->type == EXPLORATION_DANGEROUS || badge->type == EXPLORATION_EASY || badge->type == EXPLORATION_JEDI)
		{
			if(player->HasBadge(badge->GetIndex(), badge->GetBit()))
				badge_count += 1;
		}
	});

	if(badge_count >= 10 && player->HasBadge(GetBadgeBitmaskIndexById(EXP_COUNT_10), GetBadgeBitmaskBitById(EXP_COUNT_10)) == false)
		GiveBadge(object, "bdg_exp_10_badges");
	if(badge_count >= 20 && player->HasBadge(GetBadgeBitmaskIndexById(EXP_COUNT_20), GetBadgeBitmaskBitById(EXP_COUNT_20)) == false)
		GiveBadge(object, "bdg_exp_20_badges");
	if(badge_count >= 30 && player->HasBadge(GetBadgeBitmaskIndexById(EXP_COUNT_30), GetBadgeBitmaskBitById(EXP_COUNT_30)) == false)
		GiveBadge(object, "bdg_exp_30_badges");
	if(badge_count >= 10 && player->HasBadge(GetBadgeBitmaskIndexById(EXP_COUNT_40), GetBadgeBitmaskBitById(EXP_COUNT_40)) == false)
		GiveBadge(object, "bdg_exp_40_badges");
	if(badge_count >= 45 && player->HasBadge(GetBadgeBitmaskIndexById(EXP_COUNT_45), GetBadgeBitmaskBitById(EXP_COUNT_45)) == false)
		GiveBadge(object, "bdg_exp_45_badges");
}
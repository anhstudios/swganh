// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh_core/map/map_service.h>

#include <swganh/app/swganh_kernel.h>
#include <swganh/event_dispatcher.h>
#include <swganh/service/service_manager.h>
#include <swganh/database/database_manager.h>
#include <swganh/logger.h>

#include <swganh_core/simulation/simulation_service.h>
#include <swganh/connection/connection_service_interface.h>
#include <swganh/connection/connection_client_interface.h>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <swganh_core/messages/get_map_locations_request_message.h>


using namespace swganh::map;

using swganh::connection::ConnectionServiceInterface;
using swganh::service::ServiceDescription;
using swganh::simulation::SimulationService;
using swganh::connection::ConnectionClientInterface;
using swganh::messages::MapLocation;
using swganh::messages::GetMapLocationsRequestMessage;
using swganh::messages::GetMapLocationsResponseMessage;

MapService::MapService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
}

MapService::~MapService()
{
}

ServiceDescription MapService::GetServiceDescription()
{
	return ServiceDescription(
		"MapService",
		"map",
		"0.1",
		"127.0.0.1",
		0,
		0,
		0
		);
}

void MapService::Startup()
{
	simulation_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");

	// Get STATIC map locations.
	try
	{
		auto conn = kernel_->GetDatabaseManager()->getConnection("swganh_static");
		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetMapLocations"));

		while(result->next())
		{
			MapLocation location;
			location.id = result->getUInt("id");
			auto name = result->getString("name").asStdString();
			location.name = std::wstring(name.begin(), name.end());
			location.x = (float)result->getDouble("x");
			location.y = (float)result->getDouble("y");
			location.type_displayAsCategory = result->getUInt("category_main");
			location.type_displayAsSubcategory = result->getUInt("category_sub");

			uint32_t scene_id = result->getUInt("scene_id") + 1;
			InsertLocation(scene_id, location);
		}
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}

	// Get DYNAMIC map locations.
	try
	{
		auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetMapLocations"));

		while(result->next())
		{
			MapLocation location;
			location.id = result->getUInt("id");
			auto name = result->getString("name").asStdString();
			location.name = std::wstring(name.begin(), name.end());
			location.x = (float)result->getDouble("x");
			location.y = (float)result->getDouble("y");
			location.type_displayAsCategory = result->getUInt("category_main");
			location.type_displayAsSubcategory = result->getUInt("category_sub");

			uint32_t scene_id = result->getUInt("scene_id") + 1;
			InsertLocation(scene_id, location);
		}
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}

	// Register message handler.
	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
	connection_service->RegisterMessageHandler(&MapService::HandleRequestMapLocationsMessage, this);

}

void MapService::InsertLocation(uint32_t scene_id, MapLocation& location)
{
	auto i = locations_.find(scene_id);
	if(i == locations_.end())
	{
		locations_.insert(std::make_pair(scene_id, std::list<MapLocation>()));
	}

	locations_.at(scene_id).push_back(location);
}

void MapService::HandleRequestMapLocationsMessage(
	const std::shared_ptr<ConnectionClientInterface>& client, 
	swganh::messages::GetMapLocationsRequestMessage* message)
{
	swganh::messages::GetMapLocationsResponseMessage response;
	response.planet_name = message->planet_name;
	response.locations = locations_.find(simulation_->SceneIdByName(message->planet_name))->second;
	client->GetController()->Notify(&response);
}
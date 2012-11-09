// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/travel/travel_service.h>
#include <swganh_core/travel/purchase_ticket_command.h>
#include <swganh/service/service_manager.h>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

#include <swganh_core/messages/enter_ticket_purchase_mode_message.h>
#include <swganh_core/messages/planet_travel_point_list_request.h>
#include <swganh_core/messages/planet_travel_point_list_response.h>
#include <swganh_core/object/object.h>
#include <swganh_core/simulation/simulation_service.h>
#include <swganh_core/connection/connection_service.h>
#include <swganh_core/command/command_service.h>

#include <swganh_core/messages/out_of_band.h>
#include <swganh_core/messages/system_message.h>

using namespace swganh::travel;
using namespace swganh::object;
using namespace swganh::service;
using namespace swganh::simulation;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::command;

TravelService::TravelService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
}

TravelService::~TravelService(void)
{
}

void TravelService::Startup()
{
	simulation_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
	command_ = kernel_->GetServiceManager()->GetService<CommandService>("CommandService");

	LoadStaticTravelPoints();

	// Register message handler.
	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
	connection_service->RegisterMessageHandler(&TravelService::HandlePlanetTravelPointListRequest, this);

	command_->AddCommandCreator("purchaseticket", [=](swganh::app::SwganhKernel* kernel, const CommandProperties& properties)
	{
		return std::make_shared<PurchaseTicketCommand>(kernel, properties);
	});
}

swganh::service::ServiceDescription TravelService::GetServiceDescription()
{
	return swganh::service::ServiceDescription(
		"TravelService", // namve
		"travel", // type
		"0.1", // version
		"127.0.0.1", // address
		0, // tcp port
		0, // udp port
		0); // status
}

void TravelService::BeginTicketTransaction(std::shared_ptr<Object> object)
{
	if(!object->HasController())
		return;

	swganh::messages::EnterTicketPurchaseModeMessage enter_ticket;
	enter_ticket.planet_name = simulation_->SceneNameById(object->GetSceneId());
	enter_ticket.city_name = "Coronet Starport";
	object->GetController()->Notify(&enter_ticket);
}

void TravelService::LoadStaticTravelPoints()
{
	try
	{
		auto conn = kernel_->GetDatabaseManager()->getConnection("swganh_static");
		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetTravelPoints();"));

		while(result->next())
		{
			TravelPoint travel_point;
			travel_point.descriptor = result->getString("descriptor");
			travel_point.port_type = result->getUInt("port_type");
			travel_point.position = glm::vec3(result->getDouble("x"), result->getDouble("z"), result->getDouble("y"));
			travel_point.taxes = result->getUInt("taxes");
			travel_point.scene_id = result->getUInt("scene_id");
			travel_point.spawn_position = glm::vec3(result->getDouble("spawn_x"), result->getDouble("spawn_z"), result->getDouble("spawn_y"));
			travel_points_.push_back(travel_point);
		}

		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());
			while(result->next())
			{
				TravelPoint travel_point;
				travel_point.descriptor = result->getString("descriptor");
				travel_point.port_type = result->getUInt("port_type");
				travel_point.position = glm::vec3(result->getDouble("x"), result->getDouble("z"), result->getDouble("y"));
				travel_point.taxes = result->getUInt("taxes");
				travel_point.scene_id = result->getUInt("scene_id");
				travel_point.spawn_position = glm::vec3(result->getDouble("spawn_x"), result->getDouble("spawn_z"), result->getDouble("spawn_y"));
				travel_points_.push_back(travel_point);
			}
		}
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
}

void TravelService::PurchaseTicket(std::shared_ptr<swganh::object::Object> object,
	std::string source_scene,
	std::string source_location,
	std::string target_scene,
	std::string target_location,
	uint32_t price,
	uint32_t taxes, 
	bool round_trip)
{
	for(auto& location : travel_points_)
	{
		auto& location_name = location.descriptor;
		std::replace(location_name.begin(), location_name.end(), ' ', '_');

		if(location_name.compare(target_location) == 0)
		{
			simulation_->TransferObjectToScene(object, target_scene, location.spawn_position.x, location.spawn_position.z, location.spawn_position.y);
		}
	}
}

void TravelService::HandlePlanetTravelPointListRequest(
	const std::shared_ptr<ConnectionClientInterface>& client, 
	swganh::messages::PlanetTravelPointListRequest* message)
{
	auto planet_travel_points = std::map<std::string, std::vector<TravelPoint>>();
	auto travel_terminal = simulation_->GetObjectById(message->travel_terminal_id);

	for(auto& location : travel_points_)
	{
		if(location.scene_id == 43)
			continue;

		if((location.scene_id != travel_terminal->GetSceneId()) && location.port_type != 1)
			continue;

		auto iter = planet_travel_points.find(simulation_->SceneNameById(location.scene_id));
		if(iter == planet_travel_points.end())
		{
			planet_travel_points.insert(std::pair<std::string, std::vector<TravelPoint>>(simulation_->SceneNameById(location.scene_id), std::vector<TravelPoint>()));
			iter = planet_travel_points.find(simulation_->SceneNameById(location.scene_id));
		}

		iter->second.push_back(location);
	}

	for(auto& travel_point : planet_travel_points)
	{
		swganh::messages::PlanetTravelPointListResponse response;

		response.planet_name = travel_point.first;

		for(auto& loc : travel_point.second)
		{
			response.location_names.push_back(loc.descriptor);
			response.location_coordinates.push_back(loc.position);
			response.location_tax_rates.push_back(loc.taxes);
			response.location_starport_flags.push_back(loc.port_type);
		}

		client->GetController()->Notify(&response);
	}
}
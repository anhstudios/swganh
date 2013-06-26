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
#include <swganh_core/object/creature/creature.h>
#include <swganh_core/equipment/equipment_service.h>
#include <swganh_core/simulation/simulation_service.h>
#include <swganh_core/connection/connection_service.h>
#include <swganh_core/command/command_service.h>
#include <swganh_core/sui/sui_service.h>

#include <swganh_core/messages/out_of_band.h>
#include <swganh_core/messages/system_message.h>

using namespace swganh::travel;
using namespace swganh::object;
using namespace swganh::service;
using namespace swganh::simulation;
using namespace swganh::connection;
using namespace swganh::messages;
using namespace swganh::command;
using namespace swganh::equipment;

TravelService::TravelService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
    SetServiceDescription(swganh::service::ServiceDescription(
		"TravelService", // namve
		"travel", // type
		"0.1", // version
		"127.0.0.1", // address
		0, // tcp port
		0, // udp port
		0));
}

TravelService::~TravelService(void)
{}

void TravelService::Initialize()
{
	simulation_ = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
	command_ = kernel_->GetServiceManager()->GetService<CommandServiceInterface>("CommandService");
	equipment_ = kernel_->GetServiceManager()->GetService<EquipmentServiceInterface>("EquipmentService");
}

void TravelService::Startup()
{
	kernel_->GetDatabaseManager()->ExecuteAsync(&TravelService::LoadStaticTravelPoints, this, "swganh_static").get();
	kernel_->GetDatabaseManager()->ExecuteAsync(&TravelService::LoadPlanetaryRouteMap, this, "swganh_static").get();

	// Register message handler.
	auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
	connection_service->RegisterMessageHandler(&TravelService::HandlePlanetTravelPointListRequest, this);

	command_->AddCommandCreator<PurchaseTicketCommand>("purchaseticket");
}

void TravelService::BeginTicketTransaction(std::shared_ptr<Object> object)
{
	if(!object->HasController())
		return;

	// Find nearest ticket terminal.
	auto terminals = simulation_->FindObjectsInRangeByTag(object, "travel_terminal", 16.0f);
	if(terminals.size() < 1)
		return;

	std::cout << (*terminals.begin()).first << std::endl;
	std::shared_ptr<Object> nterminal = (*terminals.begin()).second;

	swganh::messages::EnterTicketPurchaseModeMessage enter_ticket;
	enter_ticket.planet_name = simulation_->SceneNameById(object->GetSceneId());
	auto descriptor = nterminal->GetAttributeAsString("location_descriptor");
	enter_ticket.city_name = std::string(descriptor.begin(), descriptor.end());
	object->GetController()->Notify(&enter_ticket);
}

void TravelService::LoadPlanetaryRouteMap(const std::shared_ptr<sql::Connection>& connection)
{
	try
	{
		auto statement = std::shared_ptr<sql::Statement>(connection->createStatement());
		auto result = std::shared_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetPlanetaryTravelRoutes();"));

		while(result->next())
		{
			PlanetaryTravelRoute route;
			route.departure_planet_id = result->getInt("srcId");
			route.arrival_planet_id = result->getInt("destId");
			route.price = result->getInt("price");
			planetary_travel_routes_.push_back(route);
		}
	}
	catch(sql::SQLException &e) {
		LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
		LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
	}
}

void TravelService::LoadStaticTravelPoints(const std::shared_ptr<sql::Connection>& connection)
{
	try
	{
		auto statement = std::shared_ptr<sql::Statement>(connection->createStatement());
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
	TravelPoint source_location_tp;
	TravelPoint target_location_tp;
	bool source_location_found = false;
	bool target_location_found = false;

	// Find Source Location
	for(auto& location : travel_points_)
	{
		auto location_name = location.descriptor;
		std::replace(location_name.begin(), location_name.end(), ' ', '_');

		if(location_name.compare(source_location) == 0) {
			source_location_tp = location;
			source_location_found = true;
		}
	}

	// Find Target Location
	for(auto& location : travel_points_)
	{
		auto location_name = location.descriptor;
		std::replace(location_name.begin(), location_name.end(), ' ', '_');

		if(location_name.compare(target_location) == 0) {
			target_location_tp = location;
			target_location_found = true;
		}
	}

	// Verify Source and Target locations exist.
	if(!source_location_found || !target_location_found)
	{
		SystemMessage::Send(object, swganh::messages::OutOfBand("travel", "no_location_found"), false, false);
		return;
	}

	// Verify Planetary Route and make sure route is online.
	PlanetaryTravelRoute* planetary_travel_route = nullptr;
	for(auto& route : planetary_travel_routes_)
	{
		if((route.departure_planet_id == source_location_tp.scene_id) && (route.arrival_planet_id == target_location_tp.scene_id))
		{
			planetary_travel_route = &route;
		}
	}

	// We didn't find a route, but we where supposted to.
	if(planetary_travel_route == nullptr)
	{
		SystemMessage::Send(object, swganh::messages::OutOfBand("travel", "route_not_available"), false, false);
		return;
	}

	// Verify our destination is online.
	if(simulation_->SceneExists(planetary_travel_route->arrival_planet_id) == false)
	{
		SystemMessage::Send(object, swganh::messages::OutOfBand("travel", "route_not_available"), false, false);
		return;
	}

	// Duduct Credits + Taxes
	//price += planetary_travel_route->price;
	//price += target_location_tp.taxes;

	auto creature = std::static_pointer_cast<swganh::object::Creature>(object);
	if(creature->GetCashCredits() < price)
	{
		auto sui = kernel_->GetServiceManager()->GetService<swganh::sui::SUIService>("SuiService");
		auto window = sui->CreateMessageBox(swganh::sui::MESSAGE_BOX_OK, L"The Galactic Travel Commision", L"You do not have enough money to complete the ticket purchase.", object);
		sui->OpenSUIWindow(window);
		return;
	}

	creature->SetCashCredits(creature->GetCashCredits() - price);
	SystemMessage::Send(object, swganh::messages::OutOfBand("base_player", "prose_pay_acct_success", "", "", "", "", "money/acct_n", "travelsystem", price), false, false);

	// Create Ticket(s)
	auto source_planet = simulation_->SceneNameById(source_location_tp.scene_id);
	auto target_planet = simulation_->SceneNameById(target_location_tp.scene_id);
	auto inventory = simulation_->GetEquipmentService()->GetEquippedObject(object, "inventory");

	auto ticket = simulation_->CreateObjectFromTemplate("object/tangible/travel/travel_ticket/base/shared_base_travel_ticket.iff", swganh::object::DEFAULT_PERMISSION, false);
	ticket->SetAttribute("travel_departure_planet", std::wstring(source_planet.begin(), source_planet.end()));
	ticket->SetAttribute("travel_departure_point", std::wstring(source_location_tp.descriptor.begin(), source_location_tp.descriptor.end()));
	ticket->SetAttribute("travel_arrival_planet", std::wstring(target_planet.begin(), target_planet.end()));
	ticket->SetAttribute("travel_arrival_point", std::wstring(target_location_tp.descriptor.begin(), target_location_tp.descriptor.end()));
	ticket->SetAttribute("radial_filename", L"radials/ticket.py");
	inventory->AddObject(object, ticket);

	if(round_trip)
	{
		auto ticket = simulation_->CreateObjectFromTemplate("object/tangible/travel/travel_ticket/base/shared_base_travel_ticket.iff", swganh::object::DEFAULT_PERMISSION, false);
		ticket->SetAttribute("travel_departure_planet", std::wstring(target_planet.begin(), target_planet.end()));
		ticket->SetAttribute("travel_departure_point", std::wstring(target_location_tp.descriptor.begin(), target_location_tp.descriptor.end()));
		ticket->SetAttribute("travel_arrival_planet", std::wstring(source_planet.begin(), source_planet.end()));
		ticket->SetAttribute("travel_arrival_point", std::wstring(source_location_tp.descriptor.begin(), source_location_tp.descriptor.end()));
		ticket->SetAttribute("radial_filename", L"radials/ticket.py");
		inventory->AddObject(object, ticket);
	}

	auto sui = kernel_->GetServiceManager()->GetService<swganh::sui::SUIService>("SuiService");
	auto window = sui->CreateMessageBox(swganh::sui::MESSAGE_BOX_OK, L"The Galactic Travel Commision", L"@travel:ticket_purchase_complete", object);
	sui->OpenSUIWindow(window);
}

void TravelService::UseTicket(std::shared_ptr<Object> requester, std::shared_ptr<Object> ticket)
{
	// Verify ticket properties.
	if(ticket->HasAttribute("travel_departure_planet") == false || 
		ticket->HasAttribute("travel_departure_point") == false || 
		ticket->HasAttribute("travel_arrival_planet") == false ||
		ticket->HasAttribute("travel_arrival_point") == false)
	{
		return;
	}

	// Find departure and destination TravelPoints.
	TravelPoint* departure;
	TravelPoint* arrival;
	std::wstring departure_planetws = ticket->GetAttributeAsString("travel_departure_planet");
	std::wstring departure_pointws = ticket->GetAttributeAsString("travel_departure_point");
	std::wstring arrival_planetws = ticket->GetAttributeAsString("travel_arrival_planet");
	std::wstring arrival_pointws = ticket->GetAttributeAsString("travel_arrival_point");

	std::string departure_planet(departure_planetws.begin(), departure_planetws.end());
	std::string departure_point(departure_pointws.begin(), departure_pointws.end());
	std::string arrival_planet(arrival_planetws.begin(), arrival_planetws.end());
	std::string arrival_point(arrival_pointws.begin(), arrival_pointws.end());

	uint32_t departure_planet_id = simulation_->SceneIdByName(departure_planet);
	uint32_t arrival_planet_id = simulation_->SceneIdByName(arrival_planet);

	// Departure
	for(auto& location : travel_points_)
	{
		if(location.scene_id == departure_planet_id && location.descriptor.compare(departure_point) == 0)
		{
			departure = &location;
			break;
		}
	}

	// Arrival
	for(auto& location : travel_points_)
	{
		if(location.scene_id == arrival_planet_id && location.descriptor.compare(arrival_point) == 0)
		{
			arrival = &location;
			break;
		}
	}

	// Ticket Route Valid?
	if(!departure || !arrival)
	{
		SystemMessage::Send(requester, swganh::messages::OutOfBand("travel", "route_not_available"), false, false);
		return;
	}

	// Find nearest shuttle.
	auto ticket_collectors = simulation_->FindObjectsInRangeByTag(requester, "ticket_collector", 16.0f);
	auto shuttles = simulation_->FindObjectsInRangeByTag(requester, "shuttle", 25.0f); 

	if(ticket_collectors.size() < 1 || shuttles.size() < 1)
	{
		SystemMessage::Send(requester, swganh::messages::OutOfBand("travel", "boarding_too_far"), false, false);
		return;
	}

	auto ticket_collector = (*ticket_collectors.begin()).second;
	auto shuttle = (*shuttles.begin()).second;
	

	// Verify we are at the correct departure point and shuttle is ready to lift off.
	auto descriptor = ticket_collector->GetAttributeAsString("travel_point");
	if(departure->descriptor.compare(std::string(descriptor.begin(), descriptor.end())) != 0)
	{
		SystemMessage::Send(requester, swganh::messages::OutOfBand("travel", "boarding_too_far"), false, false);
		return;
	}

	auto shuttle_creature = std::static_pointer_cast<Creature>(shuttle);
	if(shuttle_creature->GetPosture() != 0)
	{
		SystemMessage::Send(requester, swganh::messages::OutOfBand("travel", "shuttle_not_available"), false, false);
		return;
	}

	// Verify our destination is online.
	if(simulation_->SceneExists(arrival->scene_id) == false)
	{
		SystemMessage::Send(requester, swganh::messages::OutOfBand("travel", "route_not_available"), false, false);
		return;
	}

	ticket->GetContainer()->RemoveObject(requester, ticket);
	simulation_->RemoveObject(ticket);

	// Transport
	simulation_->TransferObjectToScene(requester, arrival_planet, arrival->spawn_position.x, arrival->spawn_position.z, arrival->spawn_position.y);
}

void TravelService::HandlePlanetTravelPointListRequest(
	const std::shared_ptr<ConnectionClientInterface>& client, 
	swganh::messages::PlanetTravelPointListRequest* message)
{
	auto planet_travel_points = std::map<std::string, std::vector<TravelPoint>>();
	auto travel_terminal = simulation_->GetObjectById(message->travel_terminal_id);

	for(auto& location : travel_points_)
	{
		// Scene is offline, restrict selection.
		if(simulation_->SceneExists(location.scene_id) == false)
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

std::vector<std::string> TravelService::GetAvailableTickets(std::shared_ptr<swganh::object::Object> object)
{
	std::vector<std::string> in;
	auto inventory = simulation_->GetEquipmentService()->GetEquippedObject(object, "inventory");

	// Find nearest ticket collector.
	auto ticket_collectors = simulation_->FindObjectsInRangeByTag(object, "ticket_collector", 16.0f);

	if(inventory == nullptr || ticket_collectors.size() < 1)
		return in;

	auto ticket_collector = (*ticket_collectors.begin()).second;

	if(ticket_collector == nullptr)
		return in;

	auto descriptor = ticket_collector->GetAttributeAsString("travel_point");
	inventory->ViewObjects(object, 0, true, [=, &in](std::shared_ptr<swganh::object::Object> obj){
		if(obj->GetTemplate() == "object/tangible/travel/travel_ticket/base/shared_base_travel_ticket.iff")
		{
			std::wstring depart = obj->GetAttribute<std::wstring>("travel_departure_point");
			if(depart.compare(descriptor) == 0) {
				std::stringstream ss;
				std::wstring arrival = obj->GetAttribute<std::wstring>("travel_arrival_point");
				ss << std::string(depart.begin(), depart.end()) << " -- " << std::string(arrival.begin(), arrival.end());
				in.push_back(ss.str());
			}
		}
	});
	return in;
}

std::shared_ptr<swganh::object::Object> TravelService::GetAvailableInventoryTicketBySelection(std::shared_ptr<swganh::object::Object> object, uint32_t selection)
{
	std::shared_ptr<swganh::object::Object> out = nullptr;
	auto inventory = simulation_->GetEquipmentService()->GetEquippedObject(object, "inventory");

	// Find nearest ticket collector.
	auto ticket_collectors = simulation_->FindObjectsInRangeByTag(object, "ticket_collector", 16.0f);

	if(inventory == nullptr || ticket_collectors.size() < 1)
		return out;

	auto ticket_collector = (*ticket_collectors.begin()).second;

	if(ticket_collector == nullptr)
		return out;

	auto descriptor = ticket_collector->GetAttributeAsString("travel_point");
	uint32_t cindex = 0;
	std::cout << "Ticket Collector Travel Point Descriptor: " << std::string(descriptor.begin(), descriptor.end()) << std::endl;
	inventory->ViewObjects(object, 0, true, [=, &out, &cindex](std::shared_ptr<swganh::object::Object> obj){
		if(obj->GetTemplate() == "object/tangible/travel/travel_ticket/base/shared_base_travel_ticket.iff")
		{
			std::wstring depart = obj->GetAttribute<std::wstring>("travel_departure_point");
			if(depart.compare(descriptor) == 0) {
				std::cout << "Added Ticket: " << std::string(depart.begin(), depart.end()) << std::endl;
				if(cindex == selection) out = obj;
				cindex++;
			}
		}
	});

	return out;
}


std::shared_ptr<swganh::object::Object> TravelService::GetInventoryTicket(std::shared_ptr<swganh::object::Object> object, uint32_t index)
{
	auto inventory = simulation_->GetEquipmentService()->GetEquippedObject(object, "inventory");
	std::shared_ptr<swganh::object::Object> ticket = nullptr;

	if(inventory == nullptr)
		return ticket;

	uint32_t cindex = 0;
	inventory->ViewObjects(object, 0, true, [=, &cindex, &ticket](std::shared_ptr<swganh::object::Object> obj){
		if(index == cindex) {
			ticket = obj;
		}
		cindex++;
	});

	return ticket;

}
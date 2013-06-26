// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh_core/travel/travel_service_interface.h>

#include <glm/glm.hpp>
#include "swganh/database/database_manager.h"
#include <swganh/app/swganh_kernel.h>
#include <swganh_core/connection/connection_client_interface.h>


namespace swganh {
namespace command {
	class CommandServiceInterface;
} // namespace swganh::command

namespace connection {
	class ConnectionClientInterface;
} // namespace swganh::connection 

namespace simulation {
	class SimulationServiceInterface;
} // namespace swganh::simulation

namespace messages {
	struct PlanetTravelPointListRequest;
} // namespace swganh::messages

namespace equipment {
	class EquipmentServiceInterface;
}

namespace travel {

	struct TravelPoint
	{
		uint32_t taxes;
		uint8_t port_type;
		std::string descriptor;
		uint32_t scene_id;
		glm::vec3 position;
		glm::vec3 spawn_position;
	};

	struct PlanetaryTravelRoute
	{
		uint32_t departure_planet_id;
		uint32_t arrival_planet_id;
		uint32_t price;
	};

	class TravelService : public TravelServiceInterface
	{
	public:
		TravelService(swganh::app::SwganhKernel* kernel);
		~TravelService();

        virtual void Initialize();
		virtual void Startup();

		virtual void BeginTicketTransaction(std::shared_ptr<swganh::object::Object> object);
		virtual void PurchaseTicket(std::shared_ptr<swganh::object::Object> object,
			std::string source_scene,
			std::string source_location,
			std::string target_scene,
			std::string target_location,
			uint32_t price,
			uint32_t taxes, 
			bool round_trip = false);

		virtual void UseTicket(std::shared_ptr<swganh::object::Object> object, std::shared_ptr<swganh::object::Object> ticket);
		virtual std::vector<std::string> GetAvailableTickets(std::shared_ptr<swganh::object::Object> object);
		virtual std::shared_ptr<swganh::object::Object> GetAvailableInventoryTicketBySelection(std::shared_ptr<swganh::object::Object> object, uint32_t selection);
		virtual std::shared_ptr<swganh::object::Object> GetInventoryTicket(std::shared_ptr<swganh::object::Object> object, uint32_t index);
	private:

		void HandlePlanetTravelPointListRequest(const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, swganh::messages::PlanetTravelPointListRequest* message);

		void LoadStaticTravelPoints(const std::shared_ptr<sql::Connection>& connection);
		void LoadDynamicTravelPoints(const std::shared_ptr<sql::Connection>& connection);
		void LoadPlanetaryRouteMap(const std::shared_ptr<sql::Connection>& connection);

		std::vector<TravelPoint> travel_points_;
		std::vector<PlanetaryTravelRoute> planetary_travel_routes_;

		swganh::app::SwganhKernel* kernel_;
		swganh::simulation::SimulationServiceInterface* simulation_;
		swganh::command::CommandServiceInterface* command_;
		swganh::equipment::EquipmentServiceInterface* equipment_;
	};
}} // namespace swganh::travel
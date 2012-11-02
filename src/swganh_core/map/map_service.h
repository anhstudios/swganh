// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/map/map_service_interface.h>

#include <swganh_core/messages/get_map_locations_response_message.h>
#include <swganh_core/messages/get_map_locations_request_message.h>

namespace swganh {
namespace app {
	class SwganhKernel;
} // namespace swganh::app

namespace connection {
	class ConnectionClientInterface;
} // namespace swganh::connection

namespace object {
	class Object;
} // namespace swganh::object

namespace simulation {
	class SimulationService;
} // namespace swganh::simulation

namespace map {

	class MapService : public MapServiceInterface
	{
	public:
		MapService(swganh::app::SwganhKernel* kernel);
		~MapService();

		void Startup();

		swganh::service::ServiceDescription GetServiceDescription();

	private:
		void InsertLocation(uint32_t scene_id, swganh::messages::MapLocation& location);

		void HandleRequestMapLocationsMessage(
			const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
			swganh::messages::GetMapLocationsRequestMessage* message);

		swganh::app::SwganhKernel* kernel_;
		swganh::simulation::SimulationService* simulation_;
		std::map<uint32_t, std::list<swganh::messages::MapLocation>> locations_;
	};

}} // swganh::map
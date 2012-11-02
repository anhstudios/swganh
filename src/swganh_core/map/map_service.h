// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/map/map_service_interface.h>

#include <swganh_core/messages/get_map_locations_response_message.h>

namespace swganh {
namespace app {
	class SwganhKernel;
} // namespace swganh::app

namespace map {

	struct MapPoint
	{
	};

	class MapService : public MapServiceInterface
	{
	public:
		MapService(swganh::app::SwganhKernel* kernel);
		~MapService();

		void Startup();

		swganh::service::ServiceDescription GetServiceDescription();

	private:
		swganh::app::SwganhKernel* kernel_;
		std::list<swganh::messages::MapLocation> locations_;
	};

}} // swganh::map
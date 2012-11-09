// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/travel/travel_service_interface.h>

#include <swganh/app/swganh_kernel.h>
#include <swganh/connection/connection_client_interface.h>

namespace swganh {
namespace simulation {
	class SimulationService;
} // namespace swganh::simulation

namespace travel {
	class TravelService : public TravelServiceInterface
	{
	public:
		TravelService(swganh::app::SwganhKernel* kernel);
		~TravelService();

		void Startup();

		swganh::service::ServiceDescription GetServiceDescription();

		virtual void BeginTicketTransaction(std::shared_ptr<swganh::object::Object> object);

	private:
		swganh::app::SwganhKernel* kernel_;
		swganh::simulation::SimulationService* simulation_;
	};
}} // namespace swganh::travel
#pragma once

#include "swganh/app/swganh_kernel.h"
#include "swganh/static/static_service_interface.h"

namespace sql
{
	class ResultSet;
}

namespace swganh
{
class EventInterface;

namespace simulation
{
	class SimulationServiceInterface;
}

namespace statics
{
	class StaticService : public swganh::statics::StaticServiceInterface
	{
	public:

		StaticService(swganh::app::SwganhKernel* kernel);
		~StaticService();

		virtual swganh::service::ServiceDescription GetServiceDescription();

	private:

		void _loadBuildings(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadCells(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadCloneLocations(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadTerminals(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadElevatorData(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadContainers(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadTicketCollectors(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadNPCS(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);
		void _loadShuttles(swganh::simulation::SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
			uint32_t scene_id, std::string scene_name);

		swganh::app::SwganhKernel* kernel_;
	};
}
}
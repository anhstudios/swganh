#pragma once

#include <map>
#include <vector>

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

namespace spawn
{
	class SpawnServiceInterface;
}

namespace statics
{
	class StaticService : public swganh::statics::StaticServiceInterface
	{
	public:

		StaticService(swganh::app::SwganhKernel* kernel);
		~StaticService();

		virtual swganh::service::ServiceDescription GetServiceDescription();

		std::vector<std::shared_ptr<ElevatorData>> GetElevatorDataForObject(uint64_t terminal_id);

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
		void _loadNPCS(swganh::simulation::SimulationServiceInterface* simulation_service, swganh::spawn::SpawnServiceInterface* spawn_service,
			std::unique_ptr<sql::ResultSet> result, uint32_t scene_id, std::string scene_name);
		void _loadShuttles(swganh::simulation::SimulationServiceInterface* simulation_service, swganh::spawn::SpawnServiceInterface* spawn_service,
			std::unique_ptr<sql::ResultSet> result, uint32_t scene_id, std::string scene_name);

		swganh::app::SwganhKernel* kernel_;

		std::map<uint64_t, std::vector<std::shared_ptr<ElevatorData>>> elevator_lookup_;
	};
}
}
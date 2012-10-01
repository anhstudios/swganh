#pragma once

#include "swganh/app/swganh_kernel.h"
#include "swganh/static/static_service_interface.h"

namespace sql
{
	class ResultSet;
}

namespace swganh
{
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

		void _loadBuildings(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);

		void _loadCells(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);
		
		void _loadCloneLocations(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);

		void _loadTerminals(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);
		
		void _loadElevatorData(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);
		
		void _loadTicketCollectors(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);
		
		void _loadNPCS(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);
		
		void _loadShuttles(std::shared_ptr<swganh::simulation::SimulationServiceInterface> simulation_service, 
			std::unique_ptr<sql::ResultSet>& result);



		swganh::app::SwganhKernel* kernel_;
	};
}
}
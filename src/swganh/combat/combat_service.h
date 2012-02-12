
#ifndef SWGANH_COMBAT_COMBAT_SERVICE_H_
#define SWGANH_COMBAT_COMBAT_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/asio/deadline_timer.hpp>

#include <tbb/concurrent_unordered_map.h>

#include "swganh/base/base_service.h"
#include "swganh/command/command_properties.h"

namespace swganh {
namespace simulation {
class SimulationService;
}
namespace command {
class CommandService; 
}
namespace object {
    class Object;
    class ObjectController;
namespace creature {
	class Creature;
}}}  // namespace swganh::object::creature;


namespace swganh {
namespace combat {

	typedef std::function<void (
		const std::shared_ptr<swganh::object::creature::Creature>&, // creature object
		const std::shared_ptr<swganh::object::Object>&,	// target object
        std::wstring command_options)
    > CombatHandler;

    class CombatService: public swganh::base::BaseService
    {
    public:
        explicit CombatService(anh::app::KernelInterface* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();
		void RegisterCombatHandler(uint32_t command_crc, CombatHandler&& handler);
       
    private:
		typedef tbb::concurrent_unordered_map<
            uint32_t, 
            CombatHandler
        > HandlerMap;

		std::shared_ptr<swganh::simulation::SimulationService> simulation_service_;
		std::shared_ptr<swganh::command::CommandService> command_service_;
        void LoadProperties();

		void onStart();

        HandlerMap	handlers_;

		swganh::command::CommandPropertiesMap combat_properties_map_;
    };

}}  // namespace swganh::combat

#endif  // SWGANH_COMBAT_COMBAT_SERVICE_H_


#ifndef SWGANH_COMBAT_COMBAT_SERVICE_H_
#define SWGANH_COMBAT_COMBAT_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/asio/deadline_timer.hpp>
#include <boost/python.hpp>

#include <tbb/concurrent_unordered_map.h>

#include "swganh/base/base_service.h"
#include "swganh/command/command_properties.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

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
        const swganh::messages::controllers::CommandQueueEnqueue&)
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

        void ProcessCombatCommand(
			const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::Object>& target,
			const swganh::messages::controllers::CommandQueueEnqueue& command_queue_message);

        void RegisterCombatScript(const swganh::command::CommandProperties& properties);

        bool InitiateCombat(const std::shared_ptr<swganh::object::creature::Creature>& attacker,const std::shared_ptr<swganh::object::Object>& target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);
        void GetCombatData(boost::python::object python_object);
        void SendCombatAction(const std::shared_ptr<swganh::object::creature::Creature>& attacker,const std::shared_ptr<swganh::object::Object>& target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);
        bool SingleTargetCombatAction(const std::shared_ptr<swganh::object::creature::Creature>& attacker,const std::shared_ptr<swganh::object::Object>& target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);

        uint16_t GetPostureModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker);
        uint16_t GetTargetPostureModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& target);
        uint16_t GetAccuracyModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker);
        uint16_t GetAccuracyBonus(const std::shared_ptr<swganh::object::creature::Creature>& attacker);
        void ApplyStates(const std::shared_ptr<swganh::object::creature::Creature>& attacker);

		std::shared_ptr<swganh::simulation::SimulationService> simulation_service_;
		std::shared_ptr<swganh::command::CommandService> command_service_;
        void LoadProperties(swganh::command::CommandPropertiesMap command_properties);

		void onStart();

        HandlerMap	combat_handlers_;

		swganh::command::CommandPropertiesMap combat_properties_map_;
    };

}}  // namespace swganh::combat

#endif  // SWGANH_COMBAT_COMBAT_SERVICE_H_

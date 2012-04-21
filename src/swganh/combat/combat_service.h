
#ifndef SWGANH_COMBAT_COMBAT_SERVICE_H_
#define SWGANH_COMBAT_COMBAT_SERVICE_H_

#ifndef WIN32
#include <Python.h>
#endif

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/asio/deadline_timer.hpp>
#include <boost/python.hpp>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <tbb/concurrent_unordered_map.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
}

#endif

#include "anh/delayed_task_processor.h"
#include "anh/random_generator.h"

#include "swganh/base/base_service.h"
#include "swganh/command/command_service.h"
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
namespace tangible {
    class Tangible;
}
namespace creature {
	class Creature;
}}}  // namespace swganh::object::creature;

namespace swganh {
namespace combat {

    enum HIT_TYPE {
        HIT = 0,
        BLOCK,
        DODGE,
        COUNTER,
        MISS
    };
    struct CombatCommand;

    class CombatService: public swganh::base::BaseService
    {
    public:
        explicit CombatService(swganh::app::SwganhKernel* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();
		void RegisterCommandHandler(uint32_t command_crc, command::CommandHandler&& handler);

        void SetIncapacitated(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& target);
        
        void SetDead(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& target);

        void EndDuel(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& target);
        void EndCombat(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& target);
        
    private:
		typedef Concurrency::concurrent_unordered_map<
            uint32_t, 
            command::CommandHandler
        > HandlerMap;

        void RegisterCombatScript(const swganh::command::CommandMap& properties);

        bool InitiateCombat(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::tangible::Tangible> & target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);
        void SendCombatAction(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::tangible::Tangible> & target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);
        void SendCombatActionMessage(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::tangible::Tangible> & target, CombatCommand& combat_command, std::string animation = std::string(""));
        int SingleTargetCombatAction(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::tangible::Tangible> & target, CombatCommand& combat_command);
        int SingleTargetCombatAction(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature> & target, CombatCommand& combat_command);

        uint16_t GetPostureModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker);
        uint16_t GetTargetPostureModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& target);
        uint16_t GetAccuracyModifier(const std::shared_ptr<swganh::object::creature::Creature>& attacker);
        uint16_t GetAccuracyBonus(const std::shared_ptr<swganh::object::creature::Creature>& attacker);
        float GetHitChance(float attacker_accuracy, float attacker_bonus, float target_defence);
        uint16_t GetHitResult(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature> & target, int damage, int accuracy_bonus);
        void ApplyStates(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& defender, CombatCommand& combat_command);
        int ApplyDamage(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::creature::Creature>& defender, CombatCommand& combat_command, int damage, int pool);
        int ApplyDamage(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::tangible::Tangible>& target, CombatCommand& combat_command, int damage, int pool);
        int GetDamagingPool(CombatCommand& properties);
        // Message Helpers
        void BroadcastCombatSpam(const std::shared_ptr<swganh::object::creature::Creature>& attacker, const std::shared_ptr<swganh::object::tangible::Tangible>& target, const CombatCommand& combat_command, uint32_t damage, const std::string& string_file);

        swganh::simulation::SimulationService* simulation_service_;
		swganh::command::CommandService* command_service_;
        void LoadProperties(swganh::command::CommandMap command_properties);
		void onStart();

        HandlerMap	combat_handlers_;

		swganh::command::CommandMap combat_properties_map_;

        anh::RandomGenerator generator_;

        std::unique_ptr<anh::SimpleDelayedTaskProcessor> delayed_task_;
    };

}}  // namespace swganh::combat

#endif  // SWGANH_COMBAT_COMBAT_SERVICE_H_

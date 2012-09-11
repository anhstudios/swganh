// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

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

#include "swganh/combat/combat_service_interface.h"

#include "swganh/random_generator.h"
#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/command/command_properties.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"


namespace swganh {
namespace combat {
    
    struct CombatData;
	typedef std::function<boost::python::object (
        swganh::app::SwganhKernel*,
		const std::shared_ptr<swganh::object::Object>&, // creature object
		const std::shared_ptr<swganh::object::Tangible>&,	// target object
        const swganh::messages::controllers::CommandQueueEnqueue&)
    > CombatHandler;

	enum HIT_TYPE {
        HIT = 0,
        BLOCK,
        DODGE,
        COUNTER,
        MISS
    };

    class CombatService: public swganh::combat::CombatServiceInterface
    {
    public:
        explicit CombatService(swganh::app::SwganhKernel* kernel);
        
        swganh::service::ServiceDescription GetServiceDescription();

        void SetIncapacitated(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        
        void SetDead(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);

        void EndDuel(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        void EndCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        
		void Startup();

        void SendCombatAction(swganh::command::BaseCombatCommand* command);

    private:
		typedef Concurrency::concurrent_unordered_map<
            uint32_t, 
            CombatHandler
        > HandlerMap;
        
        bool InitiateCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const swganh::HashString& command);
        
        bool InitiateCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);
        void SendCombatAction(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const swganh::messages::controllers::CommandQueueEnqueue& command_message, boost::python::object p_object);
        void SendCombatActionMessage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, CombatData& properties, std::string animation = std::string(""));
        int SingleTargetCombatAction(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, CombatData& properties);
        int SingleTargetCombatAction(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature> & target, CombatData& properties);

        uint16_t GetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker);
        uint16_t GetTargetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        uint16_t GetAccuracyModifier(const std::shared_ptr<swganh::object::Tangible>& attacker);
        uint16_t GetAccuracyBonus(const std::shared_ptr<swganh::object::Tangible>& attacker);
        float GetHitChance(float attacker_accuracy, float attacker_bonus, float target_defence);
        uint16_t GetHitResult(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature> & target, int damage, int accuracy_bonus);
        void ApplyStates(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& defender, CombatData& properties);
        int ApplyDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& defender, CombatData& properties, int damage, int pool);
        int ApplyDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& target, CombatData& properties, int damage, int pool);
        int GetDamagingPool(CombatData& properties);
        // Message Helpers
        void BroadcastCombatSpam(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& target, const CombatData& properties, uint32_t damage, const std::string& string_file);

        swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::command::CommandServiceInterface* command_service_;

        HandlerMap	combat_handlers_;

		swganh::command::CommandPropertiesMap combat_properties_map_;

        swganh::RandomGenerator generator_;

        swganh::ActiveObject active_;
        swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::combat

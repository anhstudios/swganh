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
namespace object {
	class Weapon;
}
namespace equipment {
	class EquipmentServiceInterface;
}
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
        MISS,
		DEFLECT
    };

	enum HIT_LOCATION {
		HEALTH = 0,
		ACTION = 2,
		MIND = 4,
		SPREAD = 8
	};

	/**
	* The service used for combat
	*/
    class CombatService: public swganh::combat::CombatServiceInterface
    {
    public:
        
		/**
		* Creates a new instance
		*/
		explicit CombatService(swganh::app::SwganhKernel* kernel);
        
		/**
		* @return the service description of this service
		*/
        swganh::service::ServiceDescription GetServiceDescription();

		/**
		* Attacker incapacitates target
		* @param attacker the object that caused the incapacitation
		* @param target the object that has become incapacitated
		*/
        void SetIncapacitated(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        
		/**
		* Attacker kills target
		* @param attacker the object that caused the kill
		* @param target the object that has been killed
		*/
        void SetDead(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);

		/**
		* Ends a duel
		* @param attacker the object that has ended the duel
		* @param target the object that has had it's duel ended
		*/
        void EndDuel(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        
		/**
		* Ends combat
		* @param attacker the object that has ended combat
		* @param target the object that has had combat ended
		*/
		void EndCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        
		/**
		* Called on startup.
		*/
		void Startup();

		/**
		* Executes a combat action
		* @param command the command of the combat action to execute
		*/
        void SendCombatAction(swganh::command::BaseCombatCommand* command);

    private:
		typedef Concurrency::concurrent_unordered_map<uint32_t, CombatHandler> HandlerMap;
      
        bool InitiateCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const swganh::HashString& command);
		bool InitiateCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const swganh::messages::controllers::CommandQueueEnqueue& command_message);
		
		std::vector<std::shared_ptr<swganh::object::Tangible>> GetCombatTargets(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, std::shared_ptr<CombatData> combat_data);
		
		void DoCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, std::shared_ptr<CombatData> combat_data);
		// Performs the intial combat damage calculation before reduction and mitigation stages
		// Damage calculated to be done to the target using a particular weapon and/or ability. 
		// This stage calculates all weapon, buff, and ability modifiers to calculate the aggregate damage
		float CalculateDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, std::shared_ptr<CombatData> combat_data);
		//
        void SendCombatActionMessage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, std::shared_ptr<CombatData> combat_data, std::string animation = std::string(""));
        
        uint16_t GetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker);
        uint16_t GetTargetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        uint16_t GetAccuracyBonus(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Weapon>& weapon);
        
		/// HIT
		float GetHitChance(int attacker_accuracy, int attacker_bonus, int target_defence);
        HIT_TYPE GetHitResult(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature> & target, std::shared_ptr<CombatData> combat_data);
		HIT_LOCATION GetHitLocation(std::shared_ptr<CombatData> combat_data);
		float GetWeaponRangeModifier(std::shared_ptr<swganh::object::Weapon>& weapon, float range_to_target);

		///
        void ApplyStates(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& defender, std::shared_ptr<CombatData> combat_data);
        int ApplyDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& defender, std::shared_ptr<CombatData> combat_data, int damage, HIT_LOCATION pool);
        int ApplyDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& target, std::shared_ptr<CombatData> combat_data, int damage, HIT_LOCATION pool);
        // Message Helpers
        void BroadcastCombatSpam(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& target, const std::shared_ptr<CombatData> combat_data, uint32_t damage, const std::string& string_file);

        swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::command::CommandServiceInterface* command_service_;

        HandlerMap	combat_handlers_;

		swganh::command::CommandPropertiesMap combat_data_map_;

        swganh::RandomGenerator generator_;

        swganh::ActiveObject active_;
        swganh::app::SwganhKernel* kernel_;			
		std::shared_ptr<swganh::equipment::EquipmentServiceInterface> equipment_service_;
    };

}}  // namespace swganh::combat

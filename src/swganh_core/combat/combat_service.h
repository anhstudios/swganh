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
#include <map>

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

#include "swganh_core/combat/combat_service_interface.h"
#include "swganh_core/combat/buff_manager.h"

#include "swganh/random_generator.h"
#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/command/command_properties.h"
#include "swganh_core/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace statics {
	class StaticServiceInterface;	
}
namespace object {
	class Weapon;
	class Player;
}
namespace equipment {
	class EquipmentServiceInterface;
}
namespace messages {
namespace controllers {
	struct CombatDefender;
}}
namespace combat {
    struct CombatData;
    typedef std::function<boost::python::object (
        swganh::app::SwganhKernel*,
		const std::shared_ptr<swganh::object::Object>&, // creature object
		const std::shared_ptr<swganh::object::Tangible>&,	// target object
        const swganh::messages::controllers::CommandQueueEnqueue&)
    > CombatHandler;

	enum HIT_TYPE {
		MISS = 0x1,
        HIT = 0x2,
        BLOCK = 0x3,
        DODGE = 0x5,
        COUNTER = 0x7,
        RICHOCHET = 0x8,
		REFLECT = 0x9,
    };

	enum HIT_LOCATION {
		HEALTH = 0,
		ACTION = 2,
		MIND = 4,
		SPREAD = 8
	};

	enum CombatSpecialMoveEffect {
		NONE = 0,
		TARGET = 0x1,
		BLINDED = 0x2,
		DAMAGE = 0x3,
		DIZZIED = 0x4,
		DUCK = 0x5,
		INTIMIDATED = 0x6,
		KNOCKDOWN = 0x7,
		STARTLE = 0x8,
		STARTLED = 0x9,
		STUNNED = 0xA,
		SUPRISED = 0xB,
		TARGET_BODY = 0xC,
		TARGET_HEAD = 0xD,
		TARGET_LEG = 0xE
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
        
        ~CombatService();

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
        
		void Initialize();
		void Startup();

		/**
		* Executes a combat action
		* @param command the command of the combat action to execute
		*/
        void SendCombatAction(swganh::command::BaseCombatCommand* command);

    private:
		typedef Concurrency::concurrent_unordered_map<uint32_t, CombatHandler> HandlerMap;
      
        bool InitiateCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const std::shared_ptr<swganh::object::Weapon>& weapon, const std::shared_ptr<CombatData> combat_data);
		
		std::vector<std::shared_ptr<swganh::object::Tangible>> GetCombatTargets(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data);
		
		swganh::messages::controllers::CombatDefender DoCombat(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data);
		// Performs the intial combat damage calculation before reduction and mitigation stages
		// Damage calculated to be done to the target using a particular weapon and/or ability. 
		// This stage calculates all weapon, buff, and ability modifiers to calculate the aggregate damage
		float CalculateDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, std::shared_ptr<CombatData> combat_data);
		//
        void SendCombatActionMessage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible> & target, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data, std::vector<swganh::messages::controllers::CombatDefender> defenders);
        
        uint16_t GetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker);
        uint16_t GetTargetPostureModifier(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& target);
        uint16_t GetAccuracyBonus(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data);
        
		/// HIT
		float GetHitChance(int attacker_accuracy, int attacker_bonus, int target_defence);
        HIT_TYPE GetHitResult(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature> & target, const std::shared_ptr<swganh::object::Weapon>& weapon,std::shared_ptr<CombatData> combat_data);
		HIT_LOCATION GetHitLocation(std::shared_ptr<CombatData> combat_data);
		float GetWeaponRangeModifier(const std::shared_ptr<swganh::object::Weapon>& weapon, float range_to_target);

		///
        CombatSpecialMoveEffect ApplyStates(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& defender, std::shared_ptr<CombatData> combat_data);
        int ApplyDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Creature>& defender, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data, int damage, HIT_LOCATION pool);
        int ApplyDamage(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& target, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data, int damage, HIT_LOCATION pool);

		bool ApplySpecialCost(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Weapon>& weapon, std::shared_ptr<CombatData> combat_data);
        // Message Helpers
        void BroadcastCombatSpam(const std::shared_ptr<swganh::object::Creature>& attacker, const std::shared_ptr<swganh::object::Tangible>& target, const std::shared_ptr<swganh::object::Weapon>& weapon, const std::shared_ptr<CombatData> combat_data, uint32_t damage, const std::string& string_file);

        swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::command::CommandServiceInterface* command_service_;

        HandlerMap	combat_handlers_;

		swganh::command::CommandPropertiesMap combat_data_map_;

        swganh::RandomGenerator generator_;

		swganh::combat::BuffManager buff_manager_;

        swganh::ActiveObject active_;
        swganh::app::SwganhKernel* kernel_;			
		std::shared_ptr<swganh::equipment::EquipmentServiceInterface> equipment_service_;
		swganh::statics::StaticServiceInterface* static_service_;
    };

}}  // namespace swganh::combat

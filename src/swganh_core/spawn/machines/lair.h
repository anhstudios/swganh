#pragma once

#include "swganh_core/spawn/fsm_manager.h"
#include "swganh_core/spawn/finite_state_machine.h"
#include "swganh_core/spawn/fsm_state.h"
#include "swganh_core/spawn/bundles/lair_bundle.h"
#include "swganh_core/object/creature/creature.h"

using namespace swganh::spawn;
using namespace swganh::object;

void _buildLairMachine(swganh::app::SwganhKernel* kernel, FsmManager& manager_)
{
	auto lair_latent = std::make_shared<FsmState>();
	auto initial_spawned = std::make_shared<FsmState>();
	auto spawn_delay = std::make_shared<FsmState>();
	auto in_combat = std::make_shared<FsmState>();
	auto dead = std::make_shared<FsmState>();

	lair_latent->AddTimedTransition(boost::posix_time::seconds(0), initial_spawned,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//Spawn some initial babies
		return true;
	});

	initial_spawned->AddEventTransition(swganh::messages::DeltasMessage::opcode, spawn_delay,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//If all our children are dead
			//Return true
		//Else
			//Return false

		return true;
	});

	//Enter Combat
	initial_spawned->AddEventTransition(swganh::messages::DeltasMessage::opcode, in_combat,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//Tell our babies we need some help

		return true;
	});

	spawn_delay->AddTimedTransition(boost::posix_time::seconds(30), in_combat,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//Spawn some creeps nearby based on our settings
		//If we're still in combat 
			//return true
		//Else 
			//return false

		return true;
	});

	spawn_delay->AddTimedTransition(boost::posix_time::seconds(30), initial_spawned,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//Spawn some creeps nearby based on our settings
		//If we're still in combat 
			//return false
		//Else 
			//return true

		return true;
	});

	//Our hp is sufficiently low
	in_combat->AddEventTransition(swganh::messages::DeltasMessage::opcode, in_combat,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//If we're still alive
			//Spawn some creeps
			//return true 
		//else 
			//return false

		return true;
	});

	//Our hp reaches 0
	in_combat->AddEventTransition(swganh::messages::DeltasMessage::opcode, dead,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//If we're not still alive
			//return true
		//else
			//return false

		return true;
	});

}
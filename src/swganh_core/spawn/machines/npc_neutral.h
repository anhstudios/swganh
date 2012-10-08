#pragma once

#include "swganh_core/spawn/fsm_manager.h"
#include "swganh_core/spawn/finite_state_machine.h"
#include "swganh_core/spawn/fsm_state.h"
#include "swganh_core/spawn/bundles/npc_neutral_bundle.h"
#include "swganh_core/object/creature/creature.h"

#include "swganh_core/messages/scene_create_object_by_crc.h"
#include "swganh_core/messages/scene_destroy_object.h"

using namespace swganh::spawn;
using namespace swganh::object;

void _buildNpcNeutralMachine(swganh::app::SwganhKernel* kernel, FsmManager& manager_)
{
	auto frozen = std::make_shared<FsmState>();
	auto amble_neutral = std::make_shared<FsmState>();

	swganh::messages::SceneCreateObjectByCrc tmp;
	frozen->AddEventTransition(swganh::messages::SceneCreateObjectByCrc::opcode, amble_neutral,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//If the thing being added is a player return true;
		//Else return false;
		return false;
	});

	amble_neutral->AddEventTransition(swganh::messages::SceneDestroyObject::opcode, frozen,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//Check our aware objects list for a player
		//If we find one return false
		//If we don't return true
		return false;
	});

	amble_neutral->AddTimedTransition(boost::posix_time::seconds(30), amble_neutral,
	[] (FsmBundleInterface* bundle, std::shared_ptr<swganh::object::Object> object) -> bool {
		//Pick a random nearby spot within a small distance and move there
		return true;
	});

	manager_.RegisterAutomaton(L"npc_neutral", std::make_shared<FiniteStateMachine>(kernel, 1, frozen,
	[] (std::shared_ptr<FsmStateInterface> initial_state) -> std::shared_ptr<FsmBundleInterface>
	{
		return std::make_shared<NpcNeutralBundle>(initial_state);
	}));
}


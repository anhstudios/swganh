#pragma once

#include "swganh_core/spawn/fsm_manager.h"
#include "swganh_core/spawn/finite_state_machine.h"
#include "swganh_core/spawn/fsm_state.h"
#include "swganh_core/spawn/controllers/default_controller.h"
#include "swganh_core/object/creature/creature.h"

using namespace swganh::spawn;
using namespace swganh::object;

void _buildShuttleMachine(swganh::app::SwganhKernel* kernel, FsmManager& manager_, uint32_t SHUTTLE_AWAY_TIME_SECONDS, uint32_t SHUTTLE_IN_PORT_TIME_SECONDS)
{
	std::shared_ptr<FsmState> in_port_state = std::make_shared<FsmState>();
	std::shared_ptr<FsmState> away_state = std::make_shared<FsmState>();
	std::shared_ptr<FsmState> takeoff_state = std::make_shared<FsmState>();
	std::shared_ptr<FsmState> landing_state = std::make_shared<FsmState>();

	in_port_state->AddTimedTransition(boost::posix_time::seconds(SHUTTLE_IN_PORT_TIME_SECONDS), takeoff_state, 
	[] (FsmController* controller, std::shared_ptr<swganh::object::Object> object) -> bool {
			auto shuttle = std::static_pointer_cast<Creature>(object);
			if(shuttle)
			{
				//Set the posture to away. Thank SOE for this hack.
				shuttle->SetPosture((Posture)2);
			}
			return true;
	});

	away_state->AddTimedTransition(boost::posix_time::seconds(SHUTTLE_AWAY_TIME_SECONDS), landing_state,
	[] (FsmController* controller, std::shared_ptr<swganh::object::Object> object) -> bool {
			auto shuttle = std::static_pointer_cast<Creature>(object);
			if(shuttle)
			{
				//Set the posture to landed. Thank SOE for this hack.
				shuttle->SetPosture((Posture)0);
			}
			return true;
	});

	takeoff_state->AddTimedTransition(boost::posix_time::seconds(20), away_state,
	[] (FsmController* controller, std::shared_ptr<swganh::object::Object> object) -> bool {return true;});

	landing_state->AddTimedTransition(boost::posix_time::seconds(20), in_port_state,
	[] (FsmController* controller, std::shared_ptr<swganh::object::Object> object) -> bool {return true;});

	manager_.RegisterAutomaton(L"shuttle", std::make_shared<FiniteStateMachine>(kernel, in_port_state,
	[] (FiniteStateMachineInterface* machine, std::shared_ptr<Object> obj, std::shared_ptr<FsmStateInterface> initial_state) 
	{
		return std::make_shared<DefaultController>(machine, obj, initial_state);
	}));
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/thread/mutex.hpp>

#include <queue>
#include <tuple>
#include <swganh_core/spawn/fsm_controller.h>

namespace swganh
{
namespace spawn
{

class DefaultController : public FsmController
{
public:
	DefaultController(FiniteStateMachineInterface* parent, std::shared_ptr<swganh::object::Object> object, 
							 std::shared_ptr<FsmStateInterface> initial_state)
		: FsmController(parent, object, initial_state)
	{}

	void Notify(swganh::messages::BaseSwgMessage* message) {}
	void Notify(swganh::messages::BaseSwgMessage* message, swganh::network::Session::SequencedCallback &&) {}
};
}
}
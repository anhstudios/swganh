// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/spawn/fsm_state_interface.h"

namespace swganh
{
namespace spawn
{

	class FsmState : public swganh::spawn::FsmStateInterface
	{
	public:
		void AddEventTransition(EventType type, std::shared_ptr<FsmStateInterface> end_state, TransitionHandler handler)
		{
			Transition trans_;
			trans_.handler_ = handler;
			trans_.end_state_ = end_state;
			event_transitions_.insert(std::make_pair(type, trans_));
		}

		void AddTimedTransition(boost::posix_time::time_duration offset_secs, std::shared_ptr<FsmStateInterface> end_state, 
			TransitionHandler handler)
		{
			Transition trans_;
			trans_.handler_ = handler;
			trans_.end_state_ = end_state;
			timed_transitions_.insert(std::make_pair(offset_secs, trans_));
		}

		std::map<EventType, Transition>& GetEventTransitions() { return event_transitions_; }
		std::multimap<boost::posix_time::time_duration, Transition>& GetTimedTransitions() { return timed_transitions_; }
		
	private:
		std::map<EventType, Transition> event_transitions_;
		std::multimap<boost::posix_time::time_duration, Transition> timed_transitions_;
	};
}
}
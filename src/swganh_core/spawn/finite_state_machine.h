#pragma once

#include <map>
#include <swganh/event_dispatcher.h>

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
	typedef std::function<void(std::shared_ptr<EventInterface>,std::shared_ptr<swganh::object::Object>)> TransitionHandler;

	class FsmStateInterface;
	
	struct StateTransition
	{
		std::shared_ptr<FsmStateInterface> start_state_;
		TransitionHandler handler_;
		std::shared_ptr<FsmStateInterface> end_state_;
	};

	class FiniteStateMachine
	{
	public:
		FiniteStateMachine(swganh::EventDispatcher* dispatch);
		
		void StartManagingObject(std::shared_ptr<swganh::object::Object> object);
		
		void StopManagingObject(std::shared_ptr<swganh::object::Object> object);
		
		void AddTransition(std::shared_ptr<FsmStateInterface> start_state, 
			EventType event_type, TransitionHandler handler, std::shared_ptr<FsmStateInterface> end_state);
	
	private:
		void HandleEvent_(const std::shared_ptr<EventInterface>& event);
	
		swganh::EventDispatcher* dispatch_;
		std::shared_ptr<FsmStateInterface> initial_state_;
	
		std::multi_map<EventType, StateTransition> transitionLookup_;
		std::multi_map<std::shared_ptr<FsmStateInterface>, std::shared_ptr<swganh::object::Object>> managed_objects_;
	};
}
}
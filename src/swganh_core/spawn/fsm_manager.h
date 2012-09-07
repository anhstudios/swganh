#pragma once

#include <map>
#include <swganh/event_dispatcher.h>

#include "finite_state_machine.h"

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
	class FiniteStateMachine;
	class FsmManager
	{
	public:
		
		FsmManager(EventDispatcher* event_dispatch);
	
		void RegisterAutomaton(const std::string& name, std::shared_ptr<FiniteStateMachine> automaton);
		void GetAutomatonByName(const std::string& name);
		void DeregisterAutomaton(const std::string& name);
		
		void StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::string machine);
		void StopManagingObject(std::shared_ptr<swganh::object::Object> object);
		
	private:
		EventDispatcher* dispatch_;
		
		std::map<std::string, std::shared_ptr<FiniteStateMachine>> machines_;
	};
}
}
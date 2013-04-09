// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <vector>
#include <map>
#include <swganh/event_dispatcher.h>
#include <boost/thread/mutex.hpp>

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
	class FiniteStateMachineInterface;
	class FsmManager
	{
	public:
		
		FsmManager(EventDispatcher* event_dispatch);
	
		void RegisterAutomaton(const std::wstring& name, std::shared_ptr<FiniteStateMachineInterface> automaton);
		std::shared_ptr<FiniteStateMachineInterface> GetAutomatonByName(const std::wstring& name);
		void DeregisterAutomaton(const std::wstring& name);
		
		void StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::wstring machine);
		void StopManagingObject(std::shared_ptr<swganh::object::Object> object);

	private:
		boost::shared_mutex mutex_;
		
		std::map<std::wstring, std::shared_ptr<FiniteStateMachineInterface>> machines_;
	};
}
}
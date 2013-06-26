#pragma once

#include <vector>
#include <memory>
#include <map>
#include <swganh/event_dispatcher.h>
#include "glm/glm.hpp"

#define PATH_STEP_CONSTANT 0.25f

namespace swganh
{
namespace messages
{
	struct BaseSwgMessage;
}

namespace object 
{
	class Object;
}
}

namespace swganh
{
namespace spawn
{
	class FsmStateInterface;
	class FsmController;

	/**
	* @brief A simple function which is used to determine success of a transition. This allows extra logic to be added
	*    to a single transition. 
	* @param FsmBundleInterface* the bundle of the object that this is transitioning on. This is provided
	*    for convienience.
	* @param Object the object that this is transitioning on. 
	*/
	typedef std::function<bool(FsmController*, std::shared_ptr<swganh::object::Object>)> TransitionHandler;

	class FsmStateInterface;
	
	struct Transition
	{
		TransitionHandler handler_;
		std::shared_ptr<FsmStateInterface> end_state_;
	};

	class FsmStateInterface
	{
	public:
		/*
		static bool WithinLineOfSight(std::shared_ptr<swganh::object::Object> object, glm::vec3& point2);
		static std::vector<glm::vec3> FindPath(std::shared_ptr<swganh::object::Object> object, glm::vec3& destination);
		*/

		virtual ~FsmStateInterface() {}
		
		virtual void AddEventTransition(swganh::EventType type, std::shared_ptr<FsmStateInterface> end_state, TransitionHandler handler) = 0;
		virtual void AddTimedTransition(boost::posix_time::time_duration offset_secs, std::shared_ptr<FsmStateInterface> end_state, TransitionHandler handler) = 0;
		virtual std::map<swganh::EventType, Transition>& GetEventTransitions() = 0;
		virtual std::multimap<boost::posix_time::time_duration, Transition>& GetTimedTransitions() = 0;
	};
}
}
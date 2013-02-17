#pragma once

#include <boost/thread/mutex.hpp>

#include <queue>
#include <tuple>
#include <swganh_core/spawn/fsm_bundle_interface.h>

namespace swganh
{
namespace spawn
{
typedef std::tuple<boost::posix_time::ptime, boost::posix_time::time_duration, Transition> TimedType;

class NpcFactionedBundle : public FsmBundleInterface
{
public:
	struct timeOrder
	{
		bool operator()(const TimedType& lhs, const TimedType& rhs) const
		{
			return std::get<0>(lhs) < std::get<0>(rhs);
		}
	};

	NpcFactionedBundle(std::shared_ptr<FsmStateInterface> initial_state);

	/**
	* @return true when this object requires post-processing
	*/
	virtual void HandleNotify(std::shared_ptr<swganh::object::Object>& object_, swganh::messages::BaseSwgMessage* message);
	
	/*
	* @return true when this object still requires processing
	*/
	virtual void HandleCleanup(std::shared_ptr<swganh::object::Object>& object_, boost::posix_time::ptime current_time_);

	virtual bool IsDirty();

	void SetCurrentState(std::shared_ptr<FsmStateInterface> new_state, boost::posix_time::ptime current_time);

private:
	std::priority_queue<TimedType, std::vector<TimedType>, timeOrder> timed;
};
}
}
#include "shuttle_bundle.h"

#include "boost/date_time/posix_time/posix_time_types.hpp"

using namespace swganh::object;
using namespace swganh::spawn;

ShuttleBundle::ShuttleBundle(std::shared_ptr<FsmStateInterface> initial_state)
{
	SetCurrentState(initial_state, boost::posix_time::second_clock::local_time());
}

void ShuttleBundle::HandleNotify(std::shared_ptr<Object>& object_, swganh::messages::BaseSwgMessage* message) 
{
}
	
bool ShuttleBundle::IsDirty()
{
	return !timed.empty();
}

void ShuttleBundle::SetCurrentState(std::shared_ptr<FsmStateInterface> new_state, boost::posix_time::ptime current_time)
{
	current_state_ = new_state;
	
	//Clear Timed Queue
	std::priority_queue<TimedType, std::vector<TimedType>, timeOrder> clear_queue;
	timed = std::move(clear_queue);

	//Fill it in with the new timed stuff
	auto itr = current_state_->GetTimedTransitions().begin();
	auto end = current_state_->GetTimedTransitions().end();
	for(; itr != end; ++itr)
	{
		timed.push(TimedType(current_time+itr->first, itr->first, itr->second));
	}
}

void ShuttleBundle::HandleCleanup(std::shared_ptr<Object>& object_, boost::posix_time::ptime current_time_)
{
	if(!timed.empty())
	{
		boost::posix_time::ptime time = std::get<0>(timed.top());
		while(time <= current_time_)
		{
			TimedType type = timed.top();

			//Call the handler
			Transition& t = std::get<2>(type);
			if(t.handler_(this, object_))
			{
				SetCurrentState(t.end_state_, current_time_);
				return;
			}

			//Pop type off
			timed.pop();

			//Advance the time for type
			std::get<0>(type) = time + std::get<1>(type);
		
			//Reinsert type
			timed.push(type);
		}
	}
}
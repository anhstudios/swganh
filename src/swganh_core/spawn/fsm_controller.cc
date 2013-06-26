// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "fsm_controller.h"

#include "swganh_core/object/object.h"

using namespace swganh::spawn;
using namespace swganh::messages;

FsmController::FsmController(FiniteStateMachineInterface* parent, std::shared_ptr<swganh::object::Object> object,
	std::shared_ptr<FsmStateInterface> initial_state)
	: parent_machine_(parent)
	, object_(object) 
{
	SetCurrentState(initial_state, boost::posix_time::second_clock::local_time());
}
	
FsmController::~FsmController()
{
	ClearMessages();
}

uint64_t FsmController::GetId() const
{
	return object_->GetObjectId();
}

void FsmController::Cleanup(boost::posix_time::ptime current_time_)
{
	//Timer information is only changed by the AI thread. We don't need to lock.
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

			//Update loop variable
			time = std::get<0>(timed.top());
		}
	}

	//Messages can take place on an arbitrary thread. We need to lock
	std::lock_guard<std::mutex> lock_(message_mutex_);
	if(!messages.empty())
	{
		auto transitions = current_state_->GetEventTransitions();

		BaseSwgMessage* front = nullptr;
		std::map<EventType, Transition>::iterator itr;
		size_t remaining = 0;
		do
		{
			//We want to atomically remove the front
			{
				std::lock_guard<std::mutex> lock_(message_mutex_);
				front = messages.front();
				itr = transitions.find(front->Opcode());
				delete front;
				messages.pop();
			}
			
			//Now handle the transition on the AI thread without blocking
			//the message queue.
			if(itr != transitions.end())
			{
				if(itr->second.handler_(this, object_)) 
				{
					SetCurrentState(itr->second.end_state_, current_time_);
				}
			}
			
			//Now update our remaining size
			{
				std::lock_guard<std::mutex> lock_(message_mutex_);
				remaining = messages.size();
			}
			
		} while(remaining > 0);
	}
}

bool FsmController::IsDirty()
{
	std::lock_guard<std::mutex> lock_(message_mutex_);
	return !timed.empty() || !messages.empty();
}

swganh::messages::BaseSwgMessage* FsmController::CurrentMessage()
{
	std::lock_guard<std::mutex> lock_(message_mutex_);
	return messages.front();
}

void FsmController::ClearMessages()
{
	std::queue<swganh::messages::BaseSwgMessage*> empty_messages;
	std::lock_guard<std::mutex> lock_(message_mutex_);
	while(!messages.empty())
	{
		delete messages.front();
		messages.pop();
	}
}

void FsmController::EnqueueMessage(swganh::messages::BaseSwgMessage* copyOfMessage)
{
	std::lock_guard<std::mutex> lock_(message_mutex_);
	messages.push(copyOfMessage);
}

void FsmController::SetCurrentState(std::shared_ptr<FsmStateInterface> new_state, boost::posix_time::ptime current_time)
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
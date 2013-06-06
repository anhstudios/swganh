// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/observer/observer_interface.h>

#include <swganh_core/spawn/fsm_state_interface.h>

#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <queue>
#include <tuple>

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{

class FiniteStateMachineInterface;

/**
* @brief Ties relevant time information together.
*/
typedef std::tuple<boost::posix_time::ptime, boost::posix_time::time_duration, Transition> TimedType;

/**
* @brief Allows sorting of the TimedType tuples above. This is used by the priority_queue
*/
struct timeOrder
{
	bool operator()(const TimedType& lhs, const TimedType& rhs) const
	{
		return std::get<0>(lhs) < std::get<0>(rhs);
	}
};

/**
* @brief This controller is used when an AI state machine is controlling an object
*/
class FsmController : public std::enable_shared_from_this<FsmController>, public swganh::observer::ObserverInterface
{
public:

	FsmController(FiniteStateMachineInterface* parent, std::shared_ptr<swganh::object::Object> object,
					std::shared_ptr<FsmStateInterface> initial_state);
	
	virtual ~FsmController();

	virtual uint64_t GetId() const;

	/**
	* @brief Called when a message is sent to the controlled object. 
	* Implementations are expected to call EnqueueMessage with copies of the given message if that message is
	* important to this machine in one of the states.
	*/
    virtual void Notify(swganh::messages::BaseSwgMessage* message) = 0;
	virtual void Notify(swganh::messages::BaseSwgMessage* message,
		swganh::network::Session::SequencedCallback &&) = 0;

	/**
	* @brief Called by a controller when this object is marked as dirty and a tick is scheduled
	*/
	virtual void Cleanup(boost::posix_time::ptime current_time_);

	/**
	* @brief Returns true if this object needs some attention by the state machine's thread
	*/
	virtual bool IsDirty();
	
	/**
	* @brief Returns the message currently being handled for the bundle. This is used by
	* Transition handlers.
	*/
	virtual swganh::messages::BaseSwgMessage* CurrentMessage();

	virtual void ClearMessages();

protected:
	std::mutex message_mutex_;

	FiniteStateMachineInterface* parent_machine_;
	std::shared_ptr<swganh::object::Object> object_;

	std::shared_ptr<FsmStateInterface> current_state_;
	std::priority_queue<TimedType, std::vector<TimedType>, timeOrder> timed;
	std::queue<swganh::messages::BaseSwgMessage*> messages;

	virtual void EnqueueMessage(swganh::messages::BaseSwgMessage* copyOfMessage);
	virtual void SetCurrentState(std::shared_ptr<FsmStateInterface> new_state, boost::posix_time::ptime current_time);
};
}
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/observer/observer_interface.h>
#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
class FiniteStateMachine;
class FsmBundleInterface;
class FsmController : public swganh::observer::ObserverInterface
{
public:

	FsmController(FiniteStateMachine* parent, std::shared_ptr<swganh::object::Object> object, std::shared_ptr<FsmBundleInterface> bundle);

	virtual uint64_t GetId() const;

    virtual void Notify(swganh::messages::BaseSwgMessage* message);
	virtual void Notify(swganh::messages::BaseSwgMessage* message, swganh::network::soe::Session::SequencedCallback&& callback) { };

	virtual void Cleanup(boost::posix_time::ptime current_time_);

	virtual bool IsDirty();

private:
	FiniteStateMachine* parent_machine_;
	std::shared_ptr<FsmBundleInterface> bundle_;
	std::shared_ptr<swganh::object::Object> object_;
};
}
}
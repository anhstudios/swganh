// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "fsm_state_interface.h"

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

namespace spawn
{

class FsmStateInterface;
class FsmBundleInterface
{
public:
	/**
	* @return true when this object requires post-processing
	*/
	virtual void HandleNotify(std::shared_ptr<swganh::object::Object>& object_, swganh::messages::BaseSwgMessage* message) = 0;
	
	/*
	* @return true when this object still requires processing
	*/
	virtual void HandleCleanup(std::shared_ptr<swganh::object::Object>& object_, boost::posix_time::ptime current_time_) = 0;	

	virtual bool IsDirty() = 0;

protected:
	std::shared_ptr<FsmStateInterface> current_state_;
};
}
}
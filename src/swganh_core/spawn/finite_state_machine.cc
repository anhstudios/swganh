// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "finite_state_machine.h"
#include "swganh_core/spawn/fsm_controller.h"
#include "swganh_core/object/object.h"

#include "swganh/app/swganh_kernel.h"

using namespace boost;
using namespace swganh::spawn;

FiniteStateMachine::FiniteStateMachine(swganh::app::SwganhKernel* kernel, std::shared_ptr<FsmStateInterface> initial_state,
	ControllerFactory controller_factory)
	: controller_factory_(controller_factory)
    , initial_state_(initial_state)
    , machine_cleaner_(kernel->GetCpuThreadPool())
    , kernel_(kernel)
{
	machine_cleaner_.expires_from_now(boost::posix_time::milliseconds(20));
	machine_cleaner_.async_wait(bind(&FiniteStateMachine::HandleDispatch, this, boost::asio::placeholders::error));
}

void FiniteStateMachine::HandleDispatch(const boost::system::error_code& error)
{
	if(!error)
	{
		std::set<std::shared_ptr<FsmController>> to_process_;
		{
			lock_guard<mutex> lock(mutex_);
			to_process_.swap(dirty_controllers_);
		}

		//Setup timer for next batch
		machine_cleaner_.expires_from_now(boost::posix_time::milliseconds(20));
		machine_cleaner_.async_wait(bind(&FiniteStateMachine::HandleDispatch, this, boost::asio::placeholders::error));
		
		for(auto& c : to_process_)
		{
			c->Cleanup(boost::posix_time::second_clock::local_time());
			if(c->IsDirty()) 
			{
				MarkDirty(c);
			}
		}
	}
}

FiniteStateMachine::~FiniteStateMachine()
{
}

void FiniteStateMachine::StartManagingObject(std::shared_ptr<swganh::object::Object> object)
{
	auto controller = controller_factory_(this, object, initial_state_);
	{
		lock_guard<mutex> lock(mutex_);
		controllers_.insert(controller);
		object->SetController(controller);

		if(controller->IsDirty())
		{
			dirty_controllers_.insert(controller);
		}
	}
}

void FiniteStateMachine::StopManagingObject(std::shared_ptr<swganh::object::Object> object)
{
	auto controller = std::static_pointer_cast<FsmController>(object->GetController());
	if(controller)
	{
		lock_guard<mutex> lock(mutex_);
		controllers_.erase(controller);
		dirty_controllers_.erase(controller);
		object->SetController(nullptr);
	}
}

void FiniteStateMachine::MarkDirty(std::shared_ptr<FsmController> controller_)
{
	lock_guard<mutex> lock(mutex_);
	dirty_controllers_.insert(controller_);
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "finite_state_machine.h"
#include "swganh_core/spawn/fsm_controller.h"
#include "swganh_core/object/object.h"

#include <boost/thread.hpp>


using namespace boost;
using namespace swganh::spawn;

FiniteStateMachine::FiniteStateMachine(swganh::app::SwganhKernel* kernel, uint32_t threads_required, std::shared_ptr<FsmStateInterface> initial_state,
			BundleGenerator bundle_factory)
	: kernel_(kernel)
	, initial_state_(initial_state)
	, bundle_factory_(bundle_factory)
	, shutdown_(false)
{
	while(threads_required > 0)
	{
		threads_.push_back(std::move(thread([this] () {
			while(!shutdown_)
			{
				std::set<std::shared_ptr<FsmController>> to_process_;
				{
					lock_guard<mutex> lock(mutex_);
					to_process_.swap(dirty_controllers_);
				}

				for(auto& c : to_process_)
				{
					c->Cleanup(boost::posix_time::second_clock::local_time());
					if(c->IsDirty()) 
					{
						MarkDirty(c);
					}
				}

				this_thread::yield();
			}
		})));

		
		--threads_required;
	}
}

FiniteStateMachine::~FiniteStateMachine()
{
	shutdown_ = true;
	for_each(threads_.begin(), threads_.end(), std::mem_fn(&boost::thread::join));
}

void FiniteStateMachine::StartManagingObject(std::shared_ptr<swganh::object::Object> object)
{
	auto controller = std::make_shared<FsmController>(this, object, bundle_factory_(initial_state_));
	{
		lock_guard<mutex> lock(mutex_);
		controllers_.insert(controller);
		object->SetController(controller);

		if(controller->IsDirty())
			dirty_controllers_.insert(controller);
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
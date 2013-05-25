// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/spawn/finite_state_machine_interface.h"

#include <atomic>
#include <map>
#include <set>
#include <thread>
#include <boost/thread/mutex.hpp>

#include <boost/asio.hpp>

namespace boost
{
namespace asio
{
class io_service;
}

class thread;
}

namespace swganh
{
namespace app
{
	class SwganhKernel;
}

namespace object
{
	class Object;
}

namespace spawn
{
	class FsmBundleInterface;
	class FsmStateInterface;
	class FsmController;

	typedef std::function<std::shared_ptr<FsmController>(FiniteStateMachineInterface*, std::shared_ptr<swganh::object::Object>, 
						std::shared_ptr<FsmStateInterface>)> ControllerFactory;

	class FiniteStateMachine : public FiniteStateMachineInterface
	{
	public:
	
		FiniteStateMachine(swganh::app::SwganhKernel* kernel_, 
			std::shared_ptr<FsmStateInterface> initial_state,
			ControllerFactory controller_factory);

		~FiniteStateMachine();

		void StartManagingObject(std::shared_ptr<swganh::object::Object> object);
		void StopManagingObject(std::shared_ptr<swganh::object::Object> object);

		void MarkDirty(std::shared_ptr<FsmController> controller_);

		swganh::app::SwganhKernel* GetKernel() { return kernel_; }

	private:

		void HandleDispatch(const boost::system::error_code& error);
		
		boost::mutex mutex_;

		std::set<std::shared_ptr<FsmController>> controllers_, dirty_controllers_;

		ControllerFactory controller_factory_;
		std::shared_ptr<FsmStateInterface> initial_state_;

		boost::asio::deadline_timer machine_cleaner_;
		swganh::app::SwganhKernel* kernel_;
	};
}
}
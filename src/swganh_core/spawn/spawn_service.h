// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/asio/deadline_timer.hpp>

#include "swganh_core/spawn/spawn_service_interface.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/permissions/permission_type.h"
#include "swganh_core/spawn/fsm_manager.h"

namespace swganh
{
namespace spawn
{
	class SpawnService : public swganh::spawn::SpawnServiceInterface
	{
	public:
		SpawnService(swganh::app::SwganhKernel* kernel);
		~SpawnService();
		
		 swganh::service::ServiceDescription GetServiceDescription();
	
		 virtual void StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::wstring machine)
		 {
			 fsm_manager_.StartManagingObject(object, machine);
		 }

		virtual void StopManagingObject(std::shared_ptr<swganh::object::Object> object)
		{
			fsm_manager_.StopManagingObject(object);
		}

		 void Startup();

	private:
		void _timerTick(const boost::system::error_code& e);

		FsmManager fsm_manager_;

		swganh::app::SwganhKernel* kernel_;

		boost::asio::deadline_timer timer_;
	};
}
}
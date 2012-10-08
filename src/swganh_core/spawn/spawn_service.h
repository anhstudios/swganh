// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#ifndef SWGANH_SPAWN_SERVICE_H_
#define SWGANH_SPAWN_SERVICE_H_

#include <boost/asio/deadline_timer.hpp>

#include "swganh/spawn/spawn_service_interface.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh/object/permissions/permission_type.h"
#include "fsm_manager.h"

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

#endif
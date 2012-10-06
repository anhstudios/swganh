// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#ifndef SWGANH_SPAWN_SERVICE_H_
#define SWGANH_SPAWN_SERVICE_H_

#include "swganh/spawn/spawn_service_interface.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh/object/permissions/permission_type.h"

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
	
		 void Startup();

	private:
		swganh::object::PermissionType FindProperPermission_(const std::string& iff_name);
		swganh::app::SwganhKernel* kernel_;
	};
}
}

#endif
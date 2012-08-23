#ifndef SWGANH_SPAWN_SERVICE_H_
#define SWGANH_SPAWN_SERVICE_H_

#include "swganh/spawn/spawn_service_interface.h"

#include "swganh/app/swganh_kernel.h"

namespace swganh_core
{
namespace spawn
{
	class SpawnService : public swganh::spawn::SpawnServiceInterface
	{
	public:
		SpawnService(swganh::app::SwganhKernel* kernel);
		~SpawnService();
		
		 anh::service::ServiceDescription GetServiceDescription();
	
	private:
		swganh::app::SwganhKernel* kernel_;
	};
}
}

#endif
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/galaxy/galaxy_service.h"

#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/galaxy/providers/galaxy_provider_interface.h"

using namespace anh;
using namespace app;
using namespace service;
using namespace swganh::app;
using namespace swganh::galaxy;
using namespace std;

GalaxyService::GalaxyService(SwganhKernel* kernel)
    : BaseService(kernel)
{
	galaxy_provider_ = kernel->GetPluginManager()->CreateObject<providers::GalaxyProviderInterface>("GalaxyService::GalaxyProvider");
}

ServiceDescription GalaxyService::GetServiceDescription()
{
	ServiceDescription service_description(        
		"ANH Galaxy Service",
        "galaxy",
        "0.1",
        "127.0.0.1", 
        0,
        0, 
        0);
	return service_description;
}

uint32_t GalaxyService::GetPopulation()
{
	return galaxy_provider_->GetPopulation();
}
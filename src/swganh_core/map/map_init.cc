// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "map_init.h"

#include <version.h>
#include <swganh/app/swganh_kernel.h>
#include <swganh/plugin/bindings.h>
#include <swganh/plugin/plugin_manager.h>

#include <swganh_core/map/map_service.h>

namespace swganh {
namespace map {

	void Initialize(swganh::app::SwganhKernel* kernel)
	{
		swganh::plugin::ObjectRegistration registration;
		registration.version.major = VERSION_MAJOR;
		registration.version.minor = VERSION_MINOR;

		// Register Map Service
		{ // Map::MapService
			registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
				auto map_service = new MapService(kernel);
				return map_service;
			};

			registration.DestroyObject = [] (void* object) {
				if(object) {
					delete static_cast<MapService*>(object);
				}
			};

			kernel->GetPluginManager()->RegisterObject("Map::MapService", &registration);
		}
	}

}} // namespace swganh::map
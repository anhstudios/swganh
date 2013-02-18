// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "travel_service.h"

#include "version.h"

namespace swganh {
namespace travel {

inline void Initialize(swganh::app::SwganhKernel* kernel)
{
	swganh::plugin::ObjectRegistration registration;
	registration.version.major = VERSION_MAJOR;
	registration.version.minor = VERSION_MINOR;

	registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
		return new TravelService(kernel);
	};

	registration.DestroyObject = [] (void* object) {
		if(object) {
			delete static_cast<TravelService*>(object);
		}
	};

	kernel->GetPluginManager()->RegisterObject("Travel::TravelService", &registration);
}

}} // namespace swganh::badge
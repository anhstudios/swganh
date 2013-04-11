// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "badge_init.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "badge_service.h"

#include "version.h"

namespace swganh {
namespace badge {

void Initialize(swganh::app::SwganhKernel* kernel)
{
	swganh::plugin::ObjectRegistration registration;
	registration.version.major = VERSION_MAJOR;
	registration.version.minor = VERSION_MINOR;

	registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
		return new BadgeService(kernel);
	};

	registration.DestroyObject = [] (void* object) {
		if(object) {
			delete static_cast<BadgeService*>(object);
		}
	};

	kernel->GetPluginManager()->RegisterObject("Badge::BadgeService", &registration);
}

}} // namespace swganh::badge
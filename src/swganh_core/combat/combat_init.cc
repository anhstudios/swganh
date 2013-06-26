// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "combat_init.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "combat_service.h"

#include "version.h"

namespace swganh {
namespace combat {

void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;

    // Register
    registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
        return new CombatService(kernel);
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<CombatService*>(object);
        }
    };

    kernel->GetPluginManager()->RegisterObject("Combat::CombatService", &registration);    
}

}}  // namespace swganh::galaxy

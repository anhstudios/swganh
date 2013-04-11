// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "attributes_init.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "attributes_service.h"
#include "version.h"

namespace swganh {
namespace attributes {

void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Attribute Service
	{ // Attribute::AttributeService
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            auto attributes_service = new AttributesService(kernel);
            
            return attributes_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<AttributesService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Attribute::AttributesService", &registration);
	}
}

}}  // namespace swganh::attribute

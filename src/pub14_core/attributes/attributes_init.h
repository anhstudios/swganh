// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_ATTRIBUTES_INITIALIZATION_H_
#define SWGANH_ATTRIBUTES_INITIALIZATION_H_

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "attributes_service.h"
#include "version.h"

namespace swganh_core {
namespace attributes {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    anh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Attribute Service
	{ // Attribute::AttributeService
        registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
            auto attributes_service = new AttributeService(kernel);
            
            return attributes_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<AttributeService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Attribute::AttributeService", &registration);
	}
}

}}  // namespace swganh_core::attribute

#endif  // SWGANH_ATTRIBUTES_INITIALIZATION_H_

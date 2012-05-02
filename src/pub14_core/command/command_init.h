// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_INITIALIZATION_H_
#define PUB14_CORE_COMMAND_INITIALIZATION_H_

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "command_properties_tre_loader.h"
#include "version.h"

namespace pub14_core {
namespace command {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    anh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register
    registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
        return new CommandPropertiesTreLoader(kernel->GetTreArchive());
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<CommandPropertiesTreLoader*>(object);
        }
    };

    kernel->GetPluginManager()->RegisterObject("Command::PropertiesLoader", &registration);
}

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_INITIALIZATION_H_

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

#include "command_factory.h"
#include "command_properties_tre_loader.h"
#include "command_queue.h"
#include "version.h"

namespace pub14_core {
namespace command {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    anh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    { // Command::CommandFactory
        registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
            return new CommandFactory();
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandFactory*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandFactory", &registration);
    }

    { // Command::PropertiesLoader
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

    { // Command::Queue
        registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
            return new CommandQueue(kernel);
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandQueue*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::Queue", &registration);
    }
}

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_INITIALIZATION_H_

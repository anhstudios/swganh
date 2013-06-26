// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_init.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "command_factory.h"
#include "command_filter.h"
#include "command_properties_manager.h"
#include "command_queue.h"
#include "command_queue_manager.h"
#include "command_service.h"
#include "command_validator.h"

#include "version.h"

namespace swganh {
namespace command {

void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    { // Command::CommandFactory
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            auto factory = new CommandFactory();
            return factory;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandFactory*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandFactory", &registration);
    }

    { // Command::CommandQueue
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            return new CommandQueue(kernel);
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandQueue*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandQueue", &registration);
    }

    { // Command::CommandQueueManager
        registration.CreateObject = [] (swganh::plugin::ObjectParams* params) -> void * {
            return new CommandQueueManager();
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandQueueManager*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandQueueManager", &registration);
    }

    { // Command::CommandPropertiesManager
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            return new CommandPropertiesManager(kernel->GetResourceManager());
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandPropertiesManager*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandPropertiesManager", &registration);
    }

    { // Command::CommandService
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            auto command_service = new CommandService(kernel);
            
            return command_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandService", &registration);
    }

    { // Command::CommandValidator
        registration.CreateObject = [] (swganh::plugin::ObjectParams* params) -> void * {
            return new CommandValidator();
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CommandValidator*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Command::CommandValidator", &registration);
    }
}

}}  // namespace swganh::command


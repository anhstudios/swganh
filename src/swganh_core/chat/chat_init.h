// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "chat_service.h"
#include "version.h"

namespace swganh {
namespace chat {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Chat Service
	{ // Chat::ChatService
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            auto chat_service = new ChatService(kernel);
            
            return chat_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<ChatService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Chat::ChatService", &registration);
	}
}

}}  // namespace swganh::chat

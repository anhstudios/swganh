// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "player_service.h"

namespace swganh_core {
namespace player {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    anh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Player Service
	{ // Player::PlayerService
        registration.CreateObject = [kernel] (anh::plugin::ObjectParams* params) -> void * {
            auto chat_service = new PlayerService(kernel);
            
            return chat_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<PlayerService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Player::PlayerService", &registration);
	}
}

}}  // namespace swganh_core::player

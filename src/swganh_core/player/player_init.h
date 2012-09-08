// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "player_service.h"

namespace swganh {
namespace player {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Player Service
	{ // Player::PlayerService
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
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

}}  // namespace swganh::player

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/character/character_service.h"

#include "mysql_character_provider.h"
#include "version.h"

namespace swganh {
namespace character {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
	
	// Register
	{
		registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
			return new MysqlCharacterProvider(kernel);
		};

		registration.DestroyObject = [] (void * object) {
			if (object) {
				delete static_cast<MysqlCharacterProvider*>(object);
			}
		};

		kernel->GetPluginManager()->RegisterObject("Character::CharacterProvider", &registration);    
	}
	// Register Character Service
	{ // Character::CharacterService
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            auto character_service = new CharacterService(kernel);
            
            return character_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<CharacterService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Character::CharacterService", &registration);
	}
}

}}  // namespace swganh::mysql_auth

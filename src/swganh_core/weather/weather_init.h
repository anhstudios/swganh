// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CORE_WEATHER_INITIALIZATION_H_
#define SWGANH_CORE_WEATHER_INITIALIZATION_H_

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "weather_service.h"
#include "version.h"

namespace swganh {
namespace weather {

inline void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Chat Service
	{ // Chat::ChatService
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {
            auto weather_service = new WeatherService(kernel);
            
            return weather_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<WeatherService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Weather::WeatherService", &registration);
	}
}

}}  // namespace swganh::chat

#endif  // SWGANH_CORE_WEATHER_INITIALIZATION_H_
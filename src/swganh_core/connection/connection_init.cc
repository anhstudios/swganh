// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "connection_init.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "connection_service.h"
#include "version.h"

namespace swganh {
namespace connection {

void Initialize(swganh::app::SwganhKernel* kernel) 
{    
    swganh::plugin::ObjectRegistration registration;
    registration.version.major = VERSION_MAJOR;
    registration.version.minor = VERSION_MINOR;
    
    // Register Connection Service
	{ // 
        registration.CreateObject = [kernel] (swganh::plugin::ObjectParams* params) -> void * {

			auto app_config = kernel->GetAppConfig();

			auto connection_service = new ConnectionService(
			app_config.connection_config.listen_address, 
			app_config.connection_config.listen_port, 
			app_config.connection_config.ping_port, 
			kernel);

            return connection_service;
        };

        registration.DestroyObject = [] (void * object) {
            if (object) {
                delete static_cast<ConnectionService*>(object);
            }
        };

        kernel->GetPluginManager()->RegisterObject("Connection::ConnectionService", &registration);
	}
}

}}  // namespace swganh::connection

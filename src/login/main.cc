/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/thread.hpp>

#include <glog/logging.h>

#include "anh/app/kernel_interface.h"
#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/character/base_character_service.h"

#include "login/login_service.h"

using namespace anh;
using namespace app;
using namespace plugin;
using namespace service;
using namespace swganh::character;
using namespace event_dispatcher;
using namespace login;
using namespace std;
using boost::any_cast;
using boost::program_options::options_description;
using boost::program_options::variables_map;

static struct LoginConfig {
    string listen_address;
    uint16_t listen_port;
    int galaxy_status_check_duration_secs;
    int login_error_timeout_secs;
} login_config;

extern "C" PLUGIN_API void ExitModule() {
    return;
}

extern "C" PLUGIN_API ExitFunc InitializePlugin(shared_ptr<KernelInterface> kernel) {

    ObjectRegistration registration;
    registration.version.major = 1;
    registration.version.minor = 0;

    // Register TestObj
    registration.CreateObject = [] (ObjectParams* params) -> void * {
        LoginService* login_service = new LoginService(
            login_config.listen_address, 
            login_config.listen_port, 
            params->kernel);

        login_service->galaxy_status_check_duration_secs(login_config.galaxy_status_check_duration_secs);
        login_service->login_error_timeout_secs(login_config.login_error_timeout_secs);

        return login_service;
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<LoginService*>(object);
        }
    };

    kernel->GetPluginManager()->RegisterObject("LoginService", &registration);

    return ExitModule;
}

extern "C" PLUGIN_API void ConfigurePlugin(options_description& description) {
    description.add_options()
        ("service.login.udp_port", 
            boost::program_options::value<uint16_t>(&login_config.listen_port),
            "The port the login service will listen for incoming client connections on")
        ("service.login.address", 
            boost::program_options::value<string>(&login_config.listen_address),
            "The public address the login service will listen for incoming client connections on")
        ("service.login.status_check_duration_secs", 
            boost::program_options::value<int>(&login_config.galaxy_status_check_duration_secs),
            "The amount of time between checks for updated galaxy status")
        ("service.login.login_error_timeout_secs", 
            boost::program_options::value<int>(&login_config.login_error_timeout_secs)->default_value(5),
            "The number of seconds to wait before disconnecting a client after failed login attempt")
    ;
}

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

#include "main.h"

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/thread.hpp>

#include <glog/logging.h>

#include "swganh/character/character_service_interface.h"

#include "login/login_service.h"

using namespace anh;
using namespace database;
using namespace swganh::character;
using namespace event_dispatcher;
using namespace login;
using namespace module_manager;
using namespace std;
using boost::any_cast;
using boost::program_options::options_description;
using boost::program_options::variables_map;

static std::shared_ptr<LoginService> login_service;

bool API Load(shared_ptr<PlatformServices> services) {
    cout << GetModuleName() << " Loading..." << endl;
    
    // subscribe to events
    auto event_dispatcher = any_cast<shared_ptr<EventDispatcherInterface>>(
        services->getService("EventDispatcher"));
    
    if (!event_dispatcher) {
        LOG(FATAL) << "No Event Dispatcher Registered";
    }

    auto db_manager = any_cast<shared_ptr<DatabaseManagerInterface>>(
        services->getService("DatabaseManager"));
    
    if (!db_manager) {
        LOG(FATAL) << "No Database Manager Registered";
    }

    auto module_config = any_cast<pair<options_description, variables_map>*>(
        services->getService("ModuleConfig"));
    
    if (!module_config) {
        LOG(FATAL) << "No ModuleConfig container registered!";
        return false;
    }

    login_service = make_shared<LoginService>(event_dispatcher, db_manager);

    login_service->DescribeConfigOptions(module_config->first);
    
    cout << GetModuleName() << " Loaded!" << endl;

    return true;
}

void API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {}

void API Start(std::shared_ptr<anh::module_manager::PlatformServices> services) {        
    if (!services->hasService("CharacterService")) {
        cout << "No Character Service Found" << endl;
    } else {
        auto character_service = any_cast<shared_ptr<CharacterServiceInterface>>(services->getService("CharacterService"));
        login_service->character_service(character_service);
        cout << "Found Character Service" << endl;
    }
    
    boost::thread t([] () { login_service->Start(); });
}

void API Stop(std::shared_ptr<anh::module_manager::PlatformServices> services) {    
    cout << GetModuleName() << " Stopping..." << endl;

    login_service->Stop();
    
    while (login_service->IsRunning()) { }

    
    cout << GetModuleName() << " Stopped!" << endl;
}

const std::string API GetModuleName(void) {
    return "ANH Login Service";
}

const std::string API GetModuleType(void) {
    return "LoginService";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
    return anh::module_manager::ModuleApiVersion(1, 0, "1.0");
}

const std::string API GetModuleDescription(void) {
    return "Provides client login services to the cluster.";
}

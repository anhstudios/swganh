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

#include "connection/connection_service.h"

using namespace anh;
using namespace event_dispatcher;
using namespace connection;
using namespace module_manager;
using namespace std;
using boost::any_cast;
using boost::program_options::options_description;
using boost::program_options::variables_map;

static std::shared_ptr<ConnectionService> connection_service;

bool API Load(shared_ptr<PlatformServices> services) {
    cout << GetModuleName() << " Loading..." << endl;
    
    // subscribe to events
    auto event_dispatcher = any_cast<shared_ptr<EventDispatcherInterface>>(
        services->getService("EventDispatcher"));
    
    if (!event_dispatcher) {
        LOG(FATAL) << "No Event Dispatcher Registered";
    }
        
    auto module_config = any_cast<pair<options_description, variables_map>*>(
        services->getService("ModuleConfig"));
    
    if (!module_config) {
        LOG(FATAL) << "No ModuleConfig container registered!";
        return false;
    }

    connection_service = make_shared<ConnectionService>(event_dispatcher);

    connection_service->DescribeConfigOptions(module_config->first);
    
    cout << GetModuleName() << " Loaded!" << endl;

    return true;
}

void API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {}

void API Start(std::shared_ptr<anh::module_manager::PlatformServices> services) {    
    boost::thread t([] () { connection_service->Start(); });
}

void API Stop(std::shared_ptr<anh::module_manager::PlatformServices> services) {  
    cout << GetModuleName() << " Stopping..." << endl;

    connection_service->Stop();

    while (connection_service->IsRunning()) { }
    
    cout << GetModuleName() << " Stopped!" << endl;
}

const std::string API GetModuleName(void) {
    return "ANH Connection Service";
}

const std::string API GetModuleType(void) {
    return "ConnectionService";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
    return anh::module_manager::ModuleApiVersion(1, 0, "1.0");
}

const std::string API GetModuleDescription(void) {
    return "Provides client connection services to the cluster.";
}

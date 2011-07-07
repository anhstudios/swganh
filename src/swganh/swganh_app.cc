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

#include "swganh/swganh_app.h"

#include <exception>
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

#include <glog/logging.h>

#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/module_manager/module_manager.h>
#include <anh/module_manager/platform_services.h>

#include "version.h"

using namespace anh;
using namespace boost::program_options;
using namespace event_dispatcher;
using namespace module_manager;
using namespace std;
using namespace swganh;

options_description AppConfig::BuildConfigDescription() {
    options_description desc;

    desc.add_options()
        ("help,h", "Display help message and config options")

        ("single_server_mode", boost::program_options::value<bool>(&single_server_mode)->default_value(true),
            "Disables cluster support and runs the server in a single executable")
        ("module,m", boost::program_options::value<std::vector<std::string>>(&modules),
            "Only used when single_server_mode is disabled, loads a module of the specified name")
        ("galaxy_name", boost::program_options::value<std::string>(&galaxy_name),
            "Name of the galaxy (cluster) to this process should run")

        ("db.galaxy_manager.host", boost::program_options::value<std::string>(&galaxy_manager_db.host),
            "Host address for the galaxy_manager datastore")
        ("db.galaxy_manager.schema", boost::program_options::value<std::string>(&galaxy_manager_db.schema),
            "Schema name for the galaxy_manager datastore")
        ("db.galaxy_manager.username", boost::program_options::value<std::string>(&galaxy_manager_db.username),
            "Username for authentication with the galaxy_manager datastore")
        ("db.galaxy_manager.password", boost::program_options::value<std::string>(&galaxy_manager_db.password),
            "Password for authentication with the galaxy_manager datastore")

        ("db.galaxy.host", boost::program_options::value<std::string>(&galaxy_db.host),
            "Host address for the galaxy datastore")
        ("db.galaxy.schema", boost::program_options::value<std::string>(&galaxy_db.schema),
            "Schema name for the galaxy datastore")
        ("db.galaxy.username", boost::program_options::value<std::string>(&galaxy_db.username),
            "Username for authentication with the galaxy datastore")
        ("db.galaxy.password", boost::program_options::value<std::string>(&galaxy_db.password),
            "Password for authentication with the galaxy datastore")
    ;

    return desc;
}

SwganhApp::SwganhApp()
    : event_dispatcher_(make_shared<EventDispatcher>())
    , platform_services_(make_shared<PlatformServices>()) 
{
    running_ = false;
}

SwganhApp::~SwganhApp() {}

void SwganhApp::Initialize(int argc, char* argv[]) {    
    LoadAppConfig_(argc, argv, app_config_);
    
    platform_services_->addService("EventDispatcher", event_dispatcher_);
    platform_services_->addService("ModuleConfig", &module_config_);
    platform_services_->addService("AppConfig", &app_config_);
    
    LoadModules_(app_config_.modules);
    LoadModuleConfig_(module_config_);

    initialized_ = true;
}

void SwganhApp::Start() {
    running_ = true;

    module_manager_->StartModules();

    do {
        event_dispatcher_->tick();
    } while(IsRunning());
}

void SwganhApp::Stop() {
    running_ = false;
    
    module_manager_->StopModules();
}

bool SwganhApp::IsRunning() {
    return running_;
}

void SwganhApp::LoadAppConfig_(int argc, char* argv[], AppConfig& app_config) {
    auto config_description = app_config.BuildConfigDescription();

    variables_map vm;
    store(parse_command_line(argc, argv, config_description), vm);

    ifstream config_file("config/swganh.cfg");

    if (!config_file.is_open()) {
        throw runtime_error("Unable to open the configuration file at: config/swganh.cfg");
    }

    try {
        store(parse_config_file(config_file, config_description, true), vm);
    } catch(...) {
        throw runtime_error("Unable to parse the configuration file at: config/swganh.cfg");
    }

    notify(vm);
    
    if (vm.count("help")) {
        std::cout << config_description << "\n\n";
        exit(0);
    }
}

void SwganhApp::LoadModuleConfig_(ModuleConfig& module_config) {    
    ifstream config_file("config/swganh.cfg");

    if (!config_file.is_open()) {
        throw runtime_error("Unable to open the configuration file at: config/swganh.cfg");
    }

    try {
        store(parse_config_file(config_file, module_config.first, true), module_config.second);
    } catch(...) {
        throw runtime_error("Unable to parse the configuration file at: config/swganh.cfg");
    }

    notify(module_config.second);
}

void SwganhApp::LoadModules_(vector<string> modules) {    
    DLOG(INFO) << "Loading application modules";

    auto modules_version = make_shared<ModuleApiVersion>(
        VERSION_MAJOR, 
        VERSION_MINOR, 
        string(VERSION_MAJOR + "." + VERSION_MINOR));

    module_manager_ = make_shared<ModuleManager>(platform_services_, modules_version);
        
    module_manager_->LoadModules(modules);
}

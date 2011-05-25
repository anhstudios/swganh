/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "connection_app.h"
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_manager.h>
#include <anh/clock.h>

#include <iostream>
#include <glog/logging.h>

#include <boost/thread/thread.hpp>

using namespace std;
using namespace anh;
using namespace scripting;
using namespace component;
using namespace event_dispatcher;
using namespace module_manager;

namespace connection {
ConnectionApp::ConnectionApp(int argc, char* argv[], list<string> config_files
    , shared_ptr<anh::module_manager::PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
{
    auto startupListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Connection Application Startup" <<endl;
        
        cout << "Connection Application Started" << endl;
        started_ = true;
        return true;
    };
    auto processListener = [] (shared_ptr<EventInterface> incoming_event)-> bool {
        return true;
    };
    auto shutdownListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Connection Application Shutdown" <<endl;
        started_ = false;
        return true;
    };

    // event subscription
    event_dispatcher_->subscribe("Startup", startupListener);
    event_dispatcher_->subscribe("Process", processListener);
    event_dispatcher_->subscribe("Shutdown", shutdownListener);
}
ConnectionApp::~ConnectionApp() {
    //destructor
}
void ConnectionApp::startup() {
    cluster_service_->Start(45566);
    
    started_ = true;
    BaseApplication::startup();
    cluster_io_service_.run();
}
void ConnectionApp::update() {
    cluster_service_->Update();
}
void ConnectionApp::shutdown() {
    cluster_service_->Shutdown();
}

void ConnectionApp::onAddDefaultOptions_() {
}
void ConnectionApp::onRegisterApp_() {
}


} //namespace connection
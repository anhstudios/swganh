/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */
#include "shared_services_app.h"
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
using namespace event_dispatcher;
using namespace module_manager;

namespace shared_services {
SharedServicesApp::SharedServicesApp(int argc, char* argv[], list<string> config_files
    , shared_ptr<anh::module_manager::PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
{
    auto startupListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "SharedServices Application Startup" <<endl;
        started_ = true;
        cout << "SharedServices Application Started" << endl;

        return true;
    };
    auto processListener = [] (shared_ptr<EventInterface> incoming_event)-> bool {
        return true;
    };
    auto shutdownListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "SharedServices Application Shutdown" <<endl;
        started_ = false;
        return true;
    };

    // event subscription
    event_dispatcher_->subscribe("Startup", startupListener);
    event_dispatcher_->subscribe("Process", processListener);
    event_dispatcher_->subscribe("Shutdown", shutdownListener);
}
SharedServicesApp::~SharedServicesApp() {
    //destructor
}
void SharedServicesApp::onAddDefaultOptions_() {
}
void SharedServicesApp::onRegisterApp_() {
}
} //namespace shared_services

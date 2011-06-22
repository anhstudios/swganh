/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "swganh_app.h"

#include <iostream>

#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/module_manager/platform_services.h>

using namespace std;
using namespace anh;
using namespace event_dispatcher;
using namespace module_manager;
using namespace swganh;

SwganhApp::SwganhApp(
    int argc, 
    char* argv[], 
    list<string> config_files, 
    shared_ptr<PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
{
    // event subscription
    event_dispatcher_->subscribe("Startup", 
        [&] (shared_ptr<EventInterface> incoming_event)-> bool 
    {
        cout << "SWGANH Application Startup" << endl;
        started_ = true;
        cout << "SWGANH Application Started" << endl;

        return true;
    });

    event_dispatcher_->subscribe("Process", [] (shared_ptr<EventInterface> incoming_event)-> bool {
        return true;
    });

    event_dispatcher_->subscribe("Shutdown", [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "SWGANH Application Shutdown" << endl;
        started_ = false;
        return true;
    });
}

SwganhApp::~SwganhApp() {}

void SwganhApp::onAddDefaultOptions_() {}

void SwganhApp::onRegisterApp_() {}

/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */
#include "zone_app.h"
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/component/entity_manager.h>
#include <anh/component/entity_builder.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_manager.h>
#include <anh/clock.h>

#include <iostream>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace anh;
using namespace scripting;
using namespace component;
using namespace event_dispatcher;
using namespace module_manager;

namespace zone {
ZoneApp::ZoneApp(int argc, char* argv[], list<string> config_files
    , shared_ptr<anh::module_manager::PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
{
    auto startupListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        // get zone name
        if(configuration_variables_map_.count("ZoneName") == 0) {
            std::cout << "Enter a zone: ";
            std::cin >> zone_name_;
        } else {
            zone_name_ = configuration_variables_map_["ZoneName"].as<std::string>();
        }

        cout << "Zone Application Startup For [" << zone_name_ << "]" <<endl;
        started_ = true;
        cout << "[" << zone_name_ << "] Started" << endl;

        return true;
    };
    auto processListener = [] (shared_ptr<EventInterface> incoming_event)-> bool {
        return true;
    };
    auto shutdownListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Zone Application Shutdown" <<endl;
        started_ = false;
        return true;
    };

    // event subscription
    event_dispatcher_->subscribe("Startup", startupListener);
    event_dispatcher_->subscribe("Process", processListener);
    event_dispatcher_->subscribe("Shutdown", shutdownListener);
}
ZoneApp::~ZoneApp() {
    //destructor
}
void ZoneApp::onAddDefaultOptions_() {
    configuration_options_description_.add_options()
    ("ZoneName", boost::program_options::value<std::string>());
}
void ZoneApp::onRegisterApp_() {
}
} //namespace zone
using namespace zone;
int main(int argc, char* argv[])
{
    /// pre startup
    // config files
    list<string> config;
    config.push_back("config/general.cfg");
    shared_ptr<EventDispatcherInterface> dispatcher = make_shared<EventDispatcher>();
    shared_ptr<ScriptingManagerInterface> scripter = make_shared<ScriptingManager>("scripts");
    shared_ptr<EntityManager> object_manager = make_shared<EntityManager>();
    shared_ptr<Clock> clock = make_shared<Clock>();
    auto services = make_shared<PlatformServices>();
    // add services
    services->addService("EventDispatcher", dispatcher);
    services->addService("ScriptingManager", scripter);
    services->addService("EntityManager", object_manager);
    services->addService("Clock", clock);
    ZoneApp app(argc, argv, config, services);
    ///
    app.startup();
    boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    if (app.hasStarted()) {
        while(true){
            app.process();
            if (app.kbHit())
            {
                if(cin.get() == 'q')
                {
                    app.shutdown();
                    return 0;
                }
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
    }
}
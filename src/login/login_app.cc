/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */
#include "login_app.h"
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

namespace login {
LoginApp::LoginApp(int argc, char* argv[], list<string> config_files
    , shared_ptr<anh::module_manager::PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
{
    auto startupListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Login Application Startup" <<endl;
        
        started_ = true;
        cout << "Login Application Started" << endl;

        return true;
    };
    auto processListener = [] (shared_ptr<EventInterface> incoming_event)-> bool {
        // return galaxy status list every 5s
        // heartbeat
        return true;
    };
    auto shutdownListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Login Application Shutdown" <<endl;
        started_ = false;
        return true;
    };

    // event subscription
    event_dispatcher_->subscribe("Startup", startupListener);
    event_dispatcher_->subscribe("Process", processListener);
    event_dispatcher_->subscribe("Shutdown", shutdownListener);
}
LoginApp::~LoginApp() {
    //destructor
}
void LoginApp::onAddDefaultOptions_() {
}
void LoginApp::onRegisterApp_() {
}

} //namespace login
using namespace login;
int main(int argc, char* argv[])
{
    /// pre startup
    // Initialize the google logging.
    google::InitGoogleLogging(argv[0]);

    #ifndef _WIN32
        google::InstallFailureSignalHandler();
    #endif

    FLAGS_log_dir = "./logs";
    FLAGS_stderrthreshold = 1;
    setvbuf( stdout, NULL, _IONBF, 0);
    // config files
    list<string> config;
    config.push_back("config/login.cfg");
    shared_ptr<EventDispatcherInterface> dispatcher = make_shared<EventDispatcher>();
    shared_ptr<ScriptingManagerInterface> scripter = make_shared<ScriptingManager>("scripts");
    shared_ptr<Clock> clock = make_shared<Clock>();
    auto services = make_shared<PlatformServices>();
    // add services
    services->addService("EventDispatcher", dispatcher);
    services->addService("ScriptingManager", scripter);
    services->addService("Clock", clock);
    LoginApp app(argc, argv, config, services);
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
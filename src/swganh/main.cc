#include "swganh/swganh_app.h"

#include <iostream>
#include <glog/logging.h>
#include <boost/thread/thread.hpp>

#include "anh/clock.h"
#include "anh/event_dispatcher/event_dispatcher.h"
#include "anh/module_manager/platform_services.h"

using namespace anh;
using namespace boost::posix_time;
using namespace boost::this_thread;
using namespace event_dispatcher;
using namespace module_manager;
using namespace scripting;
using namespace std;
using namespace swganh;

int main(int argc, char* argv[]) {
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
    config.push_back("config/general.cfg");

    auto services = make_shared<PlatformServices>();
    
    // add services
    services->addService("EventDispatcher", make_shared<EventDispatcher>());
    services->addService("Clock", make_shared<Clock>());
    
    SwganhApp app(argc, argv, config, services);
    
    ///
    app.startup();
    sleep(milliseconds(5));

    if (app.hasStarted()) {
        while(true) {
            app.process();
            if (app.kbHit()) {
                if(cin.get() == 'q') {
                    app.shutdown();
                    return 0;
                }
            }
            sleep(milliseconds(1));
        }
    }
}
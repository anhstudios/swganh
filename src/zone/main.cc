#include <zone/zone_app.h>
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/component/entity_manager.h>
#include <anh/component/entity_builder.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_manager.h>
#include <anh/clock.h>

#ifdef ERROR
#undef ERROR
#endif

#include <iostream>
#include <glog/logging.h>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace anh;
using namespace scripting;
using namespace component;
using namespace event_dispatcher;
using namespace module_manager;
using namespace zone;

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
    if (/*app.hasStarted()*/true) {
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
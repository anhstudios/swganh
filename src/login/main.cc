#include "main.h"

#include <iostream>

#include <glog/logging.h>
#include <tbb/compat/thread>

#include "login/login_service.h"

using namespace anh;
using namespace event_dispatcher;
using namespace login;
using namespace module_manager;
using namespace std;
using boost::any_cast;

bool API Load(shared_ptr<PlatformServices> services) {
    cout << GetModuleName() << " Loading..." << endl;

    // subscribe to events
    auto event_dispatcher = any_cast<shared_ptr<EventDispatcherInterface>>(
        services->getService("EventDispatcher"));
    if (event_dispatcher == nullptr)
    {
        LOG(FATAL) << "No Event Dispatcher Registered";
    }
    
    auto login_service = make_shared<LoginService>(event_dispatcher);

    std::thread t([login_service]() {
        login_service->Run();
    });

    return true;
}

bool API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
    return true;
}

const std::string API GetModuleName(void) {
    return "Login Service";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
    return anh::module_manager::ModuleApiVersion(0, 1, "0.1");
}

const std::string API GetModuleDescription(void) {
    return "Provides client login services to the cluster.";
}

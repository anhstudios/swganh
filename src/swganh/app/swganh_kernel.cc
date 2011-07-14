
#include "swganh/app/swganh_kernel.h"

#include "anh/event_dispatcher/event_dispatcher.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "version.h"

using namespace swganh::app;

using anh::app::Version;
using anh::event_dispatcher::EventDispatcherInterface;
using anh::event_dispatcher::EventDispatcher;
using anh::plugin::PluginManager;
using anh::service::ServiceManager;

using std::make_shared;
using std::shared_ptr;

SwganhKernel::SwganhKernel() {
    version_.major = VERSION_MAJOR;
    version_.minor = VERSION_MINOR;

    event_dispatcher_ = make_shared<EventDispatcher>();
    plugin_manager_ = make_shared<PluginManager>(shared_from_this());
    service_manager_ = make_shared<ServiceManager>();
}

const Version& SwganhKernel::GetVersion() {
    return version_;
}

shared_ptr<EventDispatcherInterface> SwganhKernel::GetEventDispatcher() {
    return event_dispatcher_;
}

shared_ptr<PluginManager> SwganhKernel::GetPluginManager() {
    return plugin_manager_;
}

shared_ptr<ServiceManager> SwganhKernel::GetServiceManager() {
    return service_manager_;
}

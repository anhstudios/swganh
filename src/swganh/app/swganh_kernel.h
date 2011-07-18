
#ifndef SWGANH_KERNEL_H_
#define SWGANH_KERNEL_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/app/kernel_interface.h"

namespace swganh {
namespace app {
    
class SwganhKernel : public anh::app::KernelInterface, public std::enable_shared_from_this<SwganhKernel> {
public:
    SwganhKernel();

    const anh::app::Version& GetVersion();

    std::shared_ptr<anh::database::DatabaseManagerInterface> GetDatabaseManager();

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> GetEventDispatcher();

    std::shared_ptr<anh::plugin::PluginManager> GetPluginManager();

    std::shared_ptr<anh::service::ServiceManager> GetServiceManager();

private:
    anh::app::Version version_;
    
    std::shared_ptr<anh::database::DatabaseManagerInterface> database_manager_;
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    std::shared_ptr<anh::plugin::PluginManager> plugin_manager_;
    std::shared_ptr<anh::service::ServiceManager> service_manager_;
};

}}  // namespace anh::app

#endif  // SWGANH_KERNEL_H_
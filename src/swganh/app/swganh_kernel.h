
#ifndef SWGANH_KERNEL_H_
#define SWGANH_KERNEL_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/app/kernel_interface.h"

namespace swganh {
namespace app {

struct DatabaseConfig {
    std::string host;
    std::string schema;
    std::string username;
    std::string password;
};

struct AppConfig {
    bool single_server_mode;
    std::vector<std::string> plugins;
    std::string plugin_directory;
    std::string galaxy_name;

    DatabaseConfig galaxy_manager_db;
    DatabaseConfig galaxy_db;

    boost::program_options::options_description BuildConfigDescription();
};
    
class SwganhKernel : public anh::app::KernelInterface, public std::enable_shared_from_this<SwganhKernel> {
public:
    SwganhKernel();

    const anh::app::Version& GetVersion();

    AppConfig& GetAppConfig();

    std::shared_ptr<anh::database::DatabaseManagerInterface> GetDatabaseManager();

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> GetEventDispatcher();

    std::shared_ptr<anh::plugin::PluginManager> GetPluginManager();

    std::shared_ptr<anh::service::ServiceManager> GetServiceManager();

private:
    anh::app::Version version_;
    swganh::app::AppConfig app_config_;
    
    std::shared_ptr<anh::database::DatabaseManagerInterface> database_manager_;
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    std::shared_ptr<anh::plugin::PluginManager> plugin_manager_;
    std::shared_ptr<anh::service::ServiceManager> service_manager_;
};

}}  // namespace anh::app

#endif  // SWGANH_KERNEL_H_
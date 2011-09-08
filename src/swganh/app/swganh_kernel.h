
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
    
struct AppConfig {
	std::string server_mode;
    std::vector<std::string> plugins;
    std::string plugin_directory;
    std::string galaxy_name;

    struct DatabaseConfig {
        std::string host;
        std::string schema;
        std::string username;
        std::string password;
    } galaxy_manager_db, galaxy_db;
    
    struct LoginConfig {
        std::string listen_address;
        uint16_t listen_port;
        int galaxy_status_check_duration_secs;
        int login_error_timeout_secs;
    } login_config;

    struct ConnectionConfig {
        std::string listen_address;
        uint16_t listen_port;
        uint16_t ping_port;
    } connection_config;

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
    
    boost::asio::io_service& GetIoService();

private:
    anh::app::Version version_;
    swganh::app::AppConfig app_config_;
    
    std::shared_ptr<anh::database::DatabaseManagerInterface> database_manager_;
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    std::shared_ptr<anh::plugin::PluginManager> plugin_manager_;
    std::shared_ptr<anh::service::ServiceManager> service_manager_;

    boost::asio::io_service io_service_;
};

}}  // namespace anh::app

#endif  // SWGANH_KERNEL_H_
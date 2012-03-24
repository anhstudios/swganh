
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
namespace tre {
    class TreArchive;
}}  // namespace swganh::tre

namespace swganh {
namespace app {

/*!
 * @Brief Contains information about the app config"
 */
struct AppConfig {
	std::string server_mode;
    std::vector<std::string> plugins;
    std::string plugin_directory;
    std::string galaxy_name;
    std::string tre_config;

    /*!
    * @Brief Contains information about the database config"
    */
    struct DatabaseConfig {
        std::string host;
        std::string schema;
        std::string username;
        std::string password;
    } galaxy_manager_db, galaxy_db;

    /*!
    * @Brief Contains information about the Login config"
     */
    struct LoginConfig {
        std::string listen_address;
        uint16_t listen_port;
        int galaxy_status_check_duration_secs;
        int login_error_timeout_secs;
        bool login_auto_registration;
    } login_config;
    /*!
    * @Brief Contains information about the app config"
    */
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
    ~SwganhKernel();

    const anh::app::Version& GetVersion();

    AppConfig& GetAppConfig();

    anh::database::DatabaseManagerInterface* GetDatabaseManager();
    
    anh::EventDispatcher* GetEventDispatcher();

    anh::plugin::PluginManager* GetPluginManager();

    anh::service::ServiceManager* GetServiceManager();
    
    anh::service::ServiceDirectoryInterface* GetServiceDirectory();
    
    boost::asio::io_service& GetIoService();

    swganh::tre::TreArchive* GetTreArchive();

private:
    anh::app::Version version_;
    swganh::app::AppConfig app_config_;
    
    std::unique_ptr<anh::database::DatabaseManagerInterface> database_manager_;
    std::unique_ptr<anh::EventDispatcher> event_dispatcher_;
    std::unique_ptr<anh::plugin::PluginManager> plugin_manager_;
    std::unique_ptr<anh::service::ServiceManager> service_manager_;
    std::unique_ptr<anh::service::ServiceDirectoryInterface> service_directory_;
    std::unique_ptr<swganh::tre::TreArchive> tre_archive_;

    boost::asio::io_service io_service_;
};

}}  // namespace anh::app

#endif  // SWGANH_KERNEL_H_
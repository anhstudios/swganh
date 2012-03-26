
#include "swganh/app/swganh_kernel.h"

#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>

#include "anh/database/database_manager.h"
#include "anh/event_dispatcher.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/datastore.h"
#include "anh/service/service_directory.h"
#include "anh/service/service_manager.h"

#include "swganh/tre/tre_archive.h"

#include "version.h"

using namespace anh::service;
using namespace swganh::app;

using anh::app::Version;
using anh::database::DatabaseManagerInterface;
using anh::database::DatabaseManager;
using anh::plugin::PluginManager;
using anh::service::ServiceManager;

using std::make_shared;
using std::shared_ptr;

SwganhKernel::SwganhKernel() {
    version_.major = VERSION_MAJOR;
    version_.minor = VERSION_MINOR;

    plugin_manager_ = nullptr;
    service_manager_ = nullptr;
}

SwganhKernel::~SwganhKernel()
{
    service_manager_.reset();
}

const Version& SwganhKernel::GetVersion() {
    return version_;
}

AppConfig& SwganhKernel::GetAppConfig() {
    return app_config_;
}

DatabaseManagerInterface* SwganhKernel::GetDatabaseManager() {
    if (!database_manager_) {
        database_manager_.reset(new DatabaseManager(sql::mysql::get_driver_instance()));
    }

    return database_manager_.get();
}

anh::EventDispatcher* SwganhKernel::GetEventDispatcher() {
    if (!event_dispatcher_) {
        event_dispatcher_.reset(new anh::EventDispatcher(GetIoService()));
    }

    return event_dispatcher_.get();
}

PluginManager* SwganhKernel::GetPluginManager() {
    if (!plugin_manager_) {
        plugin_manager_.reset(new PluginManager(this));
    }

    return plugin_manager_.get();
}

ServiceManager* SwganhKernel::GetServiceManager() {
    if (!service_manager_) {        
        service_manager_.reset(new ServiceManager(GetServiceDirectory()));
    }

    return service_manager_.get();
}

ServiceDirectoryInterface* SwganhKernel::GetServiceDirectory() {
    if (!service_directory_) {        
        auto data_store = make_shared<Datastore>(GetDatabaseManager()->getConnection("galaxy_manager"));
        service_directory_.reset(new ServiceDirectory(
            data_store, 
            GetEventDispatcher(),
            app_config_.galaxy_name, 
            GetVersion().ToString(),
            true));
    }

    return service_directory_.get();
}

boost::asio::io_service& SwganhKernel::GetIoService() {
    return io_service_;
}

swganh::tre::TreArchive* SwganhKernel::GetTreArchive() {
    if (!tre_archive_) {
        tre_archive_.reset(new swganh::tre::TreArchive(GetAppConfig().tre_config));
    }

    return tre_archive_.get();
}


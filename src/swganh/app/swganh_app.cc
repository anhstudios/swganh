
#include "swganh/app/swganh_app.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/program_options.hpp>
#include <glog/logging.h>

#include "anh/database/database_manager_interface.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/datastore.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

using namespace anh;
using namespace anh::app;
using namespace boost::program_options;
using namespace std;
using namespace swganh::app;

options_description AppConfig::BuildConfigDescription() {
    options_description desc;

    desc.add_options()
        ("help,h", "Display help message and config options")

        ("single_server_mode", boost::program_options::value<bool>(&single_server_mode)->default_value(true),
            "Disables cluster support and runs the server in a single executable")
        ("plugin,p", boost::program_options::value<std::vector<std::string>>(&plugins),
            "Only used when single_server_mode is disabled, loads a module of the specified name")
        ("plugin_directory", value<string>(&plugin_directory)->default_value("plugins"),
            "Directory containing the application plugins")

        ("galaxy_name", boost::program_options::value<std::string>(&galaxy_name),
            "Name of the galaxy (cluster) to this process should run")

        ("db.galaxy_manager.host", boost::program_options::value<std::string>(&galaxy_manager_db.host),
            "Host address for the galaxy_manager datastore")
        ("db.galaxy_manager.schema", boost::program_options::value<std::string>(&galaxy_manager_db.schema),
            "Schema name for the galaxy_manager datastore")
        ("db.galaxy_manager.username", boost::program_options::value<std::string>(&galaxy_manager_db.username),
            "Username for authentication with the galaxy_manager datastore")
        ("db.galaxy_manager.password", boost::program_options::value<std::string>(&galaxy_manager_db.password),
            "Password for authentication with the galaxy_manager datastore")

        ("db.galaxy.host", boost::program_options::value<std::string>(&galaxy_db.host),
            "Host address for the galaxy datastore")
        ("db.galaxy.schema", boost::program_options::value<std::string>(&galaxy_db.schema),
            "Schema name for the galaxy datastore")
        ("db.galaxy.username", boost::program_options::value<std::string>(&galaxy_db.username),
            "Username for authentication with the galaxy datastore")
        ("db.galaxy.password", boost::program_options::value<std::string>(&galaxy_db.password),
            "Password for authentication with the galaxy datastore")
    ;

    return desc;
}

SwganhApp::SwganhApp() {
    kernel_ = make_shared<SwganhKernel>();
    running_ = false;
    initialized_ = false;
}

void SwganhApp::Initialize(int argc, char* argv[]) {
    // Load the configuration    
    LoadAppConfig_(argc, argv);

    auto app_config = kernel_->GetAppConfig();

    // Initialize kernel resources    
    kernel_->GetDatabaseManager()->registerStorageType(
        "galaxy_manager",
        app_config.galaxy_manager_db.schema,
        app_config.galaxy_manager_db.host,
        app_config.galaxy_manager_db.username,
        app_config.galaxy_manager_db.password);

    kernel_->GetDatabaseManager()->registerStorageType(
        "galaxy",
        app_config.galaxy_db.schema,
        app_config.galaxy_db.host,
        app_config.galaxy_db.username,
        app_config.galaxy_db.password);
    
    auto data_store = make_shared<service::Datastore>(kernel_->GetDatabaseManager()->getConnection("galaxy_manager"));
    service_directory_ = make_shared<service::ServiceDirectory>(data_store, kernel_->GetEventDispatcher(), app_config.galaxy_name, kernel_->GetVersion().ToString(), true);
    
    CleanupServices_();

    // Load the plugin configuration.
    LoadPlugins_(app_config.plugins);

    kernel_->GetServiceManager()->Initialize(service_config_);

    LoadServiceConfig_(service_config_);

    initialized_ = true;
}

void SwganhApp::Start() {
    if (!initialized_) {
        throw std::runtime_error("Called application Start before Initialize");
    }

    running_ = true;
    
    // Start up the services to ensure there is work for the io_service instance.
    kernel_->GetServiceManager()->Start();

    // Start up a threadpool for running io_service based tasks/active objects
    for (uint32_t i = 0; i < boost::thread::hardware_concurrency(); ++i) {
        auto t = make_shared<boost::thread>(bind(&boost::asio::io_service::run, &kernel_->GetIoService()));
        io_threads_.push_back(t);
    }

    do {
        kernel_->GetEventDispatcher()->tick();
    } while(IsRunning());
                
    kernel_->GetServiceManager()->Stop();

    // stop io handling.
    kernel_->GetIoService().stop();

    // join the threadpool threads until each one has exited.
    for_each(io_threads_.begin(), io_threads_.end(), [] (shared_ptr<boost::thread> t) {
        t->join();
    });
}

void SwganhApp::Stop() {
    running_ = false;
}

bool SwganhApp::IsRunning() {
    return running_;
}

std::shared_ptr<KernelInterface> SwganhApp::GetAppKernel() {
    return kernel_;
}

void SwganhApp::LoadAppConfig_(int argc, char* argv[]) {
    auto config_description = kernel_->GetAppConfig().BuildConfigDescription();

    variables_map vm;
    store(parse_command_line(argc, argv, config_description), vm);

    ifstream config_file("config/swganh.cfg");

    if (!config_file.is_open()) {
        throw runtime_error("Unable to open the configuration file at: config/swganh.cfg");
    }

    try {
        store(parse_config_file(config_file, config_description, true), vm);
    } catch(...) {
        throw runtime_error("Unable to parse the configuration file at: config/swganh.cfg");
    }

    notify(vm);
    config_file.close();

    if (vm.count("help")) {
        std::cout << config_description << "\n\n";
        exit(0);
    }
}

void SwganhApp::LoadServiceConfig_(ServiceConfig& service_config) {    
    ifstream config_file("config/swganh.cfg");

    if (!config_file.is_open()) {
        throw runtime_error("Unable to open the configuration file at: config/swganh.cfg");
    }

    try {
        store(parse_config_file(config_file, service_config.first, true), service_config.second);
    } catch(const std::exception& e) {
        throw runtime_error("Unable to parse the configuration file at: config/swganh.cfg: " + std::string(e.what()));
    }

    notify(service_config.second);
    config_file.close();
}

void SwganhApp::LoadPlugins_(vector<string> plugins) {    
    DLOG(INFO) << "Loading plugins";

    auto plugin_manager = kernel_->GetPluginManager();

    if (plugins.empty()) {
        plugin_manager->LoadAllPlugins(kernel_->GetAppConfig().plugin_directory);
    } else {
        for_each(plugins.begin(), plugins.end(), [plugin_manager] (const string& plugin) {
            plugin_manager->LoadPlugin(plugin);
        });
    }
}

void SwganhApp::CleanupServices_() {
    auto services = service_directory_->getServiceSnapshot(service_directory_->galaxy());

    if (services.empty()) {
        return;
    }

    DLOG(WARNING) << "Services were not shutdown properly";

    for_each(services.begin(), services.end(), [this] (anh::service::ServiceDescription& service) {
        service_directory_->removeService(make_shared<anh::service::ServiceDescription>(service));
    });
}


#include "swganh/app/swganh_app.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>
#include <glog/logging.h>

#include "anh/database/database_manager_interface.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

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
    LoadAppConfig_(argc, argv, app_config_);
    LoadPlugins_(app_config_.plugins);
    
    kernel_->GetDatabaseManager()->registerStorageType(
        "galaxy_db",
        app_config_.galaxy_manager_db.schema,
        app_config_.galaxy_manager_db.host,
        app_config_.galaxy_manager_db.username,
        app_config_.galaxy_manager_db.password);

    initialized_ = true;
}

void SwganhApp::Start() {
    if (!initialized_) {
        throw std::runtime_error("Called application Start before Initialize");
    }

    running_ = true;
    
    kernel_->GetServiceManager()->Start();

    do {
        kernel_->GetEventDispatcher()->tick();
    } while(IsRunning());
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

void SwganhApp::LoadAppConfig_(int argc, char* argv[], AppConfig& app_config) {
    auto config_description = app_config.BuildConfigDescription();

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
    
    if (vm.count("help")) {
        std::cout << config_description << "\n\n";
        exit(0);
    }
}

void SwganhApp::LoadPlugins_(vector<string> plugins) {    
    DLOG(INFO) << "Loading plugins";

    auto plugin_manager = kernel_->GetPluginManager();

    if (plugins.empty()) {
        plugin_manager->LoadAllPlugins(app_config_.plugin_directory);
    } else {
        for_each(plugins.begin(), plugins.end(), [plugin_manager] (const string& plugin) {
            plugin_manager->LoadPlugin(plugin);
        });
    }
}

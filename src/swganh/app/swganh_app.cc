
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

#include "swganh/chat/chat_service.h"
#include "swganh/character/character_service.h"
#include "swganh/command/command_service.h"
#include "swganh/connection/connection_service.h"
#include "swganh/login/login_service.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/galaxy/galaxy_service.h"


using namespace anh;
using namespace anh::app;
using namespace boost::asio;
using namespace boost::program_options;
using namespace std;
using namespace swganh::app;
using namespace swganh::chat;
using namespace swganh::command;
using namespace swganh::login;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::simulation;
using namespace swganh::galaxy;

options_description AppConfig::BuildConfigDescription() {
    options_description desc;

    desc.add_options()
        ("help,h", "Display help message and config options")

		("server_mode", boost::program_options::value<std::string>(&server_mode)->default_value("all"),
			"Specifies the service configuration mode to run the server in.")

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
            
        ("service.login.udp_port", 
            boost::program_options::value<uint16_t>(&login_config.listen_port),
            "The port the login service will listen for incoming client connections on")
        ("service.login.address", 
            boost::program_options::value<string>(&login_config.listen_address),
            "The public address the login service will listen for incoming client connections on")
        ("service.login.status_check_duration_secs", 
            boost::program_options::value<int>(&login_config.galaxy_status_check_duration_secs),
            "The amount of time between checks for updated galaxy status")
        ("service.login.login_error_timeout_secs", 
            boost::program_options::value<int>(&login_config.login_error_timeout_secs)->default_value(5),
            "The number of seconds to wait before disconnecting a client after failed login attempt")
        ("service.login.auto_registration",
            boost::program_options::value<bool>(&login_config.login_auto_registration)->default_value(false),
            "Auto Registration flag")
            
        ("service.connection.ping_port", boost::program_options::value<uint16_t>(&connection_config.ping_port),
            "The port the connection service will listen for incoming client ping requests on")
        ("service.connection.udp_port", boost::program_options::value<uint16_t>(&connection_config.listen_port),
            "The port the connection service will listen for incoming client connections on")
        ("service.connection.address", boost::program_options::value<string>(&connection_config.listen_address),
            "The public address the connection service will listen for incoming client connections on")
    ;

    return desc;
}

SwganhApp::SwganhApp() {
    kernel_ = make_shared<SwganhKernel>();
    running_ = false;
    initialized_ = false;
}

SwganhApp::~SwganhApp() 
{}

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
    
    CleanupServices_();

    // Load the plugin configuration.
    LoadPlugins_(app_config.plugins);

    // Load core services
    LoadCoreServices_();
    
    initialized_ = true;
}

void SwganhApp::Start() {
    if (!initialized_) {
        throw std::runtime_error("Called application Start before Initialize");
    }

    running_ = true;
    
    // Create a work object so that io_service doesn't prematurely exit.
    boost::asio::io_service::work io_work(kernel_->GetIoService());

    // Start up a threadpool for running io_service based tasks/active objects
    for (uint32_t i = 0; i < boost::thread::hardware_concurrency(); ++i) {
        auto t = make_shared<boost::thread>([this] () {
            kernel_->GetIoService().run();
        });
        
#ifdef _WIN32
        HANDLE mtheHandle = t->native_handle();
        SetPriorityClass(mtheHandle, REALTIME_PRIORITY_CLASS);
#endif

        io_threads_.push_back(t);
    }
    
    kernel_->GetServiceManager()->Start();
    
    auto timer = make_shared<boost::asio::deadline_timer>(kernel_->GetIoService(), boost::posix_time::seconds(10));

    timer->async_wait(boost::bind(&SwganhApp::GalaxyStatusTimerHandler_, this, boost::asio::placeholders::error, timer, 10));

    do {
        kernel_->GetEventDispatcher()->tick();

        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    } while(IsRunning());
        
    kernel_->GetServiceManager()->Stop();

    // stop io handling    
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

void SwganhApp::LoadPlugins_(vector<string> plugins) {    
    DLOG(INFO) << "Loading plugins";

    auto plugin_manager = kernel_->GetPluginManager();

    if (!plugins.empty()) {
        auto plugin_manager = kernel_->GetPluginManager();

        for_each(plugins.begin(), plugins.end(), [plugin_manager] (const string& plugin) {
            plugin_manager->LoadPlugin(plugin);
        });
    }
}

void SwganhApp::CleanupServices_() {

    auto service_directory = kernel_->GetServiceDirectory();

    auto services = service_directory->getServiceSnapshot(service_directory->galaxy());

    if (services.empty()) {
        return;
    }

    DLOG(WARNING) << "Services were not shutdown properly";

    for_each(services.begin(), services.end(), [this, &service_directory] (anh::service::ServiceDescription& service) {
        service_directory->removeService(service);
    });
}

void SwganhApp::LoadCoreServices_() 
{
    auto app_config = kernel_->GetAppConfig();

	if(strcmp("login", app_config.server_mode.c_str()) == 0 || strcmp("all", app_config.server_mode.c_str()) == 0)
	{
		auto login_service = make_shared<LoginService>(
		app_config.login_config.listen_address, 
		app_config.login_config.listen_port, 
		kernel_.get());

		login_service->galaxy_status_check_duration_secs(app_config.login_config.galaxy_status_check_duration_secs);
		login_service->login_error_timeout_secs(app_config.login_config.login_error_timeout_secs);
        login_service->login_auto_registration(app_config.login_config.login_auto_registration);
    
		kernel_->GetServiceManager()->AddService("LoginService", login_service);
	} 
	if(strcmp("connection", app_config.server_mode.c_str()) == 0 || strcmp("all", app_config.server_mode.c_str()) == 0)
	{
		auto connection_service = make_shared<ConnectionService>(
			app_config.connection_config.listen_address, 
			app_config.connection_config.listen_port, 
			app_config.connection_config.ping_port, 
			kernel_.get());

		kernel_->GetServiceManager()->AddService("ConnectionService", connection_service);
	}
	if(strcmp("simulation", app_config.server_mode.c_str()) == 0 || strcmp("all", app_config.server_mode.c_str()) == 0)
	{
		kernel_->GetServiceManager()->AddService(
            "CommandService", 
            make_shared<CommandService>(kernel_.get()));

		kernel_->GetServiceManager()->AddService(
            "CharacterService", 
            make_shared<CharacterService>(kernel_.get()));
        
		kernel_->GetServiceManager()->AddService(
            "ChatService", 
            make_shared<ChatService>(kernel_.get()));

		auto simulation_service = make_shared<SimulationService>(kernel_.get());
		simulation_service->StartScene("corellia");
		simulation_service->StartScene("naboo");
		kernel_->GetServiceManager()->AddService("SimulationService", simulation_service);
	}
	// always need a galaxy service running
	auto galaxy_service = make_shared<GalaxyService>(kernel_.get());
	kernel_->GetServiceManager()->AddService("GalaxyService", galaxy_service);
}

    
void SwganhApp::GalaxyStatusTimerHandler_(const boost::system::error_code& e, shared_ptr<deadline_timer> timer, int delay_in_secs)
{
    kernel_->GetServiceDirectory()->updateGalaxyStatus();

    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(delay_in_secs));    
    timer->async_wait(std::bind(&SwganhApp::GalaxyStatusTimerHandler_, this, std::placeholders::_1, timer, delay_in_secs));
}
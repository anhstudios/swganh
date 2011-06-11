/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/
#include "anh/application.h"

#include <cassert>
#include <iostream>
#include <fstream>
// kbhit
#if defined(_MSC_VER)
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>

#include <glog/logging.h>
#include <anh/clock.h>
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/database/database_manager.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/server_directory/server_directory.h>
#include <anh/server_directory/datastore.h>
#include <anh/module_manager/module_manager.h>
#include <anh/module_manager/platform_services.h>
#include <anh/network/cluster/cluster_service.h>

using namespace std;
using namespace anh;
using namespace event_dispatcher;
using namespace database;
using namespace scripting;
using namespace server_directory;
using namespace module_manager;
using namespace network::cluster;
using namespace boost::program_options;

BaseApplication::BaseApplication(int argc, char* argv[], list<string> config_files
, shared_ptr<PlatformServices> platform_services)
    : configuration_options_description_("Configuration Options")
    , argc_(argc)
    , argv_(argv)
    , started_(false)
    , config_files_(config_files)
    , platform_services_(platform_services)
{
    getPreStartupPlatformServices_();
}

BaseApplication::BaseApplication(int argc, char* argv[]
, shared_ptr<PlatformServices> platform_services)
    : configuration_options_description_("Configuration Options")
    , argc_(argc)
    , argv_(argv)
    , started_(false)
    , config_files_(0)
    , platform_services_(platform_services)
{
    getPreStartupPlatformServices_();
}
BaseApplication::BaseApplication(list<string> config_files
, shared_ptr<PlatformServices> platform_services)
    : configuration_options_description_("Configuration Options")
    , argc_(0)
    , argv_(nullptr)
    , started_(false)
    , config_files_(config_files)
    , platform_services_(platform_services)
{
    getPreStartupPlatformServices_();
}

BaseApplication::~BaseApplication() {}

void BaseApplication::startup() {
    try {
        // instantiate basic events
        startup_event_ = make_shared_event("Startup");
        process_event_ = make_shared_event("Process");
        shutdown_event_ = make_shared_event("Shutdown");
        // add in options
        addDefaultOptions_();
        // load the options
        loadOptions_(argc_, argv_, config_files_);
        // init services
        init_services_();
        // load up cluster networking
        setupCluster_();
        // get a database manager
        if (db_manager_ == nullptr) {
            db_manager_ = createDatabaseManager(sql::mysql::get_driver_instance());
            platform_services_->addService("DatabaseManager", db_manager_);
        }
        // loads the data sources from config into db_manager
        if (addDataSourcesFromOptions_()) {
            // TODO: fix ServerDirectory database
            registerApp_();
        }
        else {
            LOG(WARNING) << "NO Datasources Loaded from Config";
        }
        // Startup the event
        if (event_dispatcher_ != nullptr) {
            event_dispatcher_->trigger(startup_event_);
        }
        else {
            throw runtime_error("No Event Dispatcher Registered");
        }
        // set any services we might need to add so the Modules have them.
        setPlatformServices_();
        // setup ModuleManager and load modules
        setupModules_();
        // start the ClusterService
        cluster_service_->Start();

    }
    catch(exception e) {
        cerr << e.what() << endl;
        LOG(FATAL) << "Exception: " << e.what() <<endl;
    }
    catch(...) {
        cerr << "Error in startup";
    }
    started_ = true;
}

void BaseApplication::process() {
    if (!hasStarted()) {
        assert(false && "Must call startup before process");
        return;
    }
    event_dispatcher_->tick(clock_->global_time());
    event_dispatcher_->trigger(process_event_);
    cluster_service_->Update();
}

void BaseApplication::shutdown() {
    // clean up code here before the server shuts down
    event_dispatcher_->trigger(shutdown_event_);
    cluster_service_->Shutdown();
    cluster_io_service_.stop();
}

shared_ptr<ServerDirectoryInterface> BaseApplication::createServerDirectory(shared_ptr<sql::Connection> conn) {
    auto datastore = make_shared<Datastore>(conn);
    return make_shared<ServerDirectory>(datastore, event_dispatcher_);
};
shared_ptr<DatabaseManager> BaseApplication::createDatabaseManager(sql::Driver* driver)
{
    return make_shared<DatabaseManager>(driver);
}
void BaseApplication::setupLogging() {
    // Initialize the google logging.
    google::InitGoogleLogging(argv_[0]);

    #ifndef _WIN32
        google::InstallFailureSignalHandler();
    #endif

    FLAGS_log_dir = "./logs";
    FLAGS_stderrthreshold = 1;
    setvbuf( stdout, NULL, _IONBF, 0);
}

void BaseApplication::addDefaultOptions_() {
    configuration_options_description_.add_options()
        ("help", "Displays this help dialog.")

        // cluster
        ("cluster.name", ::value<string>(), "Name of the cluster this application is participating in")
        ("cluster.process_type", ::value<string>(), "The type of service the application provides on the cluster ex: combat_service, login_service")
        ("cluster.version", ::value<string>()->default_value("0.1"), "The version the application is on the cluster")
        ("cluster.address", ::value<string>()->default_value("127.0.0.1"), "The network address the application will use on the cluster")
        ("cluster.tcp_port", ::value<uint16_t>(), "The tcp port the application will use on the cluster")
        ("cluster.udp_port", ::value<uint16_t>()->default_value(0), "The udp port the application will use on the cluster")
        ("cluster.ping_port", ::value<uint16_t>()->default_value(0), "The ping port the application will use on the cluster")            
            
        // data source
        ("cluster.datastore.name", ::value<string>(), "Storage Type of the datastore")
        ("cluster.datastore.host", ::value<string>()->default_value("tcp://localhost:3306"), "Host to connect to the cluster datastore at: e.x. tcp://localhost:3306")
        ("cluster.datastore.username", ::value<string>(), "Username to connect to the cluster datastore with")
        ("cluster.datastore.password", ::value<string>(), "Password to connect to the cluster datastore with")
        ("cluster.datastore.schema", ::value<string>(), "Schema name that contains the cluster data")
        
        ("galaxy.datastore.name", ::value<string>(), "Storage Type of the datastore")
        ("galaxy.datastore.host", ::value<string>()->default_value("tcp://localhost:3306"), "Host to connect to the galaxy datastore at: e.x. tcp://localhost:3306")
        ("galaxy.datastore.username", ::value<string>(), "Username to connect to the galaxy datastore with")
        ("galaxy.datastore.password", ::value<string>(), "Password to connect to the galaxy datastore with")
        ("galaxy.datastore.schema", ::value<string>(), "Schema name that contains the galaxy data")

        // optimization
        ("optimization.min_threads", ::value<uint16_t>()->default_value(0), "Minimum number of threads to use for concurrency operations")
        ("optimization.max_threads", ::value<uint16_t>()->default_value(0), "Maximum number of threads to use for concurrency operations")

        ("modules", ::value<vector<string>>()->default_value(vector<string>(), ""), "modules to load on startup")
        ("module_api_version_major",  ::value<uint32_t>()->default_value(0), "Major Module API Version Allowed")
        ("module_api_version_minor",   ::value<uint32_t>()->default_value(1), "Minor Module API Version Allowed")
    ;
    onAddDefaultOptions_();
}

void BaseApplication::loadOptions_(uint32_t argc, char* argv[], list<string> config_files)
{
    store(parse_command_line(argc, argv, configuration_options_description_), configuration_variables_map_);
    
    // Iterate through the configuration files
    // that are to be loaded. If a configuration file
    // is missing, throw a runtime_error.

    for_each(config_files.begin(), config_files.end(), [&] (const string& filename) {
        ifstream config_file(filename);
        if(!config_file.is_open())
            throw runtime_error("Could not open configuration file: " + filename);
        else
        {
            try {
                store(parse_config_file(config_file, configuration_options_description_, true), configuration_variables_map_);
            } catch(...) { 
                throw runtime_error("Could not parse config file: " + filename);
            }
        }
    })(config_files.front());

    notify(configuration_variables_map_);
    
    // The help argument has been flagged, display the
    // server options and throw a runtime_error exception
    // to stop server startup.
    if(configuration_variables_map_.count("help")) {
        cout << configuration_options_description_ << endl;
        throw runtime_error("Help option flagged.");
    }
}
void BaseApplication::setupModules_()
{
    // get modules_version
    uint32_t major = configuration_variables_map_["module_api_version_major"].as<uint32_t>();
    uint32_t minor = configuration_variables_map_["module_api_version_minor"].as<uint32_t>();
    string str_version = major + "." + minor;
    modules_version_ = make_shared<ModuleApiVersion>(major, minor, str_version);

    module_manager_ = make_shared<ModuleManager>(platform_services_, modules_version_);
    vector<string> modules = configuration_variables_map_["modules"].as<vector<string>>();
    module_manager_->LoadModules(modules);
}
bool BaseApplication::addDataSourcesFromOptions_()
{
    // check to see if options have been loaded properly
    if (configuration_variables_map_.size() < 1)
        return false;
    if (db_manager_ == nullptr){
        throw runtime_error("Database Manager must be instantiated in order to add Data Sources.");
        return false;
    }

    try {
        // make sure the value is in the map before registering
        if (configuration_variables_map_.count("galaxy.datastore.name") > 0) {
            // register the galaxy
            db_manager_->registerStorageType(
            configuration_variables_map_["galaxy.datastore.name"].as<string>(),
            configuration_variables_map_["galaxy.datastore.schema"].as<string>(),
            configuration_variables_map_["galaxy.datastore.host"].as<string>(),
            configuration_variables_map_["galaxy.datastore.username"].as<string>(),
            configuration_variables_map_["galaxy.datastore.password"].as<string>());
        }
        // make sure the value is in the map before registering
        if (configuration_variables_map_.count("cluster.datastore.name") > 0) {
            // register the cluster
            db_manager_->registerStorageType(
            configuration_variables_map_["cluster.datastore.name"].as<string>(),
            configuration_variables_map_["cluster.datastore.schema"].as<string>(),
            configuration_variables_map_["cluster.datastore.host"].as<string>(),
            configuration_variables_map_["cluster.datastore.username"].as<string>(),
            configuration_variables_map_["cluster.datastore.password"].as<string>());
        }

    } catch(...) {
        throw runtime_error( "No exceptions should be thrown during a registration with valid information");
        return false;
    }
    return true;
}

void BaseApplication::registerApp_()
{
    // sets up the server_directory
    if (server_directory_ == nullptr) 
    {
        shared_ptr<sql::Connection> conn = db_manager_->getConnection(
            configuration_variables_map_["cluster.datastore.name"].as<string>());
        if (conn != nullptr)
        {
            server_directory_ = createServerDirectory(conn);
        }
        else {
            throw runtime_error("No valid database connection available");
        }
    }
    if (server_directory_ != nullptr)
    {
        // make sure the value is in the map before registering
        if (configuration_variables_map_.count("cluster.name") > 0) {
            server_directory_->registerProcess(
                configuration_variables_map_["cluster.name"].as<string>(),
                configuration_variables_map_["cluster.process_type"].as<string>(),
                configuration_variables_map_["cluster.version"].as<string>(),
                configuration_variables_map_["cluster.address"].as<string>(),
                configuration_variables_map_["cluster.tcp_port"].as<uint16_t>(),
                configuration_variables_map_["cluster.udp_port"].as<uint16_t>(),
                configuration_variables_map_["cluster.ping_port"].as<uint16_t>()
                );
        }
    }
}
void BaseApplication::setupCluster_() 
{
    // setup cluster_service
    // get tcp port of app...
    uint16_t port = configuration_variables_map_["cluster.tcp_port"].as<uint16_t>();
    // create a new cluster service if one wasn't passed in
    if (cluster_service_ == nullptr)
    {
        cluster_service_ = std::make_shared<ClusterService>(cluster_io_service_, server_directory_, event_dispatcher_, port);
    }
    // loop through processes in server_directory and open a tcp connection to each
    if (server_directory_ != nullptr)
    {
        try {
            auto proc_list = server_directory_->getProcessSnapshot(server_directory_->cluster());
            std::for_each(proc_list.begin(), proc_list.end(), [=](anh::server_directory::Process proc) {
                if (proc.tcp_port() != port){
                    cluster_service_->Connect(std::make_shared<anh::server_directory::Process>(proc));
                }
            });
        }
        catch(std::exception e)
        {
            LOG(WARNING) << "Error with Cluster Setup: " << e.what() <<endl;
        }
    }
}

void BaseApplication::init_services_() 
{
    try {
            getRequiredPlatformServices_();
    }
    catch(boost::bad_any_cast e)
    {
        throw runtime_error("Required Services Missing: " + string(e.what()));
    }
    // these are optional so they might not be passed in, the app can handle these values not being available.
    getOptionalPlatformServices_();
}
void BaseApplication::getPreStartupPlatformServices_()
{
    event_dispatcher_ = boost::any_cast<shared_ptr<EventDispatcherInterface>>(platform_services_->getService("EventDispatcher"));
}
void BaseApplication::getRequiredPlatformServices_()
{
    clock_ = boost::any_cast<shared_ptr<Clock>>(platform_services_->getService("Clock"));
}

void BaseApplication::getOptionalPlatformServices_()
{
    try {
        db_manager_ = boost::any_cast<shared_ptr<DatabaseManagerInterface>>(platform_services_->getService("DatabaseManager"));
    }
    catch(...) {}
    try {
        server_directory_ = boost::any_cast<shared_ptr<ServerDirectoryInterface>>(platform_services_->getService("ServerDirectory"));
    }
    catch(...) {}
    try {
        cluster_service_ = boost::any_cast<shared_ptr<ClusterServiceInterface>>(platform_services_->getService("ClusterService"));
    }
    catch(...) {}
    try {
        scripting_manager_ = boost::any_cast<shared_ptr<ScriptingManagerInterface>>(platform_services_->getService("ScriptingManager"));
    }
    catch(...) {}
}
void BaseApplication::setPlatformServices_()
{
    platform_services_->addService("ProgramOptions"
        , std::make_shared<boost::program_options::variables_map>(configuration_variables_map()));
    // these might be added already, but if they are not in here they will be added
    if (db_manager_ != nullptr)
        platform_services_->addService("DatabaseManager", db_manager_);
    if (server_directory_ != nullptr)
        platform_services_->addService("ServerDirectory", server_directory_);
    if (scripting_manager_ != nullptr)
        platform_services_->addService("ScriptingManager", scripting_manager_);
    
}
int BaseApplication::kbHit()
{
#if defined(_MSC_VER)
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
#endif
}
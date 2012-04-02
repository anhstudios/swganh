#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/app/kernel_interface.h"
#include "anh/database/database_manager_interface.h"
#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "phpbb_account_provider.h"
#include "phpbb_encoder.h"
#include "phpbb_session_provider.h"

using namespace anh::app;
using namespace anh::plugin;
using namespace plugins::phpbb_auth;
using namespace std;

using boost::any_cast;
using boost::program_options::options_description;
using boost::program_options::variables_map;

struct PhpbbConfig
{
    std::string host;
    std::string schema;
    std::string username;
    std::string password;
    std::string table_prefix;
} config;

extern "C" PLUGIN_API void ExitModule() 
{
    return;
}


extern "C" PLUGIN_API  void ConfigurePlugin(options_description& description)
{
    description.add_options()
        ("phpbb_database.host", boost::program_options::value<std::string>(&config.host),
            "Host address for the galaxy_manager datastore")
        ("phpbb_database.schema", boost::program_options::value<std::string>(&config.schema),
            "Schema name for the galaxy_manager datastore")
        ("phpbb_database.username", boost::program_options::value<std::string>(&config.username),
            "Username for authentication with the galaxy_manager datastore")
        ("phpbb_database.password", boost::program_options::value<std::string>(&config.password),
            "Password for authentication with the galaxy_manager datastore")
        ("phpbb_database.table_prefix", boost::program_options::value<std::string>(&config.table_prefix),
            "Prefix used for all table names (commonly phpbb_)")
    ;

    return;
}

extern "C" PLUGIN_API ExitFunc InitializePlugin(KernelInterface* kernel) 
{    
    kernel->GetDatabaseManager()->registerStorageType(
        "phpbb",
        config.schema,
        config.host,
        config.username,
        config.password);

    ObjectRegistration registration;
    registration.version.major = 1;
    registration.version.minor = 0;

    // Register TestObj
    registration.CreateObject = [] (ObjectParams* params) -> void * {
        return new PhpbbEncoder();
    };

    kernel->GetPluginManager()->RegisterObject("LoginService::Encoder", &registration);
    
    registration.CreateObject = [kernel] (ObjectParams* params) -> void * {
        return new PhpbbAccountProvider(kernel->GetDatabaseManager(), config.table_prefix);
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<PhpbbAccountProvider*>(object);
        }
    };
    
    kernel->GetPluginManager()->RegisterObject("LoginService::AccountProvider", &registration);
        
    registration.CreateObject = [kernel] (ObjectParams* params) -> void * {
        return new PhpbbSessionProvider(kernel->GetDatabaseManager(), config.table_prefix);
    };
    
    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<PhpbbSessionProvider*>(object);
        }
    };
    
    kernel->GetPluginManager()->RegisterObject("ConnectionService::SessionProvider", &registration);

    return ExitModule;
}
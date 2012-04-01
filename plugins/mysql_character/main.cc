#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/app/kernel_interface.h"
#include "anh/database/database_manager_interface.h"
#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "mysql_character_provider.h"


using namespace anh::app;
using namespace anh::plugin;
using namespace plugins::mysql_character;
using namespace std;

using boost::any_cast;
using boost::program_options::options_description;
using boost::program_options::variables_map;


extern "C" PLUGIN_API void ExitModule() 
{
    return;
}


extern "C" PLUGIN_API  void ConfigurePlugin(options_description& description)
{
    return;
}

extern "C" PLUGIN_API ExitFunc InitializePlugin(KernelInterface* kernel) 
{    
    ObjectRegistration registration;
    registration.version.major = 0;
    registration.version.minor = 4;

    // Register
    registration.CreateObject = [kernel] (ObjectParams* params) -> void * {
        return new MysqlCharacterProvider(kernel);
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<MysqlCharacterProvider*>(object);
        }
    };

    kernel->GetPluginManager()->RegisterObject("CharacterService::CharacterProvider", &registration);
    
    return ExitModule;
}
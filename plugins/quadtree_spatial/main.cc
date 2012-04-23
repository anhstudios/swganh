// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/thread.hpp>

#include "anh/logger.h"
#include "anh/app/kernel_interface.h"
#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "quadtree_spatial_provider.h"

using namespace anh::app;
using namespace anh::plugin;
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
        return new QuadtreeSpatialProvider(kernel);
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete static_cast<QuadtreeSpatialProvider*>(object);
        }
    };

    kernel->GetPluginManager()->RegisterObject("SimulationService::SpatialProvider", &registration);
    
    return ExitModule;
}
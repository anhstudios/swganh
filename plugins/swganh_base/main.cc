// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

using anh::app::KernelInterface;
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
    
    return ExitModule;
}
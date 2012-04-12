/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2012 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
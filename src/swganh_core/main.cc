// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "swganh/logger.h"

#include "swganh/plugin/bindings.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "attributes/attributes_init.h"
#include "chat/chat_init.h"
#include "combat/combat_init.h"
#include "character/character_init.h"
#include "connection/connection_init.h"
#include "command/command_init.h"
#include "equipment/equipment_init.h"
#include "login/login_init.h"
#include "galaxy/galaxy_init.h"
#include "simulation/simulation_init.h"
#include "player/player_init.h"
#include "social/social_init.h"
#include "equipment/equipment_init.h"
#include "spawn/spawn_init.h"
#include "sui/sui_init.h"
#include "weather/weather_init.h"
#include "terrain/terrain_init.h"

using swganh::app::KernelInterface;
using swganh::plugin::ExitFunc;
using boost::any_cast;
using boost::program_options::options_description;
using boost::program_options::variables_map;
using swganh::app::SwganhKernel;

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
    auto swganh_kernel = static_cast<SwganhKernel*>(kernel);    
    
	swganh::attributes::Initialize(swganh_kernel);
    swganh::character::Initialize(swganh_kernel);
	swganh::connection::Initialize(swganh_kernel);
    swganh::command::Initialize(swganh_kernel);
	swganh::combat::Initialize(swganh_kernel);
	swganh::equipment::Initialize(swganh_kernel);
    swganh::login::Initialize(swganh_kernel);
    swganh::galaxy::Initialize(swganh_kernel);
    swganh::simulation::Initialize(swganh_kernel);
    swganh::chat::Initialize(swganh_kernel);
	swganh::simulation::Initialize(swganh_kernel);
	swganh::player::Initialize(swganh_kernel);
	swganh::social::Initialize(swganh_kernel);
	swganh::spawn::Initialize(swganh_kernel);
	swganh::sui::Initialize(swganh_kernel);
	swganh::weather::Initialize(swganh_kernel);
	swganh::terrain::Initialize(swganh_kernel);
    return ExitModule;
}
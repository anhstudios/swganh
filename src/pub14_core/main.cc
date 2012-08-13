// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "anh/logger.h"

#include "anh/plugin/bindings.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "chat/chat_init.h"
#include "character/character_init.h"
#include "command/command_init.h"
#include "login/login_init.h"
#include "galaxy/galaxy_init.h"
#include "simulation/simulation_init.h"
#include "player/player_init.h"
#include "social/social_init.h"
#include "weather/weather_init.h"

using anh::app::KernelInterface;
using anh::plugin::ExitFunc;
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
    
    swganh_core::character::Initialize(swganh_kernel);
    pub14_core::command::Initialize(swganh_kernel);
    swganh_core::login::Initialize(swganh_kernel);
    swganh_core::galaxy::Initialize(swganh_kernel);
    swganh_core::simulation::Initialize(swganh_kernel);
    swganh_core::chat::Initialize(swganh_kernel);
	swganh_core::simulation::Initialize(swganh_kernel);
	swganh_core::player::Initialize(swganh_kernel);
	swganh_core::social::Initialize(swganh_kernel);
	swganh_core::weather::Initialize(swganh_kernel);
    return ExitModule;
}
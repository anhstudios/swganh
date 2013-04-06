// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "spawn_service.h"

#include <algorithm>
#include <memory>

#include "swganh/scripting/utilities.h"

#include "swganh/logger.h"

#include "swganh/service/service_manager.h"
#include "swganh/scripting/python_script.h"
#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/simulation/scene_events.h"

#include "swganh_core/object/permissions/permission_type.h"
#include "swganh_core/object/object_manager.h"

#include "swganh_core/object/object.h"

#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/snapshot/ws_visitor.h"
#include "swganh/tre/visitors/objects/object_visitor.h"

#include "machines/creature.h"
#include "machines/lair.h"
#include "machines/npc_factioned.h"
#include "machines/npc_neutral.h"
#include "machines/shuttle.h"

using namespace swganh::service;
using namespace swganh::app;
using namespace swganh::simulation;
using namespace swganh::object;
using namespace swganh::tre;

using namespace swganh::spawn;
using namespace swganh::simulation;
using namespace swganh::scripting;

SpawnService::SpawnService(SwganhKernel* kernel) 
	: fsm_manager_(kernel->GetEventDispatcher())
        , kernel_(kernel)
	, timer_(kernel_->GetCpuThreadPool(), boost::posix_time::seconds(60))
{
    SetServiceDescription(ServiceDescription(
        "SpawnService",
        "spawn",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0));
}

SpawnService::~SpawnService()
{
	timer_.cancel();
}

void SpawnService::Initialize()
{}

void SpawnService::Startup()
{
	//Load some defaults from python
	uint32_t SHUTTLE_AWAY_TIME_SECONDS = 300;
	uint32_t SHUTTLE_IN_PORT_TIME_SECONDS = 300;

    PythonScript script(kernel_->GetAppConfig().script_directory + "/spawn_config.py");

    SHUTTLE_AWAY_TIME_SECONDS = script.GetGlobalAs<uint32_t>("SHUTTLE_AWAY_TIME_SECONDS");
    SHUTTLE_IN_PORT_TIME_SECONDS = script.GetGlobalAs<uint32_t>("SHUTTLE_IN_PORT_TIME_SECONDS");

	//Build the default machines
	//_buildCreatureMachine(kernel_, fsm_manager_);
	//_buildLairMachine(kernel_, fsm_manager_);
	//_buildNpcFactionedMachine(kernel_, fsm_manager_);
	//_buildNpcNeutralMachine(kernel_, fsm_manager_);
	_buildShuttleMachine(kernel_, fsm_manager_, SHUTTLE_AWAY_TIME_SECONDS, SHUTTLE_IN_PORT_TIME_SECONDS);

	timer_.expires_from_now(boost::posix_time::seconds(60));
	timer_.async_wait(std::bind(&SpawnService::_timerTick, this, std::placeholders::_1));
}

void SpawnService::_timerTick(const boost::system::error_code& e)
{
	//For each spawn group
		//If the spawn group can handle more spawns
			//Trigger Spawn

	timer_.expires_from_now(boost::posix_time::seconds(60));
	timer_.async_wait(std::bind(&SpawnService::_timerTick, this, std::placeholders::_1));
}
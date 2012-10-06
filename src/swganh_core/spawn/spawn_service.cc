// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#include "spawn_service.h"

#include <algorithm>
#include <memory>

#include <boost/python.hpp>
#include "swganh/scripting/utilities.h"

#include "swganh/logger.h"

#include "swganh/service/service_manager.h"

#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/simulation/scene_events.h"

#include "swganh/object/permissions/permission_type.h"
#include "swganh_core/object/object_manager.h"
#include "swganh_core/object/object.h"

#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/snapshot/ws_visitor.h"
#include "swganh/tre/visitors/objects/object_visitor.h"

using namespace swganh::service;
using namespace swganh::app;
using namespace swganh::simulation;
using namespace swganh::object;
using namespace swganh::tre;

using namespace swganh::spawn;
using namespace swganh::simulation;
using namespace swganh::scripting;

SpawnService::SpawnService(SwganhKernel* kernel) 
	: kernel_(kernel)
{
}

SpawnService::~SpawnService()
{
}

ServiceDescription SpawnService::GetServiceDescription()
{
    ServiceDescription service_description(
        "SpawnService",
        "spawn",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}

void SpawnService::Startup()
{
	//Load some defaults from python
	uint32_t SHUTTLE_AWAY_TIME_SECONDS = 60;
	uint32_t SHUTTLE_IN_PORT_TIME_SECONDS = 60;

	ScopedGilLock lock;
    try 
    {
        auto config = boost::python::import("spawn_config");
		GetValue(config, "SHUTTLE_AWAY_TIME_SECONDS", SHUTTLE_AWAY_TIME_SECONDS);
		GetValue(config, "SHUTTLE_IN_PORT_TIME_SECONDS", SHUTTLE_IN_PORT_TIME_SECONDS);
    }
    catch(boost::python::error_already_set& /*e*/)
    {
        PyErr_Print();
    }

	//Build the default machines

}
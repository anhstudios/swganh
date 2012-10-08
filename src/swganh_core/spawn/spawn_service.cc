// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "spawn_service.h"

#include <algorithm>
#include <memory>

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

SpawnService::SpawnService(SwganhKernel* kernel) : kernel_(kernel)
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

PermissionType SpawnService::FindProperPermission_(const std::string& iff_name)
{
	if(iff_name.compare("object/cell/shared_cell.iff") == 0)
	{
		return WORLD_CELL_PERMISSION;
	}
	return STATIC_CONTAINER_PERMISSION;
}
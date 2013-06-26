// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "spawn_service.h"

#include <algorithm>
#include <memory>

#include <boost/algorithm/string.hpp>
#include <boost/python.hpp>

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

#include "machines/shuttle.h"
#include "loot_group.h"
#include "spawn_region.h"

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
, next_region_id_(0)
, kernel_(kernel)
, timer_(kernel->GetCpuThreadPool(), boost::posix_time::seconds(60))
, active_(kernel->GetCpuThreadPool())
{
	//Static Objects
	kernel_->GetEventDispatcher()->Subscribe("SceneManager:NewScene", [&] (const std::shared_ptr<swganh::EventInterface>& newEvent)
	{
        active_.Async([=] () {
            auto real_event = std::static_pointer_cast<swganh::simulation::NewSceneEvent>(newEvent);

			LOG(warning) << "Loading spawn data for: " << real_event->scene_label;
			
			ScopedGilLock lock;
			try {
				std::string module_name = "spawn."+real_event->scene_label;
				auto config = boost::python::import(module_name.c_str());

				config.attr("loadRegions")(boost::python::ptr(kernel_));
			} catch(...) {
				PyErr_Print();
			}
		});
	});

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
	std::lock_guard<std::mutex> lock_(region_lock_); 
	for(auto& region : spawn_regions_)
	{
		region.second->SpawnTick(kernel_);
	}

	timer_.expires_from_now(boost::posix_time::seconds(60));
	timer_.async_wait(std::bind(&SpawnService::_timerTick, this, std::placeholders::_1));
}

void SpawnService::StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::wstring machine)
{
	fsm_manager_.StartManagingObject(object, machine);
}

void SpawnService::StopManagingObject(std::shared_ptr<swganh::object::Object> object)
{
	fsm_manager_.StopManagingObject(object);
}

void SpawnService::AddLootGroup(std::string name, std::shared_ptr<LootGroup> group)
{
	std::lock_guard<std::mutex> lock_(region_lock_);
	loot_groups_.insert(std::make_pair(name, group));
}

std::shared_ptr<LootGroup> SpawnService::GetLootGroup(std::string name)
{
	//Split into module name and real name
	std::string module, real_name;
	
	size_t index = name.find_last_of(".");
	if(index == std::string::npos)
	{
		real_name = name;
	}
	else
	{
		module = name.substr(0, index);
		real_name = name.substr(index+1, name.size()-index);
	}

	//Try and grab a copy in memory
	{
		std::lock_guard<std::mutex> lock_(region_lock_);
		auto find_itr = loot_groups_.find(name);
		if(find_itr != loot_groups_.end())
		{
			return find_itr->second;
		}
	}

	//Attempt to load from python
	if(module.size() > 0)
	{
		ScopedGilLock lock;
		try 
		{
			auto config = boost::python::import(name.c_str());
			config.attr("loadLoot")(boost::python::ptr(kernel_));
		}
		catch(boost::python::error_already_set& /*e*/)
		{
			PyErr_Print();
		}
	}
	else
	{
		//No need to reattempt since we can't load anything.
		return nullptr;
	}

	//Retry now that we've loaded the module
	{
		std::lock_guard<std::mutex> lock_(region_lock_);
		auto find_itr = loot_groups_.find(name);
		if(find_itr != loot_groups_.end())
		{
			return find_itr->second;
		}
	}

	//Fail
	return nullptr;
}

void SpawnService::AddSpawnRegion(uint32_t scene_id, std::shared_ptr<SpawnRegion> region)
{
	std::lock_guard<std::mutex> lock_(region_lock_);
	region->SetSceneId(scene_id);
	spawn_regions_.insert(std::make_pair(next_region_id_++, region));
}
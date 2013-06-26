// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/utilities.h"

#include "swganh_core/object/object.h"

#include "swganh/app/swganh_kernel.h"

#include "spawn_service_interface.h"
#include "loot_item.h"
#include "loot_group.h"
#include "spawn_region.h"
#include "circle_spawn_region.h"
#include "rectangle_spawn_region.h"
#include "fixed_gcw_spawn_region.h"



using namespace swganh::spawn;
namespace bp = boost::python;
using namespace std;
using namespace swganh::object;

shared_ptr<LootItem> makeLootItem(float drop_percent, bp::object creationFunctor)
{
	return std::make_shared<LootItem>(drop_percent, [&creationFunctor] (swganh::app::SwganhKernel* kernel) -> std::shared_ptr<Object> {
		swganh::scripting::ScopedGilLock lock;
		try {
			return bp::extract<std::shared_ptr<Object>>(creationFunctor(bp::ptr(kernel)));
		} catch(...) {
			PyErr_Print();
			return nullptr;
		}
	});
}

std::shared_ptr<LootGroup> makeLootGroup(uint32_t max_items, uint32_t min_credits, uint32_t max_credits)
{
	return std::make_shared<LootGroup>(max_items, min_credits, max_credits);
}

shared_ptr<CircleSpawnRegion> makeCircleSpawn(std::shared_ptr<swganh::terrain::TerrainServiceInterface> terrain, 
	std::shared_ptr<LootGroup> loot_group, bp::object creationFunctor, uint32_t density, uint32_t timeout, float rarity,
	float x, float y, float radius)
{
	return std::make_shared<CircleSpawnRegion>(terrain, loot_group, density, timeout, rarity,  x, y, radius, [&creationFunctor] (swganh::app::SwganhKernel* kernel) -> std::shared_ptr<Object> {
		swganh::scripting::ScopedGilLock lock;
		try {
			return bp::extract<std::shared_ptr<Object>>(creationFunctor(bp::ptr(kernel)));
		} catch(...) {
			PyErr_Print();
			return nullptr;
		}
	});
}

shared_ptr<RectangleSpawnRegion> makeRectangleSpawn(std::shared_ptr<swganh::terrain::TerrainServiceInterface> terrain, 
	std::shared_ptr<LootGroup> loot_group, bp::object creationFunctor, uint32_t density, uint32_t timeout, float rarity, 
	float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
	return std::make_shared<RectangleSpawnRegion>(terrain, loot_group, density, timeout, rarity, topLeftX, topLeftY, bottomRightX, bottomRightY, [&creationFunctor] (swganh::app::SwganhKernel* kernel) -> std::shared_ptr<Object> {
		swganh::scripting::ScopedGilLock lock;
		try {
			return bp::extract<std::shared_ptr<Object>>(creationFunctor(bp::ptr(kernel)));
		} catch(...) {
			PyErr_Print();
			return nullptr;
		}
	});
}

void exportSpawnService()
{
    bp::class_<SpawnServiceInterface, shared_ptr<SpawnServiceInterface>, boost::noncopyable>("SpawnService", bp::no_init)
		.def("startManagingObject", &SpawnServiceInterface::StartManagingObject, "Has the machine with the given name manage the given object.")
		.def("stopManagingObject", &SpawnServiceInterface::StopManagingObject, "Has the AI sub system stop managing the given object.")
		.def("add", &SpawnServiceInterface::AddLootGroup, "Adds the given LootGroup to the SpawnService.")
		.def("getLootGroup", &SpawnServiceInterface::GetLootGroup, "Returns the LootGroup with the given name.")
		.def("add", &SpawnServiceInterface::AddSpawnRegion, "Adds the given spawn region to the SpawnService.")
		;

	bp::class_<LootItem, shared_ptr<LootItem>, boost::noncopyable>("LootItem", bp::no_init)
		.def("__init__", bp::make_constructor(makeLootItem))
		;

	bp::class_<LootGroup, shared_ptr<LootGroup>, boost::noncopyable>("LootGroup", bp::no_init)
		.def("__init__", bp::make_constructor(makeLootGroup))
		.def("add", &LootGroup::AddItem)
		;

	bp::class_<SpawnRegion, shared_ptr<SpawnRegion>, boost::noncopyable>("SpawnRegion", bp::no_init)
		;

	bp::class_<CircleSpawnRegion, bp::bases<SpawnRegion>, shared_ptr<CircleSpawnRegion>, boost::noncopyable>("CircleSpawnRegion", bp::no_init)
		.def("__init__", bp::make_constructor(makeCircleSpawn))
		;

	bp::class_<RectangleSpawnRegion, bp::bases<SpawnRegion>, shared_ptr<RectangleSpawnRegion>, boost::noncopyable>("RectangleSpawnRegion", bp::no_init)
		.def("__init__", bp::make_constructor(makeRectangleSpawn))
		;

	bp::class_<FixedGCWSpawnRegion, bp::bases<SpawnRegion>, shared_ptr<FixedGCWSpawnRegion>, boost::noncopyable>("FixedGCWSpawnRegion", bp::no_init)
		;

	bp::implicitly_convertible<std::shared_ptr<CircleSpawnRegion>, std::shared_ptr<SpawnRegion>>();
	bp::implicitly_convertible<std::shared_ptr<RectangleSpawnRegion>, std::shared_ptr<SpawnRegion>>();
	bp::implicitly_convertible<std::shared_ptr<FixedGCWSpawnRegion>, std::shared_ptr<SpawnRegion>>();
}

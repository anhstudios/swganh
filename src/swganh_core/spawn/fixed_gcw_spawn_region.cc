// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "fixed_gcw_spawn_region.h"

using namespace swganh::spawn;

FixedGCWSpawnRegion::FixedGCWSpawnRegion(std::shared_ptr<LootGroup> loot_group, 
	SpawnFactory typeRebel, SpawnFactory typeImperial)
	: SpawnRegion(loot_group, 0, 0, 1.0f)
	, rebel_factory_(typeRebel)
	, imp_factory_(typeImperial)
{
}
	
std::shared_ptr<swganh::object::Object> FixedGCWSpawnRegion::Spawn(swganh::app::SwganhKernel* kernel)
{
	return rebel_factory_(kernel);
}
	
void FixedGCWSpawnRegion::AddSpawnPoint(glm::vec3 spawn_point)
{
	
}
	
bool FixedGCWSpawnRegion::NeedsSpawn(swganh::app::SwganhKernel* kernel)
{
	return false;
}
	
glm::vec3 FixedGCWSpawnRegion::NextSpawnLocation(swganh::app::SwganhKernel* kernel)
{
	return glm::vec3(0, 0, 0);
}
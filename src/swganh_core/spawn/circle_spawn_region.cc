// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "circle_spawn_region.h"

using namespace swganh::spawn;

CircleSpawnRegion::CircleSpawnRegion(std::shared_ptr<swganh::terrain::TerrainServiceInterface> terrain, 
		std::shared_ptr<LootGroup> loot_group, uint32_t density, uint32_t timeout, float rarity,
		float x, float y, float radius, SpawnFactory type)
	: SpawnRegion(loot_group, density, timeout, rarity)
	, factory_(type)
	, center_(x, y)
{
}
	
std::shared_ptr<swganh::object::Object> CircleSpawnRegion::Spawn(swganh::app::SwganhKernel* kernel)
{
	return factory_(kernel);
}
	
bool CircleSpawnRegion::NeedsSpawn(swganh::app::SwganhKernel* kernel)
{
	return false;
}
	
glm::vec3 CircleSpawnRegion::NextSpawnLocation(swganh::app::SwganhKernel* kernel)
{
	return glm::vec3(0, 0, 0);
}
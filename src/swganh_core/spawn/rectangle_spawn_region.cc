// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "rectangle_spawn_region.h"

using namespace swganh::spawn;

RectangleSpawnRegion::RectangleSpawnRegion(std::shared_ptr<swganh::terrain::TerrainServiceInterface> terrain, 
		std::shared_ptr<LootGroup> loot_group, 
		uint32_t density, uint32_t timeout, float rarity,
		float topLeftX, float topLeftY, float bottomRightX, float bottomRightY, SpawnFactory type)
		: SpawnRegion(loot_group, density, timeout, rarity)
		, topLeft(topLeftX, topLeftY)
        , bottomRight(bottomRightX, bottomRightY)
        , factory_(type)
{
}
	
std::shared_ptr<swganh::object::Object> RectangleSpawnRegion::Spawn(swganh::app::SwganhKernel* kernel)
{
	return factory_(kernel);
}
	
bool RectangleSpawnRegion::NeedsSpawn(swganh::app::SwganhKernel* kernel)
{
	return false;
}
	
glm::vec3 RectangleSpawnRegion::NextSpawnLocation(swganh::app::SwganhKernel* kernel)
{
	return glm::vec3(0, 0, 0);
}
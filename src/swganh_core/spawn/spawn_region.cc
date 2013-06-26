// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "spawn_region.h"

using namespace swganh::spawn;

SpawnRegion::SpawnRegion(std::shared_ptr<LootGroup> loot_group, uint32_t density, uint32_t timeout, float rarity)
	 /*loot_group_(loot_group)
	, density_(density)
	, timeout_(timeout)
	, rarity_(rarity)*/
{
}

void SpawnRegion::SetSceneId(uint32_t scene_id)
{
	scene_id_ = scene_id;
}

void SpawnRegion::SpawnTick(swganh::app::SwganhKernel* kernel)
{
	//Despawn any mobs that need it.

	//Spawn more mobs if needed.
}
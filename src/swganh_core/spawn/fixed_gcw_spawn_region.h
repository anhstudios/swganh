// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "spawn_region.h"

namespace swganh
{
namespace spawn
{

class FixedGCWSpawnRegion : public SpawnRegion
{
public:
	
	FixedGCWSpawnRegion(std::shared_ptr<LootGroup> loot_group, SpawnFactory typeRebel, SpawnFactory typeImperial);
	
	std::shared_ptr<swganh::object::Object> Spawn(swganh::app::SwganhKernel* kernel);
	
	void AddSpawnPoint(glm::vec3 spawn_point);
	
	bool NeedsSpawn(swganh::app::SwganhKernel* kernel);
	
	glm::vec3 NextSpawnLocation(swganh::app::SwganhKernel* kernel);
	
private:
	SpawnFactory rebel_factory_, imp_factory_;
};

}}
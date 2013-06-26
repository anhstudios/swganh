// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <functional>
#include <memory>
#include <glm/glm.hpp>

namespace swganh
{
namespace app
{
	class SwganhKernel;
}

namespace object
{
	class Object;
}

namespace spawn
{

typedef std::function<std::shared_ptr<swganh::object::Object>(swganh::app::SwganhKernel* kernel)> SpawnFactory;

class LootGroup;

class SpawnRegion
{
public:

	SpawnRegion(std::shared_ptr<LootGroup> loot_group, uint32_t density, uint32_t timeout, float rarity);

	virtual ~SpawnRegion() {}
	
	void SetSceneId(uint32_t scene_id);

	/**
	* @brief Called when this region is expected to perform new spawns.
	*/
	void SpawnTick(swganh::app::SwganhKernel* kernel);

	virtual std::shared_ptr<swganh::object::Object> Spawn(swganh::app::SwganhKernel* kernel) = 0;
	
	virtual bool NeedsSpawn(swganh::app::SwganhKernel* kernel) = 0;
	
	virtual glm::vec3 NextSpawnLocation(swganh::app::SwganhKernel* kernel) = 0;

private:
	uint32_t scene_id_;
    /*
    uint32_t density_, timeout_;
	float rarity_;
	std::shared_ptr<LootGroup> loot_group_;
    */
};

}}
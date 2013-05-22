// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/service/service_interface.h>

namespace swganh
{
namespace object
{
	class Object;
}

namespace spawn
{
	class LootGroup;
	class SpawnRegion;

	class SpawnServiceInterface : public swganh::service::BaseService
	{
	public:
        virtual ~SpawnServiceInterface() {}

		virtual void StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::wstring machine) = 0;
		virtual void StopManagingObject(std::shared_ptr<swganh::object::Object> object) = 0;

		virtual void AddLootGroup(std::string name, std::shared_ptr<LootGroup> group) = 0;
		virtual std::shared_ptr<LootGroup> GetLootGroup(std::string name) = 0;

		virtual void AddSpawnRegion(uint32_t scene_id, std::shared_ptr<SpawnRegion> region) = 0;
	};
}
}

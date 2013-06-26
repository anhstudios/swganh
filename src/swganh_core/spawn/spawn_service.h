// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/asio/deadline_timer.hpp>

#include "swganh/active_object.h"
#include "swganh_core/spawn/spawn_service_interface.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/permissions/permission_type.h"
#include "swganh_core/spawn/fsm_manager.h"

namespace swganh
{
namespace spawn
{

	class SpawnService : public swganh::spawn::SpawnServiceInterface
	{
	public:
		SpawnService(swganh::app::SwganhKernel* kernel);
		
		virtual ~SpawnService();
	
		void StartManagingObject(std::shared_ptr<swganh::object::Object> object, std::wstring machine);

		void StopManagingObject(std::shared_ptr<swganh::object::Object> object);

		void AddLootGroup(std::string name, std::shared_ptr<LootGroup> group);

		std::shared_ptr<LootGroup> GetLootGroup(std::string name);

		void AddSpawnRegion(uint32_t scene_id, std::shared_ptr<SpawnRegion> region);

		void Startup();

	private:
		void _timerTick(const boost::system::error_code& e);

		FsmManager fsm_manager_;

		std::mutex region_lock_;
		std::map<std::string, std::shared_ptr<LootGroup>> loot_groups_;
		std::map<uint32_t, std::shared_ptr<SpawnRegion>> spawn_regions_;

		uint32_t next_region_id_;
		swganh::app::SwganhKernel* kernel_;

		boost::asio::deadline_timer timer_;
		swganh::ActiveObject active_;
	};
}
}
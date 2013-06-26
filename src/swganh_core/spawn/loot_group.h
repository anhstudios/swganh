// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <set>
#include <vector>
#include "swganh/random_generator.h"

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

class LootItem;

class LootGroup
{
public:

	LootGroup(uint32_t max_items, uint32_t min_credits, uint32_t max_credits);
	
	void AddItem(std::shared_ptr<LootItem> item);
	
	uint32_t GetId();
	void SetId(uint32_t id);
	
	std::set<std::shared_ptr<swganh::object::Object>> GetDrops(swganh::app::SwganhKernel* kernel);
	
	uint32_t GetCreditPayout();

private:
	std::vector<std::shared_ptr<LootItem>> loot_items_;
	
	uint32_t id_;
	uint32_t max_items_;
	RandomGenerator random_gen_;
};

}}
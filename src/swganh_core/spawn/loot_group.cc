// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "loot_group.h"
#include "loot_item.h"

using namespace swganh::spawn;

LootGroup::LootGroup(uint32_t max_items, uint32_t min_credits, uint32_t max_credits)
	: max_items_(max_items)
	, random_gen_(min_credits, max_credits)
{
}
	
void LootGroup::AddItem(std::shared_ptr<LootItem> item)
{
	loot_items_.push_back(item);
}
	
uint32_t LootGroup::GetId()
{
	return id_;
}

void LootGroup::SetId(uint32_t id)
{
	id_ = id;
}
	
std::set<std::shared_ptr<swganh::object::Object>> LootGroup::GetDrops(swganh::app::SwganhKernel* kernel)
{
	std::set<std::shared_ptr<swganh::object::Object>> result;
	for(auto& item : loot_items_)
	{
		if(result.size() == max_items_)
		{
			break;
		}
		else
		{
			if(auto new_item = item->AttemptDrop(kernel, random_gen_.Randf()))
			{
				result.insert(new_item);
			}
		}
	}
	return result;
}
	
uint32_t LootGroup::GetCreditPayout()
{
	return random_gen_.Rand();
}
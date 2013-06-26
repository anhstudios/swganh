// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "loot_item.h"

using namespace swganh::spawn;

LootItem::LootItem(float drop_percent, LootFactory factory)
	: factory_(factory)
{
}

std::shared_ptr<swganh::object::Object> LootItem::AttemptDrop(swganh::app::SwganhKernel* kernel, float attempt)
{
	return factory_(kernel);
}
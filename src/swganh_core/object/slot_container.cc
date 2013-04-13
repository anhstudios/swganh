// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE


#include "slot_container.h"
#include "object.h"

using namespace swganh::object;

std::shared_ptr<swganh::object::Object> SlotContainer::insert_object(const std::shared_ptr<swganh::object::Object> insertObject)
{
	if (std::find(std::begin(held_objects_), std::end(held_objects_), insertObject) == std::end(held_objects_))
	{
		held_objects_.push_back(insertObject);
	}

	return nullptr;
}

void SlotContainer::remove_object(const std::shared_ptr<swganh::object::Object> removeObject)
{
	held_objects_.erase(std::remove_if(std::begin(held_objects_), std::end(held_objects_),
		[&removeObject] (const std::shared_ptr<Object>& compare) { return removeObject == compare; }),
		std::end(held_objects_));
}

void SlotContainer::view_objects(std::function<void(const std::shared_ptr<swganh::object::Object>&)> walkerFunction)
{
	for (auto& object : held_objects_)
	{
		walkerFunction(object);
	}
}
void SlotContainer::view_objects_if(std::function<bool(std::shared_ptr<swganh::object::Object>)> walkerFunction)
{
	for(auto& object : held_objects_)
	{		
		if (walkerFunction(object))
			return;
	}
}
#include "slot_container.h"
#include "object.h"

using namespace swganh::object;

std::shared_ptr<swganh::object::Object> SlotContainer::insert_object(const std::shared_ptr<swganh::object::Object> insertObject)
{
	held_objects_.insert(insertObject);
	return nullptr;
}

void SlotContainer::remove_object(const std::shared_ptr<swganh::object::Object> removeObject)
{
	auto itr = held_objects_.find(removeObject);
	if(itr != held_objects_.end())
	{
		held_objects_.erase(itr);
	}
}

void SlotContainer::view_objects(std::function<void(const std::shared_ptr<swganh::object::Object>&)> walkerFunction)
{
	for_each(held_objects_.begin(), held_objects_.end(), walkerFunction);
}
void SlotContainer::view_objects_if(std::function<bool(std::shared_ptr<swganh::object::Object>)> walkerFunction)
{
	for(auto& v : held_objects_)
	{		
		if (walkerFunction(v))
			return;
	}
}
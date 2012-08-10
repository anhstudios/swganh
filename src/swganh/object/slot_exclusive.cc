#include "slot_exclusive.h"
#include "object.h"

using namespace swganh::object;

std::shared_ptr<swganh::object::Object> SlotExclusive::insert_object(const std::shared_ptr<swganh::object::Object> insertObject)
{
	std::shared_ptr<swganh::object::Object> result = held_object_;
	held_object_ = insertObject;
	return result;
}

void SlotExclusive::remove_object(const std::shared_ptr<swganh::object::Object> removeObject)
{
	if(held_object_ == removeObject) 
	{
		held_object_ = nullptr;
	}
}

void SlotExclusive::view_objects(std::function<void(const std::shared_ptr<swganh::object::Object>&)> walkerFunction)
{
	if (held_object_ != nullptr)
		walkerFunction(held_object_);
}

void SlotExclusive::view_objects_if(std::function<bool(std::shared_ptr<swganh::object::Object>)> walkerFunction)
{
	if (held_object_ != nullptr)
	walkerFunction(held_object_);
}
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

namespace equipment
{
	class EquipmentServiceInterface : public swganh::service::ServiceInterface
	{
	public:

		virtual int32_t GetSlotIdByName(std::string slot_name) = 0;
		virtual std::string GetSlotNameById(int32_t slot_id) = 0;

		virtual void ClearSlot(std::shared_ptr<swganh::object::Object> object, std::string slot_name) = 0;

		template<typename T>
		std::shared_ptr<T> GetEquippedObject(
			std::shared_ptr<swganh::object::Object> object, std::string slot_name)
		{
			auto found = GetEquippedObject(object, slot_name);
#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(found);
#else
            return std::static_pointer_cast<T>(found);
#endif
		}
		
		virtual std::shared_ptr<swganh::object::Object> GetEquippedObject(
			std::shared_ptr<swganh::object::Object> object, std::string slot_name) = 0;
	};
}
}

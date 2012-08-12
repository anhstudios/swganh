#ifndef SWGANH_EQUIPMENT_SERVICE_INTERFACE_H_
#define SWGANH_EQUIPMENT_SERVICE_INTERFACE_H_

#include <anh/service/service_interface.h>

namespace swganh
{
namespace object
{
	class Object;
}

namespace equipment
{
	class EquipmentServiceInterface : public anh::service::ServiceInterface
	{
	public:

		virtual int32_t GetSlotIdByName(std::string slot_name) = 0;
		virtual std::string GetSlotNameById(int32_t slot_id) = 0;

		virtual void ClearSlot(std::shared_ptr<swganh::object::Object> object, std::string slot_name) = 0;
		
		virtual std::shared_ptr<swganh::object::Object> GetEquippedObject(
			std::shared_ptr<swganh::object::Object> object, std::string slot_name) = 0;
	};
}
}

#endif
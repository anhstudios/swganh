#pragma once

#include <swganh/equipment/equipment_service_interface.h>

namespace swganh
{
namespace tre
{
	class ResourceManager;
	class SlotDefinitionVisitor;
}
}

namespace swganh
{
namespace equipment
{
	class EquipmentService : public swganh::equipment::EquipmentServiceInterface
	{
	public:
		EquipmentService(swganh::tre::ResourceManager* resourceManager);

		swganh::service::ServiceDescription GetServiceDescription();

		virtual int32_t GetSlotIdByName(std::string slot_name);
		virtual std::string GetSlotNameById(int32_t slot_id);

		virtual void ClearSlot(std::shared_ptr<swganh::object::Object> object, std::string slot_name);
		
		virtual std::shared_ptr<swganh::object::Object> GetEquippedObject(
			std::shared_ptr<swganh::object::Object> object, std::string slot_name);

	private:
		std::shared_ptr<swganh::tre::SlotDefinitionVisitor> slot_definitions_;
	};
}
}
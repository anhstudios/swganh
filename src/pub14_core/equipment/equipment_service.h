#ifndef PUB14_CORE_EQUIPMENT_SERVICE_H_
#define PUB14_CORE_EQUIPMENT_SERVICE_H_

#include <swganh/equipment/equipment_service_interface.h>

#include <anh/resource/resource_manager_interface.h>

namespace swganh
{
namespace tre
{
	class SlotDefinitionVisitor;
}
}

namespace swganh_core
{
namespace equipment
{
	class EquipmentService : public swganh::equipment::EquipmentServiceInterface
	{
	public:
		EquipmentService(anh::resource::ResourceManagerInterface* resourceManager);

		anh::service::ServiceDescription GetServiceDescription();

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
#endif
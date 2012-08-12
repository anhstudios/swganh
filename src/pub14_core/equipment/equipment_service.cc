#include "equipment_service.h"

#include <memory>
#include <swganh/tre/visitors/slots/slot_definition_visitor.h>
#include <swganh/object/object.h>

using namespace anh::service;
using namespace swganh::object;
using namespace swganh::tre;
using namespace swganh_core::equipment;

EquipmentService::EquipmentService(anh::resource::ResourceManagerInterface* resourceManager)
{
	auto slot_definition = resourceManager->getResourceByName("abstract/slot/slot_definition/slot_definitions.iff", SLOT_DEFINITION_VISITOR);
	slot_definitions_ = std::static_pointer_cast<SlotDefinitionVisitor>(slot_definition);	
}

anh::service::ServiceDescription EquipmentService::GetServiceDescription()
{
	ServiceDescription service_description(        
		"Equipment Service",
        "equipment",
        "0.1",
        "127.0.0.1", 
        0,
        0, 
        0);
	return service_description;
}

int32_t EquipmentService::GetSlotIdByName(std::string slot_name)
{
	return slot_definitions_->findSlotByName(slot_name);
}

std::string EquipmentService::GetSlotNameById(int32_t slot_id)
{
	return slot_definitions_->entry(slot_id).name;
}

void EquipmentService::ClearSlot(std::shared_ptr<Object> object, std::string slot_name)
{
	//object->ClearSlot(slot_definitions_->findSlotByName(slot_name));
}
		
std::shared_ptr<Object> EquipmentService::GetEquippedObject(std::shared_ptr<Object> object, std::string slot_name)
{
	return nullptr; //object->GetEquippedObject(slot_definitions_->findSlotByName(slot_name));
}
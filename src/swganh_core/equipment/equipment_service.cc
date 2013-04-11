// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "equipment_service.h"

#include <memory>
#include "swganh/logger.h"
#include <swganh/tre/resource_manager.h>
#include <swganh/tre/visitors/slots/slot_definition_visitor.h>
#include "swganh_core/object/object.h"

using namespace swganh::service;
using namespace swganh::object;
using namespace swganh::tre;
using namespace swganh::equipment;

EquipmentService::EquipmentService(swganh::tre::ResourceManager* resourceManager)
{
	slot_definitions_ = resourceManager->GetResourceByName<SlotDefinitionVisitor>("abstract/slot/slot_definition/slot_definitions.iff");  

    SetServiceDescription(ServiceDescription(        
		"Equipment Service",
        "equipment",
        "0.1",
        "127.0.0.1", 
        0,
        0, 
        0));
}

EquipmentService::~EquipmentService()
{}

void EquipmentService::Initialize()
{}

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
	if (!object->ClearSlot(slot_definitions_->findSlotByName(slot_name)))
    {
		DLOG(warning) << "Could not find slot with name " << slot_name << " in object " << object->GetObjectId();
    }
}
		
std::shared_ptr<Object> EquipmentService::GetEquippedObject(std::shared_ptr<Object> object, std::string slot_name)
{
	return object->GetSlotObject(slot_definitions_->findSlotByName(slot_name));
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object_manager.h"

#include "anh/logger.h"

#include "object_factory.h"
#include "anh/event_dispatcher.h"
#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/objects/object_visitor.h"
#include "swganh/tre/visitors/slots/slot_arrangement_visitor.h"
#include "swganh/tre/visitors/slots/slot_descriptor_visitor.h"
#include "swganh/object/slot_exclusive.h"
#include "swganh/object/slot_container.h"

#include "anh/database/database_manager_interface.h"
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "permissions/creature_permissions.h"
#include "permissions/creature_container_permissions.h"
#include "permissions/ridable_permissions.h"

using namespace std;
using namespace anh;
using namespace swganh::tre;
using namespace swganh::object;
using namespace swganh::messages;

ObjectManager::ObjectManager(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel), next_dynamic_id_(17596481011712) //Dynamic id start value
{
	//Load Permissions
	permissions_objects_.insert(std::make_pair<int, std::shared_ptr<ContainerPermissionsInterface>>(static_cast<int>(DEFAULT_CONTAINER_PERMISSION), 
		shared_ptr<ContainerPermissionsInterface>(new DefaultContainerPermissions())));

	permissions_objects_.insert(std::make_pair<int, std::shared_ptr<ContainerPermissionsInterface>>(static_cast<int>(WORLD_CONTAINER_PERMISSION), 
		shared_ptr<ContainerPermissionsInterface>(new WorldContainerPermissions())));

	permissions_objects_.insert(std::make_pair<int, std::shared_ptr<ContainerPermissionsInterface>>(static_cast<int>(CREATURE_CONTAINER_PERMISSION), 
		shared_ptr<ContainerPermissionsInterface>(new CreaturePermissions())));

	permissions_objects_.insert(std::make_pair<int, std::shared_ptr<ContainerPermissionsInterface>>(static_cast<int>(CREATURE_CONTAINER_CONTAINER_PERMISSION), 
		shared_ptr<ContainerPermissionsInterface>(new CreatureContainerPermissions())));

	permissions_objects_.insert(std::make_pair<int, std::shared_ptr<ContainerPermissionsInterface>>(static_cast<int>(RIDEABLE_CONTAINER_PERMISSION), 
		shared_ptr<ContainerPermissionsInterface>(new RideablePermissions())));

	//Load slot definitions
	auto slot_definition = kernel->GetResourceManager()->getResourceByName("abstract/slot/slot_definition/slot_definitions.iff", SLOT_DEFINITION_VISITOR);
	slot_definition_ = static_pointer_cast<SlotDefinitionVisitor>(slot_definition);	

	//Load Object Templates
	auto conn = kernel->GetDatabaseManager()->getConnection("galaxy");
    auto statement = shared_ptr<sql::Statement>(conn->createStatement());
    statement->execute("SELECT i.iff_template, i.object_type FROM iff_templates i WHERE i.object_type != 0;");
	auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());

	while(result->next())
	{
		std::string key = result->getString("iff_template");
		uint32_t value = result->getUInt("object_type");
		type_lookup_.insert(make_pair(key, value));
	}

}

ObjectManager::~ObjectManager()
{}

void ObjectManager::RegisterObjectType(uint32_t object_type, const shared_ptr<ObjectFactoryInterface>& factory)
{
    auto find_iter = factories_.find(object_type);

    if (find_iter != factories_.end())
    {
        throw InvalidObjectType("A factory for the specified object type already exists.");
    }

    factories_.insert(make_pair(object_type, factory));
}

void ObjectManager::UnregisterObjectType(uint32_t object_type)
{
    auto find_iter = factories_.find(object_type);

    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot remove a factory for an object type that has not been registered.");
    }

    factories_.erase(find_iter);
}
void ObjectManager::RegisterMessageBuilder(uint32_t object_type, std::shared_ptr<ObjectMessageBuilder> message_builder)
{
    auto find_iter = message_builders_.find(object_type);
    if (find_iter != end(message_builders_))
        throw InvalidObjectType("A message builder for the specified type already exists.");
    
    message_builders_.insert(make_pair(object_type, message_builder));
}

shared_ptr<Object> ObjectManager::LoadObjectById(uint64_t object_id)
{
    auto object = GetObjectById(object_id);

    if (!object)
    {
        object = CreateObjectFromStorage(object_id);

        boost::lock_guard<boost::shared_mutex> lg(object_map_mutex_);
        object_map_.insert(make_pair(object_id, object));
		
		LoadContainedObjects(object);
    }

    return object;
}

shared_ptr<Object> ObjectManager::LoadObjectById(uint64_t object_id, uint32_t object_type)
{
    auto object = GetObjectById(object_id);

    if (!object)
    {
        object = CreateObjectFromStorage(object_id, object_type);

        boost::lock_guard<boost::shared_mutex> lg(object_map_mutex_);
        object_map_.insert(make_pair(object_id, object));

		LoadContainedObjects(object);		
    }

    return object;
}

void ObjectManager::LoadContainedObjects(std::shared_ptr<Object> object)
{	
	object->ViewObjects(nullptr, 0, true, [&](shared_ptr<Object> contained_object){
		object_map_.insert(make_pair(contained_object->GetObjectId(), contained_object));
	});
}

shared_ptr<Object> ObjectManager::GetObjectById(uint64_t object_id)
{
    boost::shared_lock<boost::shared_mutex> lock(object_map_mutex_);
    auto find_iter = object_map_.find(object_id);

    if (find_iter == object_map_.end())
    {
        return nullptr;
    }

    return find_iter->second;
}

void ObjectManager::RemoveObject(const shared_ptr<Object>& object)
{
    boost::lock_guard<boost::shared_mutex> lg(object_map_mutex_);
    object_map_.unsafe_erase(object_map_.find(object->GetObjectId()));
}

shared_ptr<Object> ObjectManager::GetObjectByCustomName(const wstring& custom_name)
{
    boost::shared_lock<boost::shared_mutex> lock(object_map_mutex_);
	auto find_iter = std::find_if(
        std::begin(object_map_), 
        std::end(object_map_),
        [custom_name] (pair<uint64_t, shared_ptr<Object>> key_value)
    {
		return key_value.second->GetCustomName().compare(custom_name) == 0;
	});

	if (find_iter == object_map_.end())
    {
        return nullptr;
    }

    return find_iter->second;
}

shared_ptr<Object> ObjectManager::CreateObjectFromStorage(uint64_t object_id)
{
    shared_ptr<Object> object;
    
    if (factories_.size() == 0)
        return object;
    
    // lookup the type
    uint32_t object_type = factories_[0]->LookupType(object_id);
    auto find_iter = factories_.find(object_type);

    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot create object for an unregistered type.");
    }
    object = find_iter->second->CreateObjectFromStorage(object_id);

    return object;
}

shared_ptr<Object> ObjectManager::CreateObjectFromStorage(uint64_t object_id, uint32_t object_type)
{
    auto find_iter = factories_.find(object_type);

    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot create object for an unregistered type.");
    }

    return find_iter->second->CreateObjectFromStorage(object_id);
}

shared_ptr<Object> ObjectManager::CreateObjectFromTemplate(const std::string& template_name, bool is_persisted, bool is_initialized)
{
	shared_ptr<Object> created_object;
    auto template_itr = type_lookup_.find(template_name);
	if(template_itr != type_lookup_.end())
	{
		auto factory_itr = factories_.find(template_itr->second);
		if(factory_itr != factories_.end())
		{
			created_object = factory_itr->second->CreateObjectFromTemplate(template_name, is_persisted, is_initialized);
			if(created_object != nullptr)
			{
				created_object->SetPermissions(permissions_objects_.find(DEFAULT_CONTAINER_PERMISSION)->second);

				if(!is_persisted)
				{
					created_object->SetObjectId(next_dynamic_id_++);
				}
				object_map_.insert(make_pair(created_object->GetObjectId(), created_object));
			}
		}
	}

	//Make sure the object is valid
	if(created_object == nullptr)
	{
		throw InvalidObjectTemplate("Invalid object template: "+template_name);
	}

	return created_object;
}

std::shared_ptr<Object> ObjectManager::CreateObjectFromTemplate(const std::string& template_name, uint64_t object_id)
{
	shared_ptr<Object> created_object;
    auto template_itr = type_lookup_.find(template_name);
	if(template_itr != type_lookup_.end())
	{
		auto factory_itr = factories_.find(template_itr->second);
		if(factory_itr != factories_.end())
		{
			created_object = factory_itr->second->CreateObjectFromTemplate(template_name, false, false);
			if(created_object != nullptr)
			{
				created_object->SetObjectId(object_id);
				object_map_.insert(make_pair(object_id, created_object));
			}
		}
	}

	//Make sure the object is valid
	if(created_object == nullptr)
	{
		throw InvalidObjectTemplate("Invalid object template: "+template_name);
	}

	return created_object;
}

void ObjectManager::DeleteObjectFromStorage(const std::shared_ptr<Object>& object)
{
    auto find_iter = factories_.find(object->GetType());

    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot delete object from storage for an unregistered type.");
    }

    return find_iter->second->DeleteObjectFromStorage(object);
}

void ObjectManager::PersistObject(const std::shared_ptr<Object>& object)
{
    auto find_iter = factories_.find(object->GetType());

    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot persist object to storage for an unregistered type.");
    }

    find_iter->second->PersistObject(object);
}

void ObjectManager::PersistObject(uint64_t object_id)
{
    auto object = GetObjectById(object_id);

    if (object)
    {
        PersistObject(object);
    }
}

void ObjectManager::PersistRelatedObjects(const std::shared_ptr<Object>& object)
{
	
    if (object)
    {
		// first persist the parent object
        PersistObject(object);
		// Now related objects
		object->ViewObjects(nullptr, 0, true, [&](shared_ptr<Object> contained)
		{
			PersistObject(contained);
		});
    }
}
	
void ObjectManager::PersistRelatedObjects(uint64_t parent_object_id)
{
    auto object = GetObjectById(parent_object_id);

    if (object)
    {
        PersistRelatedObjects(object);
    }
}


std::shared_ptr<swganh::tre::SlotDefinitionVisitor>  ObjectManager::GetSlotDefinition()
{
	return slot_definition_;
}

void ObjectManager::LoadSlotsForObject(std::shared_ptr<Object> object)
{
	auto oiff = static_pointer_cast<ObjectVisitor>(kernel_->GetResourceManager()->getResourceByName(object->GetTemplate(), OIFF_VISITOR));
	oiff->load_aggregate_data(kernel_->GetResourceManager());
	oiff->load_referenced_files(kernel_->GetResourceManager());

	auto arrangmentDescriptor = oiff->attribute<std::shared_ptr<SlotArrangementVisitor>>("arrangementDescriptorFilename");
	auto slotDescriptor = oiff->attribute<std::shared_ptr<SlotDescriptorVisitor>>("slotDescriptorFilename");
	ObjectArrangements arrangements;
		
	// CRAZY SHIT
	// arrangements
	if (arrangmentDescriptor != nullptr)
	{
		for_each(arrangmentDescriptor->begin(), arrangmentDescriptor->end(), [&](std::vector<std::string> arrangement)
		{			
			std::vector<int32_t> arr;
			for (auto& str : arrangement)
			{
				arr.push_back(slot_definition_->findSlotByName(str));				
			}
			arrangements.push_back(arr);
		});
	}
	ObjectSlots descriptors;

	// Globals
	//
	descriptors.insert(ObjectSlots::value_type(-1, shared_ptr<SlotContainer>(new SlotContainer())));
	for (size_t k = 0; k < slot_definition_->count(); ++k)
	{
		auto entry = slot_definition_->entry(k);		
		if (entry.global)
		{
			if(entry.exclusive)
				descriptors.insert(ObjectSlots::value_type(k, shared_ptr<SlotExclusive>(new SlotExclusive())));
			else
				descriptors.insert(ObjectSlots::value_type(k, shared_ptr<SlotContainer>(new SlotContainer())));
		}
	}

	// Descriptors
	if (slotDescriptor != nullptr)
	{
		for ( size_t j = 0; j < slotDescriptor->available_count(); ++j)
		{
			auto descriptor = slotDescriptor->slot(j);
			size_t id = slot_definition_->findSlotByName(descriptor);
			auto entry = slot_definition_->entry(id);
			if(entry.exclusive)
				descriptors.insert(ObjectSlots::value_type(id, shared_ptr<SlotExclusive>(new SlotExclusive())));
			else
				descriptors.insert(ObjectSlots::value_type(id, shared_ptr<SlotContainer>(new SlotContainer())));
		}
	}
	
	object->SetSlotInformation(descriptors, arrangements);
}

PermissionsObjectMap& ObjectManager::GetPermissionsMap()
{
	return permissions_objects_;
}
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object_manager.h"

#include <boost/asio.hpp>
#include <boost/python.hpp>

#include "swganh/logger.h"

#include <bitset>
#include <sstream>

#include "object_factory.h"
#include "swganh/event_dispatcher.h"
#include "swganh/tre/resource_manager.h"
#include "swganh/tre/visitors/objects/object_visitor.h"
#include "swganh/tre/visitors/slots/slot_arrangement_visitor.h"
#include "swganh/tre/visitors/slots/slot_descriptor_visitor.h"
#include "swganh_core/object/slot_exclusive.h"
#include "swganh_core/object/slot_container.h"
#include "swganh/object/template_interface.h"

#include "swganh/scripting/utilities.h"

#include "swganh/database/database_manager_interface.h"
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "permissions/default_permission.h"
#include "permissions/world_permission.h"
#include "permissions/static_container_permission.h"
#include "permissions/creature_permission.h"
#include "permissions/creature_container_permission.h"
#include "permissions/ridable_permission.h"
#include "permissions/world_cell_permission.h"

using namespace std;
using namespace swganh;
using namespace swganh::tre;
using namespace swganh::object;
using namespace swganh::messages;
namespace bp = boost::python;

#define DYNAMIC_ID_START 17596481011712

void ObjectManager::AddContainerPermissionType_(PermissionType type, ContainerPermissionsInterface* ptr)
{
	permissions_objects_.insert(std::make_pair<int, std::shared_ptr<ContainerPermissionsInterface>>(static_cast<int>(type), 
		shared_ptr<ContainerPermissionsInterface>(ptr)));
}

ObjectManager::ObjectManager(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel), next_dynamic_id_(DYNAMIC_ID_START)
{
	//Load Permissions
	AddContainerPermissionType_(DEFAULT_PERMISSION, new DefaultPermission());
	AddContainerPermissionType_(WORLD_PERMISSION, new WorldPermission());
	AddContainerPermissionType_(STATIC_CONTAINER_PERMISSION, new StaticContainerPermission());
	AddContainerPermissionType_(WORLD_CELL_PERMISSION, new WorldCellPermission());
	AddContainerPermissionType_(CREATURE_PERMISSION, new CreaturePermission());
	AddContainerPermissionType_(CREATURE_CONTAINER_PERMISSION, new CreatureContainerPermission());
	AddContainerPermissionType_(RIDEABLE_PERMISSION, new RideablePermission());

	//Load slot definitions
	slot_definition_ = kernel->GetResourceManager()->GetResourceByName<SlotDefinitionVisitor>("abstract/slot/slot_definition/slot_definitions.iff");

	persist_timer_ = std::make_shared<boost::asio::deadline_timer>(kernel_->GetIoService(), boost::posix_time::minutes(5));
	persist_timer_->async_wait(boost::bind(&ObjectManager::PersistObjectsByTimer, this, boost::asio::placeholders::error));

	// Load the highest object_id from the db
	unique_ptr<sql::Statement> statement(kernel_->GetDatabaseManager()->getConnection("galaxy")->createStatement());
	auto result = unique_ptr<sql::ResultSet>(statement->executeQuery("CALL sp_GetHighestObjectId();"));
	if (result->next())
		next_persistent_id_ = result->getUInt64(1);
	while(statement->getMoreResults());

	LoadPythonObjectTemplates();
}

ObjectManager::~ObjectManager()
{}

void ObjectManager::RegisterObjectType(uint32_t object_type, const shared_ptr<ObjectFactoryInterface>& factory)
{
	{
		boost::lock_guard<boost::shared_mutex> lock(object_factories_mutex_);
		auto find_iter = factories_.find(object_type);
		if (find_iter != factories_.end())
		{
			throw InvalidObjectType("A factory for the specified object type already exists.");
		}
		factories_.insert(make_pair(object_type, factory));
	}

	factory->RegisterEventHandlers();
}

void ObjectManager::UnregisterObjectType(uint32_t object_type)
{
	boost::lock_guard<boost::shared_mutex> lock(object_factories_mutex_);
    auto find_iter = factories_.find(object_type);
    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot remove a factory for an object type that has not been registered.");
    }

    factories_.erase(find_iter);
}
void ObjectManager::RegisterMessageBuilder(uint32_t object_type, std::shared_ptr<ObjectMessageBuilder> message_builder)
{
	boost::lock_guard<boost::shared_mutex> lock(object_factories_mutex_);
    auto find_iter = message_builders_.find(object_type);
    if (find_iter != end(message_builders_))
        throw InvalidObjectType("A message builder for the specified type already exists.");
    message_builders_.insert(make_pair(object_type, message_builder));	
}
void ObjectManager::InsertObject(std::shared_ptr<swganh::object::Object> object)
{
	boost::lock_guard<boost::shared_mutex> lg(object_map_mutex_);
    object_map_.insert(make_pair(object->GetObjectId(), object));
}

void ObjectManager::PersistObjectsByTimer(const boost::system::error_code& e)
{
	if (!e)
	{
		boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
		for (auto& factory : factories_)
		{
			factory.second->PersistChangedObjects();
		}
		persist_timer_->expires_from_now(boost::posix_time::minutes(5));
		persist_timer_->async_wait(boost::bind(&ObjectManager::PersistObjectsByTimer, this, boost::asio::placeholders::error));
	}
	else
	{
		LOG(warning) << "PersistObjectsByTimer error: " << e.message();
	}

}

shared_ptr<Object> ObjectManager::LoadObjectById(uint64_t object_id)
{
    auto object = GetObjectById(object_id);

    if (!object)
    {
        object = CreateObjectFromStorage(object_id);

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

        InsertObject(object);

		LoadContainedObjects(object);		
    }	

    return object;
}

void ObjectManager::LoadContainedObjects(std::shared_ptr<Object> object)
{	
	object->ViewObjects(nullptr, 0, true, [&](shared_ptr<Object> contained_object){
		InsertObject(contained_object);
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
    object_map_.erase(object_map_.find(object->GetObjectId()));
}

shared_ptr<Object> ObjectManager::GetObjectByCustomName(const wstring& custom_name)
{
    auto check_name = custom_name;
    std::transform(std::begin(check_name), std::end(check_name), std::begin(check_name), ::towlower);

    boost::shared_lock<boost::shared_mutex> lock(object_map_mutex_);
	auto find_iter = std::find_if(
        std::begin(object_map_), 
        std::end(object_map_),
        [&check_name] (pair<uint64_t, shared_ptr<Object>> key_value) -> bool
    {
        // Names are case insensitive, normalize by converting to lowercase before comparison checks
        auto custom_name = key_value.second->GetCustomName();
        std::transform(std::begin(custom_name), std::end(custom_name), std::begin(custom_name), ::towlower);

        if (custom_name.compare(check_name) == 0)
        {
            return true;
        }

        // Only first names are unique, if a complete match fails then
        // attempt to match the first name only.
        std::size_t pos = custom_name.find(L" ");
        std::wstring firstname = custom_name.substr(0, pos);

        if (firstname.compare(check_name) == 0)
        {
            return true;
        }

        return false;
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
	std::shared_ptr<ObjectFactoryInterface> factory;
	{
		boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
		uint32_t object_type = factories_[0]->LookupType(object_id);
		auto find_iter = factories_.find(object_type);
		if (find_iter == factories_.end())
		{
			throw InvalidObjectType("Cannot create object for an unregistered type.");
		}
		factory = find_iter->second;
	}
    object = factory->CreateObjectFromStorage(object_id);
	
	return object;
}

shared_ptr<Object> ObjectManager::CreateObjectFromStorage(uint64_t object_id, uint32_t object_type)
{
	std::shared_ptr<ObjectFactoryInterface> factory;
	{
		boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
		auto find_iter = factories_.find(object_type);
		if (find_iter == factories_.end())
		{
			throw InvalidObjectType("Cannot create object for an unregistered type.");
		}
		factory = find_iter->second;
	}

    return factory->CreateObjectFromStorage(object_id);
}

shared_ptr<Object> ObjectManager::CreateObjectFromTemplate(const string& template_name, 
			PermissionType type, bool is_persisted, bool is_initialized, uint64_t object_id)
{
	//First find the container permission
	auto permission_itr = permissions_objects_.find(type);
	if(permission_itr == permissions_objects_.end())
	{
		DLOG(warning) << "Bad permission type requested in CreateObjectFromTemplate";
		return nullptr;
	}

	//Then make sure we actually can create an object of this type
	shared_ptr<Object> created_object, prototype;
	{
		boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
		auto prototype_itr = prototypes_.find(template_name);
		if(prototype_itr != prototypes_.end())
		{
			prototype = prototype_itr->second;	
		}		
	}
	if (!prototype)
	{
		// Call python To get Object
		swganh::scripting::ScopedGilLock lock;
		{
			{
				boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
				auto template_iter = object_templates_.find(template_name);
				if (template_iter == object_templates_.end())
				{
					return nullptr;	
				}
				// Temp
				prototype = template_iter->second->CreateTemplate(kernel_,std::map<std::string, std::string>());
			}

		}
	}

	if(prototype)
	{
		if(is_initialized)
		{
			created_object = prototype->Clone();
		}
		else
		{
			boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
			auto factory_itr = factories_.find(prototype->GetType());
			if(factory_itr != factories_.end())
			{
				created_object = factory_itr->second->CreateObject();
			}
		}

		if(created_object != nullptr)
		{
			//Set the required stuff
			created_object->SetPermissions(permission_itr->second);
			created_object->SetEventDispatcher(kernel_->GetEventDispatcher());
			created_object->SetDatabasePersisted(is_persisted);
			LoadSlotsForObject(created_object);
			LoadCollisionInfoForObject(created_object);

			//Set the ID based on the inputs
			if(is_persisted)
			{
				created_object->SetObjectId(next_persistent_id_++);
			}
			else if(object_id == 0)
			{
				created_object->SetObjectId(next_dynamic_id_++);
			}
			else 
			{
				created_object->SetObjectId(object_id);
			}

			//Insert it into the object map
			InsertObject(created_object);
		}
	}
	return created_object;
}

void ObjectManager::DeleteObjectFromStorage(const std::shared_ptr<Object>& object)
{
	std::shared_ptr<ObjectFactoryInterface> factory;
	{
		boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
		auto find_iter = factories_.find(object->GetType());
		if (find_iter == factories_.end())
		{
			throw InvalidObjectType("Cannot delete object from storage for an unregistered type.");
		}
		factory = find_iter->second;
	}

    return factory->DeleteObjectFromStorage(object);
}

void ObjectManager::PersistObject(const std::shared_ptr<Object>& object, bool persist_inherited)
{
	std::shared_ptr<ObjectFactoryInterface> factory;
	{
		boost::shared_lock<boost::shared_mutex> lock(object_factories_mutex_);
		auto find_iter = factories_.find(object->GetType());
		if (find_iter == factories_.end())
		{
			throw InvalidObjectType("Cannot delete object from storage for an unregistered type.");
		}
		factory = find_iter->second;
	}

	if(object->IsDatabasePersisted())
    {
		factory->PersistObject(object, persist_inherited);
	}
}

void ObjectManager::PersistObject(uint64_t object_id, bool persist_inherited)
{
    auto object = GetObjectById(object_id);
    if (object)
    {
        PersistObject(object, persist_inherited);
    }
}

void ObjectManager::PersistRelatedObjects(const std::shared_ptr<Object>& object, bool persist_inherited)
{
    if (object)
    {
		// first persist the parent object
        PersistObject(object, persist_inherited);

		// Now related objects
		object->ViewObjects(nullptr, 0, true, [&](shared_ptr<Object> contained)
		{
			PersistObject(contained, persist_inherited);
		});
    }
}
	
void ObjectManager::PersistRelatedObjects(uint64_t parent_object_id, bool persist_inherited)
{
    auto object = GetObjectById(parent_object_id);
    if (object)
    {
        PersistRelatedObjects(object, persist_inherited);
    }
}


std::shared_ptr<swganh::tre::SlotDefinitionVisitor>  ObjectManager::GetSlotDefinition()
{
	return slot_definition_;
}

void ObjectManager::LoadCollisionInfoForObject(std::shared_ptr<Object> obj)
{
		auto obj_visitor = kernel_->GetResourceManager()->GetResourceByName<ObjectVisitor>(obj->GetTemplate());
		obj_visitor->load_aggregate_data(kernel_->GetResourceManager());

		if(obj_visitor->has_attribute("collisionLength") && obj_visitor->has_attribute("collisionHeight"))
		{
			obj->SetCollisionBoxSize(obj_visitor->attribute<float>("collisionLength"), obj_visitor->attribute<float>("collisionHeight"));
			obj->SetCollidable(true);
		}
		else
			obj->SetCollidable(false);
}

void ObjectManager::LoadSlotsForObject(std::shared_ptr<Object> object)
{
	auto oiff = kernel_->GetResourceManager()->GetResourceByName<ObjectVisitor>(object->GetTemplate());
	if(oiff == nullptr)
		return;

	oiff->load_aggregate_data(kernel_->GetResourceManager());

	if(oiff->has_attribute("arrangementDescriptorFilename") &&
		oiff->has_attribute("slotDescriptorFilename"))
	{
		auto arrangmentDescriptor = kernel_->GetResourceManager()->GetResourceByName<SlotArrangementVisitor>(
			oiff->attribute<std::string>("arrangementDescriptorFilename"));

		auto slotDescriptor = kernel_->GetResourceManager()->GetResourceByName<SlotDescriptorVisitor>(
			oiff->attribute<std::string>("slotDescriptorFilename"));

		ObjectArrangements arrangements;
		
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
}

PermissionsObjectMap& ObjectManager::GetPermissionsMap()
{
	return permissions_objects_;
}

void ObjectManager::PrepareToAccomodate(uint32_t delta)
{
	boost::lock_guard<boost::shared_mutex> lg(object_map_mutex_);
	object_map_.reserve(object_map_.size() + delta);
}

void ObjectManager::LoadPythonObjectTemplates()
{
	swganh::scripting::ScopedGilLock lock;
	try {		
		LOG(info) << "Loading Prototype and Template Objects";
		auto module = bp::import("load_objects");
		auto python_template = module.attr("templates");
		object_templates_ = bp::extract<PythonTemplateMap>(python_template);			
		auto python_prototypes = module.attr("prototypes");
		prototypes_ = bp::extract<PrototypeMap>(python_prototypes);
		LOG(info) << "Finished Loading...";
	}
	catch(bp::error_already_set&)
	{
		PyErr_Print();		
	}
}

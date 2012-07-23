// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object_manager.h"

#include "anh/logger.h"

#include "object_factory.h"

using namespace std;
using namespace swganh::object;

ObjectManager::ObjectManager(anh::EventDispatcher* event_dispatcher, 
                             anh::database::DatabaseManagerInterface* db_manager)
    : event_dispatcher_(event_dispatcher)
    , db_manager_(db_manager)
{}

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
	for(auto& inner_object : object->GetContainedObjects())
	{
		object_map_.insert(make_pair(inner_object.first, inner_object.second));
		// Recurse
		if (inner_object.second->GetContainedObjects().size() > 0)
			LoadContainedObjects(inner_object.second);
	}
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

shared_ptr<Object> ObjectManager::CreateObjectFromTemplate(const std::string& template_name)
{
    auto find_iter = find_if(begin(factories_), end(factories_),
        [&template_name] (const ObjectFactoryMap::value_type& factory_entry) 
    {
        return factory_entry.second->HasTemplate(template_name);
    });
    
    if (find_iter == factories_.end())
    {
        throw InvalidObjectType("Cannot create object for an unregistered type.");
    }
    
    return find_iter->second->CreateObjectFromTemplate(template_name);
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

    return find_iter->second->PersistObject(object);
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

        // get all the contained objects
        auto contained_objects = object->GetContainedObjects();
        for (auto& contained_object : contained_objects)
        {
            PersistRelatedObjects(contained_object.second);
        }
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

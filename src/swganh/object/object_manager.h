// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_OBJECT_MANAGER_H_
#define SWGANH_OBJECT_OBJECT_MANAGER_H_

#include <cstdint>
#include <exception>
#include <map>
#include <memory>
#include <string>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#include <concurrent_queue.h>
#else
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_queue.h>

namespace concurrency {
    using ::tbb::concurrent_unordered_map;
    using ::tbb::concurrent_queue;
}
#endif

#include "anh/database/database_manager_interface.h"

#include "swganh/object/object.h"
#include "swganh/object/exception.h"
#include "swganh/object/object_factory_interface.h"
#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace object {
    
    /**
     * ObjectManager is a general interface for managing the object lifecycles for
     * general user defined types. In order to manage an object type an implementation
     * of ObjectFactoryInterface is required.
     */
    class ObjectManager
    {
    public:
        ObjectManager(anh::EventDispatcher* event_dispatcher, anh::database::DatabaseManagerInterface* db_manager)
            : event_dispatcher_(event_dispatcher)
            , db_manager_(db_manager)
        {}

        /**
         * Registers an object type for management.
         *
         * @param object_type the type of object to manage.
         * @param factory a concrete ObjectFactoryInterface implementation.
         */
        void RegisterObjectType(uint32_t object_type,
            const std::shared_ptr<ObjectFactoryInterface>& factory);

        /**
         * Registers an object type for management based on the concrete
         * factory provided.
         *
         * @param factory A concrete ObjectFactoryInterface implementation.
         */
        template<typename T>
        void RegisterObjectType(const std::shared_ptr<T>& factory)
        {
            RegisterObjectType(T::ObjectType::type, factory);
        }

        template<typename T>
        void RegisterObjectType(uint32_t object_type = T::type)
        {
            auto factory = std::make_shared<typename T::FactoryType>(db_manager_, event_dispatcher_);
            factory->SetObjectManager(this);

            RegisterObjectType(object_type, factory);
            
            auto message_builder = std::make_shared<typename T::MessageBuilderType>(event_dispatcher_);
            RegisterMessageBuilder(object_type, message_builder);
        }

        /**
         * Unregisters an object type.
         *
         * @param object_type the type of object to stop managing.
         */
        void UnregisterObjectType(uint32_t object_type);

        std::shared_ptr<Object> LoadObjectById(uint64_t object_id);
        std::shared_ptr<Object> LoadObjectById(uint64_t object_id, uint32_t type);

        void RemoveObject(const std::shared_ptr<Object>& object);
        
        std::shared_ptr<Object> GetObjectById(uint64_t object_id);

		std::shared_ptr<Object> GetObjectByCustomName(const std::wstring& custom_name);

        /**
         * Creates an instance of a stored object with the specified id.
         *
         * Slightly more expensive than it's counterparts, this method attempts
         * to have each registered factory create the object one by one until
         * an object is successfully created.
         *
         * @param object_id The id of the object being created.
         * @return the created object instance.
         * @throws InvalidObject when no object exists for the specified id.
         */
        std::shared_ptr<Object> CreateObjectFromStorage(uint64_t object_id);
                
        /**
         * Creates an instance of a stored object with the specified id.
         *
         * @param object_type The type of the object being created.
         * @param object_id The id of the object being created.
         * @return the created object instance.
         * @throws InvalidObject when no object exists for the specified id.
         */
        std::shared_ptr<Object> CreateObjectFromStorage(uint64_t object_id, uint32_t object_type);
        
        /**
         * Creates an instance of a stored object with the specified id.
         *
         * @param object_id The id of the object being created.
         * @return the created object instance as the specified type.
         * @throws InvalidObject when no object exists for the specified id.
         */
        template<typename T>
        std::shared_ptr<T> CreateObjectFromStorage(uint64_t object_id)
        {
            std::shared_ptr<Object> object = CreateObjectFromStorage(T::type, object_id);

#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(object);
#else
            return std::static_pointer_cast<T>(object);
#endif
        }
        
        /**
         * Creates an instance of an object from the specified template.
         *
         * @return the created object instance.
         * @throws InvalidObjectTemplate when the specified template does not exist.
         */
        std::shared_ptr<Object> CreateObjectFromTemplate(const std::string& template_name);
        
        /**
         * Creates an instance of an object from the specified template.
         *
         * @return the created object instance as the specified type.
         * @throws InvalidObjectTemplate when the specified template does not exist.
         */
        template<typename T>
        std::shared_ptr<T> CreateObjectFromTemplate(const std::string& template_name)
        {
            std::shared_ptr<Object> object = CreateObjectFromTemplate(template_name);

#ifdef _DEBUG
            return std::dynamic_pointer_cast<T>(object);
#else
            return std::static_pointer_cast<T>(object);
#endif
        }
        
        /**
         * Deletes the requested object from storage.
         *
         * @param object the object instance to delete from storage.
         */
        void DeleteObjectFromStorage(const std::shared_ptr<Object>& object);
        
        /**
         * Persists the object's state to storage.
         *
         * @param object the object instance to persist.
         */
        void PersistObject(const std::shared_ptr<Object>& object);

        void PersistObject(uint64_t object_id);
        
        void PersistRelatedObjects(const std::shared_ptr<Object>& object);
	    void PersistRelatedObjects(uint64_t parent_object_id);
    private:
        /**
         * Registers message builders
         *
         * @param object_type the type of the object to register a builder to.
         */
        template<typename T>
        void RegisterMessageBuilder(std::shared_ptr<T>& message_builder)
        {
            RegisterMessageBuilder(T::type, message_builder);
        }

        void RegisterMessageBuilder(uint32_t object_type, std::shared_ptr<ObjectMessageBuilder> message_builder);

        anh::EventDispatcher* event_dispatcher_;
        anh::database::DatabaseManagerInterface* db_manager_;

        typedef std::map<
            uint32_t, 
            std::shared_ptr<ObjectFactoryInterface>
        > ObjectFactoryMap;

        ObjectFactoryMap factories_;

        typedef std::map<
            uint32_t,
            std::shared_ptr<ObjectMessageBuilder>
        > ObjectMessageBuilderMap;

        ObjectMessageBuilderMap message_builders_;
        
        concurrency::concurrent_unordered_map<uint64_t, std::shared_ptr<Object>> object_map_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_MANAGER_H_

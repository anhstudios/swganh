
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_OBJECT_MANAGER_H_
#define SWGANH_OBJECT_OBJECT_MANAGER_H_

#include <cstdint>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <queue>

#include <boost/thread/shared_mutex.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/python/object.hpp>

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
#include "swganh/app/swganh_kernel.h"
#include "swganh/tre/visitors/slots/slot_definition_visitor.h"

#include "swganh_core/object/object.h"
#include "swganh_core/object/exception.h"
#include "swganh_core/object/object_factory_interface.h"
#include "swganh_core/object/object_message_builder.h"
#include "swganh_core/object/permissions/permission_type.h"

namespace swganh {
namespace object {
    
	typedef std::map<int, std::shared_ptr<ContainerPermissionsInterface>> PermissionsObjectMap;

	class TemplateInterface;
    /**
     * ObjectManager is a general interface for managing the object lifecycles for
     * general user defined types. In order to manage an object type an implementation
     * of ObjectFactoryInterface is required.
     */
    class ObjectManager
    {
    public:
        ObjectManager(
			swganh::app::SwganhKernel* kernel
			);
        ~ObjectManager();

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

        /**
         * Registers an object type for management based on the object type.
         *
         * @param object_type the type of object to manage.
         */
        template<typename T>
        void RegisterObjectType(uint32_t object_type = T::type)
        {
            auto factory = std::make_shared<typename T::FactoryType>(kernel_);
            factory->SetObjectManager(this);

            RegisterObjectType(object_type, factory);
            
            auto message_builder = std::make_shared<typename T::MessageBuilderType>(kernel_->GetEventDispatcher());
            RegisterMessageBuilder(object_type, message_builder);
        }

        /**
         * Unregisters an object type.
         *
         * @param object_type the type of object to stop managing.
         */
        void UnregisterObjectType(uint32_t object_type);

        /**
         * Loads an existing object by its identifier.
         *
         * @param object_id The id of the object to load
         * @return Instance of the requested object, or nullptr if the object does not exist
         */
        std::shared_ptr<Object> LoadObjectById(uint64_t object_id);
        
        /**
         * Loads an existing object by its identifier.
         *
         * @param object_id The id of the object to load
         * @param object_type the type of object to manage.
         * @return Instance of the requested object, or nullptr if the object does not exist
         */
        std::shared_ptr<Object> LoadObjectById(uint64_t object_id, uint32_t object_type);

		/**
		 * Loads an objects contained objects (recursively)
		 *
		 * @param object the object that will be used to load contained objects
		 */
		void LoadContainedObjects(std::shared_ptr<Object> object);

        /**
         * Removes an object from management.
         *
         * @TODO Refactor this and rename to Unload to match the Load semantics.
         *
         * This method removes an object from management. All existing handles to the
         * object remain valid until the last one goes out of scope.
         *
         * @param object The object to remove from management
         */
        void RemoveObject(const std::shared_ptr<Object>& object);
        
        /**
         * Finds and returns an object from management based on its id.
         *
         * @param object_id The id of the object to load
         * @return Instance of the requested object, or nullptr if the object does not exist
         */
        std::shared_ptr<Object> GetObjectById(uint64_t object_id);
        
        /**
         * Finds and returns an object from management based on its custom name.
         *
         * @param custom_name The custom name of the object to load
         * @return Instance of the requested object, or nullptr if the object does not exist
         */
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
        virtual std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, 
			swganh::object::PermissionType type=swganh::object::DEFAULT_PERMISSION, bool is_persisted=true, uint64_t object_id=0);
		
        /**
         * Creates an instance of an object from the specified template.
         *
         * @return the created object instance as the specified type.
         * @throws InvalidObjectTemplate when the specified template does not exist.
         */
        template<typename T>
        std::shared_ptr<T> CreateObjectFromTemplate(const std::string& template_name, 
			swganh::object::PermissionType type=swganh::object::DEFAULT_PERMISSION, bool is_persisted=true, uint64_t object_id=0)
        {
            std::shared_ptr<Object> object = CreateObjectFromTemplate(template_name, is_persisted, objectId);

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
        void PersistObject(const std::shared_ptr<Object>& object, bool persist_inherited = false);

        /**
         * Persists a currently managed object by its id.
         *
         * @param object_id The id of the object to persist
         */
        void PersistObject(uint64_t object_id, bool persist_inherited = false);
        
        /**
         * Persists an object and all its related objects.
         *
         * @param object the object instance to persist.
         */
        void PersistRelatedObjects(const std::shared_ptr<Object>& object, bool persist_inherited = false);
        
        /**
         * Persists a managed object and all its related objects by id.
         *
         * @param object_id The id of the object to persist
         */
	    void PersistRelatedObjects(uint64_t parent_object_id, bool persist_inherited = false);

		/**
		 * Gets the objects definition file data from the resource manager
		 *
		 */
		std::shared_ptr<swganh::tre::SlotDefinitionVisitor> GetSlotDefinition();

		void LoadSlotsForObject(std::shared_ptr<Object> object);
		void LoadCollisionInfoForObject(std::shared_ptr<Object> object);

		PermissionsObjectMap& GetPermissionsMap();

		virtual void PrepareToAccomodate(uint32_t delta);

    private:
		void PersistObjectsByTimer(const boost::system::error_code& e);
		void InsertObject(std::shared_ptr<swganh::object::Object> object);
		
		typedef std::map<
            uint32_t, 
            std::shared_ptr<ObjectFactoryInterface>
        > ObjectFactoryMap;

		typedef std::map<
            uint32_t,
            std::shared_ptr<ObjectMessageBuilder>
        > ObjectMessageBuilderMap;

		typedef std::map<std::string, boost::python::object> PythonTemplateMap;

        /**
         * Registers a message builder for a specific object type
         *
         * @param message_builder The message builder to store.
         */
        template<typename T>
        void RegisterMessageBuilder(std::shared_ptr<T>& message_builder)
        {
            RegisterMessageBuilder(T::type, message_builder);
        }
        
        /**
         * Registers a message builder for a specific object type
         *
         * @param object_type the type of the object to register a builder to.
         * @param message_builder The message builder to store.
         */
        void RegisterMessageBuilder(uint32_t object_type, std::shared_ptr<ObjectMessageBuilder> message_builder);
		/**
		 *  Loads the python object templates and creates any objects inserting them as prototypes
		 */
		void LoadPythonObjectTemplates();

		void ObjectManager::AddContainerPermissionType_(swganh::object::PermissionType type, swganh::object::ContainerPermissionsInterface* ptr);

        swganh::app::SwganhKernel* kernel_;

		std::shared_ptr<swganh::tre::SlotDefinitionVisitor> slot_definition_;

        ObjectFactoryMap factories_;
        ObjectMessageBuilderMap message_builders_;
        
		PythonTemplateMap object_templates_;

		uint64_t next_dynamic_id_;
		uint64_t next_persistent_id_;

        boost::shared_mutex object_map_mutex_;
		boost::shared_mutex object_factories_mutex_;
		std::unordered_map<uint64_t, std::shared_ptr<Object>> object_map_;
		std::shared_ptr<boost::asio::deadline_timer> persist_timer_;
		

		PermissionsObjectMap permissions_objects_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_MANAGER_H_


#ifndef SWGANH_OBJECT_OBJECT_MANAGER_H_
#define SWGANH_OBJECT_OBJECT_MANAGER_H_

#include <cstdint>
#include <exception>
#include <map>
#include <memory>
#include <string>

#include "swganh/object/object.h"
#include "swganh/object/exception.h"
#include "swganh/object/object_factory_interface.h"

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
            RegisterObjectType(T::type, factory);
        }

        /**
         * Unregisters an object type.
         *
         * @param object_type the type of object to stop managing.
         */
        void UnregisterObjectType(uint32_t object_type);
                
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

#if _DEBUG
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

#if _DEBUG
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

    private:
        typedef std::map<
            uint32_t, 
            std::shared_ptr<ObjectFactoryInterface>
        > ObjectFactoryMap;

        ObjectFactoryMap factories_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_MANAGER_H_

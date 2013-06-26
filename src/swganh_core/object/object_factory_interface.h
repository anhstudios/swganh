// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <future>
#include <memory>
#include <string>

#include <cppconn/connection.h>

#include <boost/thread.hpp>

namespace swganh {
namespace object {

    class Object;
    class ObjectManager;
    
    class ObjectFactoryInterface
    {
    public:
        virtual ~ObjectFactoryInterface() {}

        /**
         * Persists the object's state to storage.
         *
         * @param object the object instance to persist.
         */
        virtual uint32_t PersistObject(const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false) = 0;

        /**
         * Deletes the requested object from storage.
         *
         * @param object the object instance to delete from storage.
         */
        virtual void DeleteObjectFromStorage(const std::shared_ptr<Object>& object) = 0;

        /**
         * Creates an instance of a stored object with the specified id.
         *
         * Intended to be passed to DatabaseManager::ExecuteAsync
         *
         * @return a future to the created object instance.
         * @throws InvalidObject when no object exists for the specified id.
         */
        virtual std::future<std::shared_ptr<Object>> LoadFromStorage(uint64_t object_id) = 0;
        
        virtual void LoadContainedObjects(const std::shared_ptr<Object>& object) = 0;
                
        /**
         * Creates an instance of an object from the specified template.
         *
         * @return the created object instance.
         * @throws InvalidObjectTemplate when the specified template does not exist.
         */
        virtual std::shared_ptr<Object> CreateObject() = 0;

        /**
         * Gets the type of the object from a db lookup call
         *
         * @param the object_id of which to find the type
         * @return the object type
         */
        virtual uint32_t LookupType(uint64_t object_id) = 0;

		/**
		 * Builds an update query for all the objects that have changed.
		 * 
		 */
		virtual void PersistChangedObjects() = 0;

		/**
		 *  Registers events for a specific factory 
		 */
		virtual void RegisterEventHandlers() = 0;

    };

}}  // namespace swganh::object

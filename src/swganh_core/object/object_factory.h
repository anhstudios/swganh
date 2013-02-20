// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_OBJECT_FACTORY_H_
#define SWGANH_OBJECT_OBJECT_FACTORY_H_

#include "swganh_core/object/object_factory_interface.h"
#include "swganh/event_dispatcher.h"
#include "swganh/app/swganh_kernel.h"

#include <set>
#include <boost/thread/mutex.hpp>

namespace swganh {
namespace database {
class DatabaseManager;
}} // swganh::database

namespace sql {
    class ResultSet;
    class Statement;
    class PreparedStatement;
}  // namespace sql

namespace swganh {
namespace simulation {
    class SimulationServiceInterface;
}

namespace tre {
    class TreArchive;
}}  // namespace swganh::tre

namespace swganh {
namespace object {

    class Object;
    class ObjectManager;
	class ContainerPermissionsInterface;

    class ObjectFactory : public ObjectFactoryInterface
    {
    public:
        ObjectFactory(swganh::app::SwganhKernel* kernel);
        virtual ~ObjectFactory() {}

        void SetObjectManager(ObjectManager* object_manager) { object_manager_ = object_manager; }

        /**
         * Loads in base values from a result set
         *
         * @param the object which to load values into
         * @param the result set from which to load the values from
         */
        void CreateBaseObjectFromStorage(const std::shared_ptr<Object>& object, const std::shared_ptr<sql::ResultSet>& result);
        virtual uint32_t PersistObject(const std::shared_ptr<Object>& object, bool persist_inherited = false);
        
        virtual void DeleteObjectFromStorage(const std::shared_ptr<Object>& object);
        virtual std::shared_ptr<Object> CreateObjectFromStorage(uint64_t object_id){ return nullptr; }
		virtual std::shared_ptr<Object> CreateObject() { return nullptr; }
        uint32_t LookupType(uint64_t object_id);
		void LoadAttributes(std::shared_ptr<Object> object);
		void PersistAttributes(std::shared_ptr<Object> object);

		virtual void PersistChangedObjects();
		void PersistHandler(const std::shared_ptr<swganh::EventInterface>& incoming_event);
        virtual void RegisterEventHandlers();
        void SetTreArchive(swganh::tre::TreArchive* tre_archive);
		// Fiils in missing data for the object from the client file...
		void GetClientData(const std::shared_ptr<Object>& object);

		swganh::database::DatabaseManager* GetDatabaseManager() { return kernel_->GetDatabaseManager(); }
		swganh::EventDispatcher* GetEventDispatcher() { return kernel_->GetEventDispatcher(); }
    protected:
        void LoadContainedObjects(const std::shared_ptr<Object>& object,
            const std::shared_ptr<sql::Statement>& statement);
        
        ObjectManager* object_manager_;
		boost::mutex persisted_objects_mutex_;
		swganh::app::SwganhKernel* kernel_;         
        std::set<std::shared_ptr<Object>> persisted_objects_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_FACTORY_H_

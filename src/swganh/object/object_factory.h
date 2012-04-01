
#ifndef SWGANH_OBJECT_OBJECT_FACTORY_H_
#define SWGANH_OBJECT_OBJECT_FACTORY_H_

#include "swganh/object/object_factory_interface.h"
#include "anh/event_dispatcher.h"

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace sql {
    class ResultSet;
    class Statement;
    class PreparedStatement;
}  // namespace sql

namespace swganh {
namespace simulation {
    class SimulationService;
}}  // namespace swganh::simulation

namespace swganh {
namespace object {

    class Object;

    class ObjectFactory : public ObjectFactoryInterface
    {
    public:
        ObjectFactory(anh::database::DatabaseManagerInterface* db_manager,
            swganh::simulation::SimulationService* simulation_service,
            anh::EventDispatcher* event_dispatcher);
        virtual ~ObjectFactory() {}
        /**
         * Loads in base values from a result set
         *
         * @param the object which to load values into
         * @param the result set from which to load the values from
         */
        void CreateBaseObjectFromStorage(const std::shared_ptr<Object>& object, const std::shared_ptr<sql::ResultSet>& result);
        virtual void LoadTemplates(){}
        virtual bool HasTemplate(const std::string& template_name){ return false; }
        virtual void PersistObject(const std::shared_ptr<Object>& object);
        /**
         * Persists the Base Object Data
         *
         * @param object data to persist
         * @param PreparedStatement to add values to.
         */
        void PersistObject(const std::shared_ptr<Object>& object, const std::shared_ptr<sql::PreparedStatement>& prepared_statement);
        virtual void DeleteObjectFromStorage(const std::shared_ptr<Object>& object){}
        virtual std::shared_ptr<Object> CreateObjectFromStorage(uint64_t object_id){ return nullptr; }
        virtual std::shared_ptr<Object> CreateObjectFromTemplate(const std::string& template_name) { return nullptr; }
        uint32_t LookupType(uint64_t object_id) const;
        virtual uint32_t GetType() const { return 0; }
        const static uint32_t type;
        virtual void RegisterEventHandlers(){}

    protected:
        

        void LoadContainedObjects(const std::shared_ptr<Object>& object,
            const std::shared_ptr<sql::Statement>& statement);

        anh::database::DatabaseManagerInterface* db_manager_;   
        swganh::simulation::SimulationService* simulation_service_;
        anh::EventDispatcher* event_dispatcher_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_FACTORY_H_

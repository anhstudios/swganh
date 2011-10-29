
#ifndef SWGANH_OBJECT_OBJECT_FACTORY_H_
#define SWGANH_OBJECT_OBJECT_FACTORY_H_

#include "swganh/object/object_factory_interface.h"

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace sql {
    class ResultSet;
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
        ObjectFactory(const std::shared_ptr<anh::database::DatabaseManagerInterface>& db_manager,
            swganh::simulation::SimulationService* simulation_service);
        virtual ~ObjectFactory() {}
        
        void CreateBaseObjectFromStorage(const std::shared_ptr<Object>& object, const std::shared_ptr<sql::ResultSet>& result);
        virtual void LoadTemplates(){}
        virtual bool HasTemplate(const std::string& template_name){ return false; }
        virtual void PersistObject(const std::shared_ptr<Object>& object){}
        virtual void DeleteObjectFromStorage(const std::shared_ptr<Object>& object){}
        virtual std::shared_ptr<Object> CreateObjectFromStorage(uint64_t object_id){ return nullptr; }
        virtual std::shared_ptr<Object> CreateObjectFromTemplate(const std::string& template_name) { return nullptr; }
        uint32_t LookupType(uint64_t object_id) const;
        virtual uint32_t GetType() const { return 0; }
        const static uint32_t type;
    protected:
        std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager_;   
        swganh::simulation::SimulationService* simulation_service_;
    };

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_FACTORY_H_

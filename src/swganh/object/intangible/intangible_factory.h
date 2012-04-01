
#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_FACTORY_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_FACTORY_H_

#include "swganh/object/object_factory.h"
#include <unordered_map>

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace swganh {
namespace simulation {
    class SimulationService;
}}  // namespace swganh::simulation

namespace swganh {
namespace object {
namespace intangible {
    class Intangible;

    class IntangibleFactory : public swganh::object::ObjectFactory
    {
    public:
        IntangibleFactory(anh::database::DatabaseManagerInterface* db_manager,
            swganh::simulation::SimulationService* simulation_service,
            anh::EventDispatcher* event_dispatcher);
        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);

        virtual void RegisterEventHandlers(){}
    private:
        std::unordered_map<std::string, std::shared_ptr<Intangible>>::iterator GetTemplateIter_(const std::string& template_name);
        std::unordered_map<std::string, std::shared_ptr<Intangible>> intangible_templates_;
    };

}}}  // namespace swganh::object::intangible

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_FACTORY_H_


#ifndef SWGANH_OBJECT_CREATURE_CREATURE_FACTORY_H_
#define SWGANH_OBJECT_CREATURE_CREATURE_FACTORY_H_

#include "swganh/object/tangible/tangible_factory.h"
#include <unordered_map>

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace sql {
    class Statement;
}  // namespace sql

namespace swganh {
namespace simulation {
    class SimulationService;
}}  // namespace swganh::simulation

namespace swganh {
namespace object {
namespace creature {

    class Creature;
    class CreatureFactory : public swganh::object::tangible::TangibleFactory
    {
    public:
        CreatureFactory(anh::database::DatabaseManagerInterface* db_manager,
            swganh::simulation::SimulationService* simulation_service,
            anh::EventDispatcher* event_dispatcher);

        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);
        
    private:
        void LoadSkills_(const std::shared_ptr<Creature>& creature, 
            const std::shared_ptr<sql::Statement>& statement);

        void LoadSkillMods_(const std::shared_ptr<Creature>& creature, 
            const std::shared_ptr<sql::Statement>& statement);

        void LoadSkillCommands_(const std::shared_ptr<Creature>& creature, 
            const std::shared_ptr<sql::Statement>& statement);

        std::unordered_map<std::string, std::shared_ptr<Creature>>::iterator GetTemplateIter_(const std::string& template_name);
        std::unordered_map<std::string, std::shared_ptr<Creature>> creature_templates_;
    };

}}}  // namespace swganh::object::creature

#endif  // SWGANH_OBJECT_CREATURE_CREATURE_FACTORY_H_

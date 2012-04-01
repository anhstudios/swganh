
#ifndef SWGANH_OBJECT_WAYPOINT_WAYPOINT_FACTORY_H_
#define SWGANH_OBJECT_WAYPOINT_WAYPOINT_FACTORY_H_

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

namespace player {class Player; }

namespace waypoint {
    class Waypoint;
    class WaypointFactory : public swganh::object::ObjectFactory
    {
    public:
        WaypointFactory(anh::database::DatabaseManagerInterface* db_manager,
            swganh::simulation::SimulationService* simulation_service,
            anh::EventDispatcher* event_dispatcher);

        void LoadTemplates();

        bool HasTemplate(const std::string& template_name);

        void PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name);

        void LoadWaypoints(const std::shared_ptr<swganh::object::player::Player>& player, const std::shared_ptr<sql::ResultSet> result_set);

        virtual void RegisterEventHandlers();
    private:
        std::unordered_map<std::string, std::shared_ptr<Waypoint>>::iterator GetTemplateIter_(const std::string& template_name);
        std::unordered_map<std::string, std::shared_ptr<Waypoint>> waypoint_templates_;
    };

}}}  // namespace swganh::object::waypoint

#endif  // SWGANH_OBJECT_WAYPOINT_WAYPOINT_FACTORY_H_

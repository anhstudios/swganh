// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_WAYPOINT_WAYPOINT_FACTORY_H_
#define SWGANH_OBJECT_WAYPOINT_WAYPOINT_FACTORY_H_

#include "swganh/object/intangible/intangible_factory.h"
#include <unordered_map>

namespace anh {
namespace database {
class DatabaseManagerInterface;
}} // anh::database

namespace swganh {
namespace object {

namespace player {class Player; }

namespace waypoint {
    class Waypoint;
    class WaypointFactory : public swganh::object::intangible::IntangibleFactory
    {
    public:
		typedef Waypoint ObjectType;

        WaypointFactory(anh::database::DatabaseManagerInterface* db_manager,
            anh::EventDispatcher* event_dispatcher);

        uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, bool db_persisted=true, bool db_initialized=true);

        void LoadWaypoints(const std::shared_ptr<swganh::object::player::Player>& player, const std::shared_ptr<sql::ResultSet> result_set);

        virtual void RegisterEventHandlers();
    private:
    };

}}}  // namespace swganh::object::waypoint

#endif  // SWGANH_OBJECT_WAYPOINT_WAYPOINT_FACTORY_H_

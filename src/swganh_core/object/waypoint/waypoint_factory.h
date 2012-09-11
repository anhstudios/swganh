// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/intangible/intangible_factory.h"
#include <unordered_map>

namespace swganh {
namespace database {
class DatabaseManagerInterface;
}} // swganh::database

namespace swganh {
namespace object {
	class Player; 
    class Waypoint;
    class WaypointFactory : public swganh::object::IntangibleFactory
    {
    public:
		typedef Waypoint ObjectType;

        WaypointFactory(swganh::database::DatabaseManagerInterface* db_manager,
            swganh::EventDispatcher* event_dispatcher);

        uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object);

        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);

        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObjectFromTemplate(const std::string& template_name, bool db_persisted=true, bool db_initialized=true);

        void LoadWaypoints(const std::shared_ptr<swganh::object::Player>& player, const std::shared_ptr<sql::ResultSet> result_set);

        virtual void RegisterEventHandlers();
    private:
    };

}}  // namespace swganh::object

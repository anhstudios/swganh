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

        WaypointFactory(swganh::app::SwganhKernel* kernel);

        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, bool persist_inherited = false);
		void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);
		virtual void PersistChangedObjects();
        std::shared_ptr<swganh::object::Object> CreateObjectFromStorage(uint64_t object_id);

        std::shared_ptr<swganh::object::Object> CreateObject();

        void LoadWaypoints(const std::shared_ptr<swganh::object::Player>& player, const std::shared_ptr<sql::ResultSet> result_set);

        virtual void RegisterEventHandlers();
    private:
    };

}}  // namespace swganh::object

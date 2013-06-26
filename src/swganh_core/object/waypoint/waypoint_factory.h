// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <glm/glm.hpp>

#include "swganh_core/object/waypoint/waypoint.h"
#include "swganh_core/object/intangible/intangible_factory.h"

namespace swganh {
namespace object {
    class Waypoint;
    class WaypointFactory : public swganh::object::IntangibleFactory
    {
    public:
		typedef Waypoint ObjectType;

        WaypointFactory(swganh::app::SwganhKernel* kernel);
        
        virtual void LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock);

        virtual uint32_t PersistObject(const std::shared_ptr<swganh::object::Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited = false);
		
        void DeleteObjectFromStorage(const std::shared_ptr<swganh::object::Object>& object);
		
        virtual void PersistChangedObjects();

        std::shared_ptr<swganh::object::Object> CreateObject();        

        virtual void RegisterEventHandlers();
    };

}}  // namespace swganh::object

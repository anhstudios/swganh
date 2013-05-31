// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/service/service_interface.h"
#include "swganh_core/object/waypoint/waypoint.h"

namespace swganh {
namespace object {
    class Object;
} // namespace swganh::object

namespace waypoint {
    class WaypointServiceInterface : public swganh::service::BaseService
    {
    public:

        virtual ~WaypointServiceInterface() {}

        virtual std::shared_ptr<swganh::object::Waypoint> CreateWaypointInStorage(
            const std::shared_ptr<swganh::object::Object>& owner,
            const std::string& name,
            const glm::vec3& coordinates,
            const std::string& planet,
            swganh::object::Waypoint::WaypointColor color = swganh::object::Waypoint::WHITE,
            bool activated = true) = 0;
    };
}} // namespace swganh::waypoint

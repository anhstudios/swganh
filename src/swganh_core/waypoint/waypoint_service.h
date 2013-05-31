// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once


#include "waypoint_service_interface.h"

namespace swganh {

namespace app {
    class SwganhKernel;
}

namespace object {
    class Waypoint;
}

namespace waypoint {

    class WaypointService : public WaypointServiceInterface
    {
    public:
        WaypointService(swganh::app::SwganhKernel* kernel);
        ~WaypointService();

        virtual std::shared_ptr<swganh::object::Waypoint> CreateWaypointInStorage(
            const std::shared_ptr<swganh::object::Object>& owner,
            const std::string& name,
            const glm::vec3& coordinates,
            const std::string& planet,
            swganh::object::Waypoint::WaypointColor color = swganh::object::Waypoint::WHITE,
            bool activated = true);

    private:
        swganh::app::SwganhKernel* kernel_;
    };
}} // namespace swganh::waypoint

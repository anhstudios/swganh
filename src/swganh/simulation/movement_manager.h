
#ifndef SWGANH_SIMULATION_MOVEMENT_MANAGER_H_
#define SWGANH_SIMULATION_MOVEMENT_MANAGER_H_

#include <memory>

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace object {
    class ObjectController;
}}  // namespace swganh::object

namespace swganh {
namespace simulation {

    class MovementManager
    {
    public:
        MovementManager();

        void HandleDataTransform(
            const std::shared_ptr<swganh::object::ObjectController>&, 
            const swganh::messages::ObjControllerMessage&);

        void HandleDataTransformWithParent(
            const std::shared_ptr<swganh::object::ObjectController>&, 
            const swganh::messages::ObjControllerMessage&);
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_MOVEMENT_MANAGER_H_

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SIMULATION_MOVEMENT_MANAGER_INTERFACE_H_
#define SWGANH_SIMULATION_MOVEMENT_MANAGER_INTERFACE_H_

#include <memory>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <tbb/concurrent_unordered_map.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
}

#endif

#include "swganh/messages/controllers/data_transform.h"
#include "swganh/messages/controllers/data_transform_with_parent.h"

namespace anh {
    class EventDispatcher;
namespace event_dispatcher {
    class EventDispatcherInterface;
}}  // namespace anh::event_dispatcher

namespace swganh {
namespace object {
    class Object;
    class ObjectController;
}}  // namespace swganh::object

namespace swganh {
namespace simulation {
	class SpatialProviderInterface;

    class MovementManagerInterface
    {
    public:
		virtual void HandleDataTransform(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            swganh::messages::controllers::DataTransform message) = 0;

        virtual void HandleDataTransformWithParent(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            swganh::messages::controllers::DataTransformWithParent message) = 0;
        
        virtual void SendDataTransformMessage(const std::shared_ptr<swganh::object::Object>& object, uint32_t unknown = 0x0000000B) = 0;
        virtual void SendUpdateDataTransformMessage(const std::shared_ptr<swganh::object::Object>& object) = 0;
        
        virtual void SendDataTransformWithParentMessage(const std::shared_ptr<swganh::object::Object>& object, uint32_t unknown = 0x0000000B) = 0;
        virtual void SendUpdateDataTransformWithParentMessage(const std::shared_ptr<swganh::object::Object>& object) = 0;

    private:
        virtual void RegisterEvents_(const std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface>& event_dispatcher) = 0;
        virtual void RegisterEvents(anh::EventDispatcher* event_dispatcher) = 0;

        bool ValidateCounter_(uint64_t object_id, uint32_t counter);
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_MOVEMENT_MANAGER_INTERFACE_H_


#ifndef SWGANH_SIMULATION_MOVEMENT_MANAGER_H_
#define SWGANH_SIMULATION_MOVEMENT_MANAGER_H_

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

    class MovementManager
    {
    public:
        explicit MovementManager(anh::EventDispatcher* event_dispatcher);

        void HandleDataTransform(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            swganh::messages::controllers::DataTransform message);

        void HandleDataTransformWithParent(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            swganh::messages::controllers::DataTransformWithParent message);
        
        void SendDataTransformMessage(const std::shared_ptr<swganh::object::Object>& object, uint32_t unknown = 0x0000000B);
        void SendUpdateDataTransformMessage(const std::shared_ptr<swganh::object::Object>& object);
        
        void SendDataTransformWithParentMessage(const std::shared_ptr<swganh::object::Object>& object, uint32_t unknown = 0x0000000B);
        void SendUpdateDataTransformWithParentMessage(const std::shared_ptr<swganh::object::Object>& object);

    private:
        void RegisterEvents_(const std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface>& event_dispatcher);
        void RegisterEvents(anh::EventDispatcher* event_dispatcher);

        bool ValidateCounter_(uint64_t object_id, uint32_t counter);

        typedef Concurrency::concurrent_unordered_map<
            uint64_t, uint32_t
        > UpdateCounterMap;

        UpdateCounterMap counter_map_;
    };

}}  // namespace swganh::simulation

#endif  // SWGANH_SIMULATION_MOVEMENT_MANAGER_H_

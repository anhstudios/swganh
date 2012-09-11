// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_OBJECT_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_OBJECT_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

#include "swganh/event_dispatcher.h"
#include "swganh/observer/observer_interface.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
    class DeltasMessage;
}} // swganh::messages

namespace swganh {
namespace object {

    class Object;
    class ObjectController;
    class ObjectMessageBuilder
    {
    public:
        ObjectMessageBuilder(swganh::EventDispatcher* event_dispatcher_)
            : event_dispatcher(event_dispatcher_)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        virtual void SendEndBaselines(const std::shared_ptr<Object>& object, const std::shared_ptr<swganh::observer::ObserverInterface>& observer);
        static void BuildComplexityDelta(const std::shared_ptr<Object>& object);
        static void BuildStfNameDelta(const std::shared_ptr<Object>& object);
        static void BuildCustomNameDelta(const std::shared_ptr<Object>& object);
        static void BuildVolumeDelta(const std::shared_ptr<Object>& object);
        // delta 6
        static void BuildServerIDDelta(const std::shared_ptr<Object>& object);

        static swganh::messages::BaselinesMessage CreateBaselinesMessage(const std::shared_ptr<Object>& object, uint8_t view_type, uint16_t opcount = 0) ;

        static swganh::messages::DeltasMessage CreateDeltasMessage(const std::shared_ptr<Object>& object, uint8_t view_type, uint16_t update_type, uint16_t update_count = 1) ;

        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Object>& object);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Object>& object);
        typedef swganh::ValueEvent<std::shared_ptr<Object>> ObjectEvent;
    protected:
        swganh::EventDispatcher* event_dispatcher;        
	};

}}  // swganh::object

#endif  // SWGANH_OBJECT_OBJECT_MESSAGE_BUILDER_H_

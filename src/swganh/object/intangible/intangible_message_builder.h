
#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace intangible {

    class Intangible;

    class IntangibleMessageBuilder : public ObjectMessageBuilder
    {
    public:
        IntangibleMessageBuilder(anh::EventDispatcher* dispatcher) :
            ObjectMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        static void BuildStfDetailDelta(std::shared_ptr<Intangible> intangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(std::shared_ptr<Intangible> intangible);
    };

}}}  // swganh::object::intangible

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_


#ifndef SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace intangible {

    class Intangible;

    class IntangibleMessageBuilder
    {
    public:
        static void BuildStfDetailDelta(Intangible* object);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(Intangible* object);
    };

}}}  // swganh::object::intangible

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_


#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_

#include <cstdint>
#include <memory>
#include <boost/optional.hpp>

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace tangible {

    class Tangible;
    class TangibleMessageBuilder
    {
    public:
        // deltas
        static void BuildCustomizationDelta(Tangible* tangible);
        static void BuildComponentCustomizationDelta(Tangible* tangible);
        static void BuildOptionsMaskDelta(Tangible* tangible);
        static void BuildIncapTimerDelta(Tangible* tangible);
        static void BuildConditionDamageDelta(Tangible* tangible);
        static void BuildMaxConditionDelta(Tangible* tangible);
        static void BuildStaticDelta(Tangible* tangible);
        static void BuildDefendersDelta(Tangible* tangible);
        
        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(Tangible* tangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(Tangible* tangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline7(Tangible* tangible);
    };

}}}  // swganh::object::tangible

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_

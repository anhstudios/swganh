
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

    class BaseTangible;
    class TangibleMessageBuilder
    {
    public:
        // deltas
        static void BuildCustomizationDelta(BaseTangible* tangible);
        static void BuildComponentCustomizationDelta(BaseTangible* tangible, uint8_t subType, uint32_t crc);
        static void BuildOptionsMaskDelta(BaseTangible* tangible);
        static void BuildIncapTimerDelta(BaseTangible* tangible);
        static void BuildConditionDamageDelta(BaseTangible* tangible);
        static void BuildMaxConditionDelta(BaseTangible* tangible);
        static void BuildStaticDelta(BaseTangible* tangible);
        static void BuildDefendersDelta(BaseTangible* tangible, uint8_t subtype, uint64_t defender);
        static void BuildNewDefendersDelta(BaseTangible* tangible);
        
        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(BaseTangible* tangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(BaseTangible* tangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline7(BaseTangible* tangible);
    };

}}}  // swganh::object::tangible

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_

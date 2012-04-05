
#ifndef SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_

#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace object {
namespace tangible {

    class Tangible;
    class TangibleMessageBuilder : public ObjectMessageBuilder
    {
    public:
        TangibleMessageBuilder(anh::EventDispatcher* dispatcher) :
            ObjectMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        virtual void SendBaselines(std::shared_ptr<Tangible> tangible);
        // deltas
        static void BuildCustomizationDelta(std::shared_ptr<Tangible> tangible);
        static void BuildComponentCustomizationDelta(std::shared_ptr<Tangible> tangible);
        static void BuildOptionsMaskDelta(std::shared_ptr<Tangible> tangible);
        static void BuildIncapTimerDelta(std::shared_ptr<Tangible> tangible);
        static void BuildConditionDamageDelta(std::shared_ptr<Tangible> tangible);
        static void BuildMaxConditionDelta(std::shared_ptr<Tangible> tangible);
        static void BuildStaticDelta(std::shared_ptr<Tangible> tangible);
        static void BuildDefendersDelta(std::shared_ptr<Tangible> tangible);
        
        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(std::shared_ptr<Tangible> tangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(std::shared_ptr<Tangible> tangible);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline7(std::shared_ptr<Tangible> tangible);
    private:
        typedef anh::ValueEvent<std::shared_ptr<Tangible>> TangibleEvent;
    };

}}}  // swganh::object::tangible

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_

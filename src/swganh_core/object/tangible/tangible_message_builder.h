// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/object_message_builder.h"

namespace swganh {
namespace object {


    class Tangible;
    class TangibleMessageBuilder : public ObjectMessageBuilder
    {
    public:
        TangibleMessageBuilder(swganh::EventDispatcher* dispatcher) :
            ObjectMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        virtual void SendBaselines(const std::shared_ptr<Tangible>& tangible, const std::shared_ptr<swganh::observer::ObserverInterface>& controller);
        // deltas
        static void BuildCustomizationDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildComponentCustomizationDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildOptionsMaskDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildIncapTimerDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildConditionDamageDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildMaxConditionDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildStaticDelta(const std::shared_ptr<Tangible>& tangible);
        static void BuildDefendersDelta(const std::shared_ptr<Tangible>& tangible);
        
        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Tangible>& tangible);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Tangible>& tangible);
        static swganh::messages::BaselinesMessage BuildBaseline7(const std::shared_ptr<Tangible>& tangible);
    private:
        typedef swganh::ValueEvent<std::shared_ptr<Tangible>> TangibleEvent;
    };

}}  // swganh::object

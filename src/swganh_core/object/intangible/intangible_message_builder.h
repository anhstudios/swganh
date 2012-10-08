// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <boost/optional.hpp>

#include "swganh_core/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {


    class Intangible;

    class IntangibleMessageBuilder : public ObjectMessageBuilder
    {
    public:
        IntangibleMessageBuilder(swganh::EventDispatcher* dispatcher) :
            ObjectMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();

		virtual void SendBaselines(const std::shared_ptr<Intangible>& intangible, const std::shared_ptr<swganh::observer::ObserverInterface>& observer);

		static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Intangible>& intangible);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Intangible>& intangible);
    };

}}  // swganh::object

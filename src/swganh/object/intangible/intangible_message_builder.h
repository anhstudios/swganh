// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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

		virtual void SendBaselines(const std::shared_ptr<Intangible>& intangible, const std::shared_ptr<anh::observer::ObserverInterface>& observer);

		static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Intangible>& intangible);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Intangible>& intangible);
    };

}}}  // swganh::object::intangible

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_

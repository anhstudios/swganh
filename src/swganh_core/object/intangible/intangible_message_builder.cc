// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "intangible_message_builder.h"

#include <cstdint>

#include "swganh_core/object/object_events.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/intangible/intangible.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

void IntangibleMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Intangible::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto observer_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Intangible>(observer_event->object), observer_event->observer);
    });
}

boost::optional<BaselinesMessage> IntangibleMessageBuilder::BuildBaseline3(const std::shared_ptr<Intangible>& intangible, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(intangible, lock, intangible->Object::VIEW_3, 5);
	message.data.append((*ObjectMessageBuilder::BuildBaseline3(intangible, lock)).data);
	message.data.write(intangible->GetGenericInt(lock)); 
	return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> IntangibleMessageBuilder::BuildBaseline6(const shared_ptr<Intangible>& intangible, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(intangible, lock, intangible->Object::VIEW_6, 1);
	message.data.append((*ObjectMessageBuilder::BuildBaseline6(intangible,lock)).data);
    return BaselinesMessage(std::move(message));
}

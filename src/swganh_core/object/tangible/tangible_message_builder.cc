// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tangible_message_builder.h"

#include <algorithm>

#include "tangible.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;


void TangibleMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Tangible::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Tangible>(controller_event->object), controller_event->observer);
    });
    event_dispatcher->Subscribe("Tangible::Customization", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildCustomizationDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::ComponentCustomization", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildComponentCustomizationDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::OptionsMask", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildOptionsMaskDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::IncapTimer", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildIncapTimerDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::ConditionDamage", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildConditionDamageDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::MaxCondition", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildMaxConditionDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::Static", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildStaticDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::Customization", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildCustomizationDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Tangible::Defenders", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        BuildDefendersDelta(value_event->Get());
    });
}
void TangibleMessageBuilder::SendBaselines(const shared_ptr<Tangible>& tangible, const shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    tangible->AddBaselineToCache(&BuildBaseline3(tangible));
    tangible->AddBaselineToCache(&BuildBaseline6(tangible));
    tangible->AddBaselineToCache(&BuildBaseline7(tangible));

    for (auto& baseline : tangible->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(tangible, observer);
}
void TangibleMessageBuilder::BuildCustomizationDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 4);
        message.data.write<std::string>(tangible->GetCustomization());

        tangible->AddDeltasUpdate(&message);
    }
}
void TangibleMessageBuilder::BuildComponentCustomizationDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 5);
        tangible->GetComponentCustomization().Serialize(message);
        tangible->AddDeltasUpdate(&message);
    }
    else
        tangible->GetComponentCustomization().ClearDeltas();
}
void TangibleMessageBuilder::BuildOptionsMaskDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 6);
        message.data.write(tangible->GetOptionsMask());
        tangible->AddDeltasUpdate(&message);
    }
}
void TangibleMessageBuilder::BuildIncapTimerDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 7);
        message.data.write(tangible->GetIncapTimer());
        tangible->AddDeltasUpdate(&message);
    }
}
void TangibleMessageBuilder::BuildConditionDamageDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 8);
        message.data.write(tangible->GetCondition());
        tangible->AddDeltasUpdate(&message);
    }
}
void TangibleMessageBuilder::BuildMaxConditionDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 9);
        message.data.write(tangible->GetMaxCondition());
        tangible->AddDeltasUpdate(&message);
    }
}
void TangibleMessageBuilder::BuildStaticDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 10);
        message.data.write(tangible->IsStatic());
        tangible->AddDeltasUpdate(&message);
    }
}

void TangibleMessageBuilder::BuildDefendersDelta(const shared_ptr<Tangible>& tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_6, 1);
        tangible->GetDefenders().Serialize(message);
        tangible->AddDeltasUpdate(&message);
    }
}

// baselines
BaselinesMessage TangibleMessageBuilder::BuildBaseline3(const shared_ptr<Tangible>& tangible)
{
    auto message = CreateBaselinesMessage(tangible, Object::VIEW_3, 11);
    message.data.append(ObjectMessageBuilder::BuildBaseline3(tangible).data);
    message.data.write<std::string>(tangible->GetCustomization());
    tangible->GetComponentCustomization().Serialize(message);
    message.data.write<uint32_t>(tangible->GetOptionsMask());
    message.data.write<uint32_t>(tangible->GetIncapTimer());
    message.data.write<uint32_t>(tangible->GetCondition());
    message.data.write<uint32_t>(tangible->GetMaxCondition());
    message.data.write<uint8_t>(tangible->IsStatic() ? 1 : 0);
    return BaselinesMessage(std::move(message));
}
BaselinesMessage TangibleMessageBuilder::BuildBaseline6(const shared_ptr<Tangible>& tangible)
{
    auto message = CreateBaselinesMessage(tangible, Object::VIEW_6, 2);
    message.data.append(ObjectMessageBuilder::BuildBaseline6(tangible).data);
    tangible->GetDefenders().Serialize(message);
    return BaselinesMessage(std::move(message));
}
BaselinesMessage TangibleMessageBuilder::BuildBaseline7(const shared_ptr<Tangible>& tangible)
{
    auto message = CreateBaselinesMessage(tangible, Object::VIEW_7, 2);
    message.data.write<uint64_t>(0);
    message.data.write<uint64_t>(0);
    return BaselinesMessage(std::move(message));
}
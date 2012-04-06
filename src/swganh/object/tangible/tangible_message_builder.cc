#include "tangible_message_builder.h"

#include <algorithm>

#include "tangible.h"
#include "swganh/messages/scene_end_baselines.h"
#include "swganh/messages/deltas_message.h"
#include "swganh/messages/baselines_message.h"

using namespace anh;
using namespace std;
using namespace swganh::object::tangible;
using namespace swganh::messages;


void TangibleMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Tangible::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<TangibleEvent>(incoming_event);
        SendBaselines(value_event->Get());
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
void TangibleMessageBuilder::SendBaselines(std::shared_ptr<Tangible> tangible)
{
    auto controller = tangible->GetController();
    tangible->AddBaselineToCache(BuildBaseline3(tangible));
    tangible->AddBaselineToCache(BuildBaseline6(tangible));
    tangible->AddBaselineToCache(BuildBaseline7(tangible));

    if (tangible->HasController())
    {
        for (auto& baseline : tangible->GetBaselines())
        {
            controller->Notify(baseline);
        }
        
        SendEndBaselines(tangible);
    }
    else if (tangible->HasObservers())
    {
        for (auto& baseline : tangible->GetBaselines())
        {
            tangible->NotifyObservers(baseline);
        }
        
        SendEndBaselines(tangible, true);     
    }
}
void TangibleMessageBuilder::BuildCustomizationDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 4);
        message.data.write<std::string>(tangible->GetCustomization());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildComponentCustomizationDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 5);
        tangible->GetComponentCustomization().Serialize(message);
        tangible->AddDeltasUpdate(move(message));
    }
    else
        tangible->GetComponentCustomization().ClearDeltas();
}
void TangibleMessageBuilder::BuildOptionsMaskDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 6);
        message.data.write(tangible->GetOptionsMask());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildIncapTimerDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 7);
        message.data.write(tangible->GetIncapTimer());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildConditionDamageDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 8);
        message.data.write(tangible->GetCondition());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildMaxConditionDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 9);
        message.data.write(tangible->GetMaxCondition());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildStaticDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_3, 10);
        message.data.write(tangible->IsStatic());
        tangible->AddDeltasUpdate(move(message));
    }
}

void TangibleMessageBuilder::BuildDefendersDelta(shared_ptr<Tangible> tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(tangible, Object::VIEW_6, 1);
        tangible->GetDefenders().Serialize(message);
        tangible->AddDeltasUpdate(move(message));
    }
}

// baselines
BaselinesMessage TangibleMessageBuilder::BuildBaseline3(shared_ptr<Tangible> tangible)
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
BaselinesMessage TangibleMessageBuilder::BuildBaseline6(shared_ptr<Tangible> tangible)
{
    auto message = CreateBaselinesMessage(tangible, Object::VIEW_6, 2);
    message.data.append(ObjectMessageBuilder::BuildBaseline6(tangible).data);
    tangible->GetDefenders().Serialize(message);
    return BaselinesMessage(std::move(message));
}
BaselinesMessage TangibleMessageBuilder::BuildBaseline7(shared_ptr<Tangible> tangible)
{
    auto message = CreateBaselinesMessage(tangible, Object::VIEW_7, 2);
    message.data.write<uint64_t>(0);
    message.data.write<uint64_t>(0);
    return BaselinesMessage(std::move(message));
}
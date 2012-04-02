#include "tangible_message_builder.h"

#include <algorithm>

#include "tangible.h"
#include "swganh/messages/deltas_message.h"
#include "swganh/messages/baselines_message.h"

using namespace std;
using namespace swganh::object::tangible;
using namespace swganh::messages;


void TangibleMessageBuilder::BuildCustomizationDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 4);
        message.data.write<std::string>(tangible->GetCustomization());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildComponentCustomizationDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 5);
        tangible->GetComponentCustomization().Serialize(message);
        tangible->AddDeltasUpdate(move(message));
    }
    else
        tangible->GetComponentCustomization().ClearDeltas();
}
void TangibleMessageBuilder::BuildOptionsMaskDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 6);
        message.data.write(tangible->GetOptionsMask());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildIncapTimerDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 7);
        message.data.write(tangible->GetIncapTimer());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildConditionDamageDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 8);
        message.data.write(tangible->GetCondition());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildMaxConditionDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 9);
        message.data.write(tangible->GetMaxCondition());
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildStaticDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 10);
        message.data.write(tangible->IsStatic());
        tangible->AddDeltasUpdate(move(message));
    }
}

void TangibleMessageBuilder::BuildDefendersDelta(Tangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_6, 1);
        tangible->GetDefenders().Serialize(message);
        tangible->AddDeltasUpdate(move(message));
    }
}

// baselines
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline3(Tangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(tangible->Object::VIEW_3, 11);
    message.data.append(tangible->Object::GetBaseline3().get().data);
    message.data.write<std::string>(tangible->GetCustomization());
    tangible->GetComponentCustomization().Serialize(message);
    message.data.write<uint32_t>(tangible->GetOptionsMask());
    message.data.write<uint32_t>(tangible->GetIncapTimer());
    message.data.write<uint32_t>(tangible->GetCondition());
    message.data.write<uint32_t>(tangible->GetMaxCondition());
    message.data.write<uint8_t>(tangible->IsStatic() ? 1 : 0);
    return boost::optional<BaselinesMessage>(std::move(message));
}
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline6(Tangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(Object::VIEW_6, 2);
    message.data.append(tangible->Object::GetBaseline6().get().data);
    tangible->GetDefenders().Serialize(message);
    return boost::optional<BaselinesMessage>(std::move(message));
}
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline7(Tangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(Object::VIEW_7, 2);
    message.data.write<uint64_t>(0);
    message.data.write<uint64_t>(0);
    return boost::optional<BaselinesMessage>(std::move(message));
}
#include "tangible_message_builder.h"

#include <algorithm>

#include "base_tangible.h"
#include "swganh/messages/deltas_message.h"
#include "swganh/messages/baselines_message.h"

using namespace std;
using namespace swganh::object::tangible;
using namespace swganh::messages;


void TangibleMessageBuilder::BuildCustomizationDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 4);
        message.data.write<std::string>(tangible->customization_);

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildComponentCustomizationDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 5);
        tangible->component_customization_list_.Serialize(message);
        tangible->AddDeltasUpdate(move(message));
    }
    else
        tangible->component_customization_list_.ClearDeltas();
}
void TangibleMessageBuilder::BuildOptionsMaskDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 6);
        message.data.write(tangible->options_bitmask_);
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildIncapTimerDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 7);
        message.data.write(tangible->incap_timer_);
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildConditionDamageDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 8);
        message.data.write(tangible->condition_damage_);
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildMaxConditionDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 9);
        message.data.write(tangible->max_condition_);
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildStaticDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        uint8_t val = MOVEABLE;
        if (tangible->is_static_)
            val = STATIC;
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 10);
        message.data.write(val);
        tangible->AddDeltasUpdate(move(message));
    }
}

void TangibleMessageBuilder::BuildDefendersDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_6, 1);
        tangible->defender_list_.Serialize(message);
        tangible->AddDeltasUpdate(move(message));
    }
    else
        tangible->component_customization_list_.ClearDeltas();
}

// baselines
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline3(BaseTangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(tangible->Object::VIEW_3, 11);
    message.data.append(tangible->Object::GetBaseline3().get().data);
    message.data.write<std::string>(tangible->customization_);
    tangible->component_customization_list_.Serialize(message);
    message.data.write<uint32_t>(tangible->options_bitmask_);
    message.data.write<uint32_t>(tangible->incap_timer_);
    message.data.write<uint32_t>(tangible->condition_damage_);
    message.data.write<uint32_t>(tangible->max_condition_);
    message.data.write<uint8_t>(tangible->is_static_ ? 1 : 0);
    return boost::optional<BaselinesMessage>(std::move(message));
}
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline6(BaseTangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(Object::VIEW_6, 2);
    message.data.append(tangible->Object::GetBaseline6().get().data);
    tangible->defender_list_.Serialize(message);
    return boost::optional<BaselinesMessage>(std::move(message));
}
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline7(BaseTangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(Object::VIEW_7, 2);
    message.data.write<uint64_t>(0);
    message.data.write<uint64_t>(0);
    return boost::optional<BaselinesMessage>(std::move(message));
}
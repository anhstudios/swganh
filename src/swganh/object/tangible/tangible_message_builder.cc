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
        message.data.write(tangible->GetCustomization());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildComponentCustomizationDelta(BaseTangible* tangible, uint8_t subType, uint32_t crc)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 5);
        message.data.write(tangible->component_customization_list_.size());
        // list counter
        message.data.write(tangible->component_customization_counter_++);
        // subtype
        message.data.write<uint8_t>(subType);
        switch (subType)
        {
            case 0:
            case 1:
                message.data.write(crc);
                break;
            case 2:
                break;
            default:
                return;
        }
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildOptionsMaskDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 6);
        message.data.write(tangible->GetOptionsMask());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildIncapTimerDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 7);
        message.data.write(tangible->GetIncapTimer());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildConditionDamageDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 8);
        message.data.write(tangible->GetCondition());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildMaxConditionDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3, 9);
        message.data.write(tangible->GetMaxCondition());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildStaticDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        uint8_t val = MOVEABLE;
        if (tangible->IsStatic())
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
}

// baselines
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline3(BaseTangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(tangible->Object::VIEW_3, 11);
    
    // base data
    message.data.append(tangible->Object::GetBaseline3().get().data);
    message.data.write(tangible->GetCustomization());
    message.data.write(tangible->component_customization_list_.size());
    message.data.write(tangible->component_customization_counter_);
    for_each(tangible->GetComponentCustomization().begin(), tangible->GetComponentCustomization().end(), [&message](uint32_t crc){
        message.data.write(crc);
    });
    message.data.write(tangible->GetOptionsMask());
    message.data.write(tangible->GetIncapTimer());
    message.data.write(tangible->GetCondition());
    message.data.write(tangible->GetMaxCondition());
    uint8_t static_val = MOVEABLE;
    if (tangible->IsStatic())
        static_val = STATIC;
    message.data.write<uint8_t>(0);
    
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
    // always seen 0, used for crafting tool
    message.data.write<uint64_t>(0);
    message.data.write<uint64_t>(0);

    return boost::optional<BaselinesMessage>(std::move(message));
}
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
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(4);
        message.data.write(tangible->GetCustomization());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildComponentCustomizationDelta(BaseTangible* tangible, uint8_t subType, uint32_t crc)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(5);
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
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(6);
        message.data.write(tangible->GetOptionsMask());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildIncapTimerDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(7);
        message.data.write(tangible->GetIncapTimer());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildConditionDamageDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(8);
        message.data.write(tangible->GetCondition());

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildMaxConditionDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(9);
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
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_3);
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(10);
        message.data.write(val);

        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildDefendersDelta(BaseTangible* tangible, uint8_t subType, uint16_t index, uint64_t defender)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_6);
        auto defender_list = tangible->GetDefenders();
        // update count
        message.data.write<uint16_t>(1);
        // update type
        message.data.write<uint16_t>(1);
        message.data.write(defender_list.size());
        // list counter
        message.data.write(tangible->defender_list_counter_++);
        // subtype
        message.data.write<uint8_t>(subType);
        switch (subType)
        {
        // remove
        case 0:
            message.data.write(index);
            break;
        // Add
        case 1:
        // Change
        case 2:
            // index
            message.data.write(index);
            message.data.write(defender);
            break;
        
        // Clear
        case 4:
            break;
        }
        tangible->AddDeltasUpdate(move(message));
    }
}
void TangibleMessageBuilder::BuildNewDefendersDelta(BaseTangible* tangible)
{
    if (tangible->HasObservers())
    {
        DeltasMessage message = tangible->CreateDeltasMessage(Object::VIEW_6);
        auto defender_list = tangible->GetDefenders();
        // update count
        message.data.write<uint16_t>(defender_list.size());
        // update type
        message.data.write<uint16_t>(1);
        message.data.write(defender_list.size());
        // list counter
        message.data.write(tangible->defender_list_counter_++);
        // Reset All
        message.data.write<uint8_t>(3);
        // loop through all defenders
        for_each(begin(defender_list), end(defender_list), [&message] (uint64_t defender) {
            message.data.write(defender);
        });
        // clear the free list
        tangible->defender_index_free_list_.clear();

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
    
    // server id
    message.data.append(tangible->Object::GetBaseline6().get().data);
    // defender list
    message.data.write(tangible->defender_list_.size());
    message.data.write(tangible->defender_list_counter_);
    for_each(begin(tangible->GetDefenders()), end(tangible->GetDefenders()), [&message](uint64_t defender) {
        message.data.write(defender);
    });

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
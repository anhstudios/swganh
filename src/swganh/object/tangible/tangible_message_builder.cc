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
        message.data.write(tangible->GetComponentCustomization().size());
        // list counter
        message.data.write(tangible->GetComponentCustomizationCounter());
        // subtype
        message.data.write<uint8_t>(subType);
        if (subType == 0 || subType == 1)
        {
            tangible->IncrementComponentCustomizationCounter();
            message.data.write(crc);
        }
        // reset all
        else if (subType == 2)
        {
            tangible->ClearComponentCustomizationCounter();
        }
        else
        {
            // dont add to deltas update
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
void TangibleMessageBuilder::BuildDefendersDelta(BaseTangible* tangible, uint8_t subType, uint64_t defender)
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
        message.data.write(tangible->GetDefendersCounter());
        // subtype
        message.data.write<uint8_t>(subType);
        // find defender
        auto found = tangible->FindDefender(defender);
        switch (subType)
        {
        // remove
        case 0:
            message.data.write(found - begin(defender_list));
            break;
        // Add
        case 1:
            // index
            message.data.write(found - begin(defender_list));
            message.data.write(defender);
            break;
        // Change
        case 2:
            // index
            message.data.write(found - begin(defender_list));
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
        message.data.write(1);
        // Reset All
        message.data.write<uint8_t>(3);
        // loop through all defenders
        for_each(begin(defender_list), end(defender_list), [&message] (uint64_t defender) {
            message.data.write(defender);
        });

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
    message.data.write(tangible->GetComponentCustomization().size());
    message.data.write(tangible->GetComponentCustomizationCounter());
    for_each(begin(tangible->GetComponentCustomization()), end(tangible->GetComponentCustomization()), [&message](uint32_t crc){
        message.data.write(crc);
    });
    message.data.write(tangible->GetOptionsMask());
    message.data.write(tangible->GetIncapTimer());
    message.data.write(tangible->GetCondition());
    message.data.write(tangible->GetMaxCondition());
    uint8_t static_val = MOVEABLE;
    if (tangible->IsStatic())
        static_val = STATIC;
    message.data.write<uint8_t>(static_val);
    
    return boost::optional<BaselinesMessage>(std::move(message));
}
boost::optional<BaselinesMessage> TangibleMessageBuilder::BuildBaseline6(BaseTangible* tangible)
{
    auto message = tangible->CreateBaselinesMessage(Object::VIEW_6, 2);
    
    // server id
    message.data.append(tangible->Object::GetBaseline6().get().data);
    // defender list
    message.data.write(tangible->GetDefenders().size());
    message.data.write(tangible->GetDefendersCounter());
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
// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/intangible/intangible_message_builder.h"

#include <cstdint>

#include "swganh/object/object.h"
#include "swganh/object/intangible/intangible.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::intangible;

void IntangibleMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register Event Handlers for Intangible
}

void IntangibleMessageBuilder::BuildStfDetailDelta(const shared_ptr<Intangible>& object)
{
    // Only build a message if there are observers.
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_6, 1);
        message.data.write(object->GetStfDetailFile());
        message.data.write<uint32_t>(0);
        message.data.write(object->GetStfDetailString());

        object->AddDeltasUpdate(message);                
    }
}
BaselinesMessage IntangibleMessageBuilder::BuildBaseline6(const shared_ptr<Intangible>& object)
{
    auto message = CreateBaselinesMessage(object, object->Object::VIEW_6, 1);
    message.data.write(0);
    message.data.write(object->GetStfDetailFile());
    message.data.write(0);
    message.data.write(object->GetStfDetailString());
    return BaselinesMessage(std::move(message));
}
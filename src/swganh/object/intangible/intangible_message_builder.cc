
#include "swganh/object/intangible/intangible_message_builder.h"

#include <cstdint>

#include "swganh/object/object.h"
#include "swganh/object/intangible/intangible.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::intangible;

void IntangibleMessageBuilder::BuildStfDetailDelta(Intangible* object)
{
    // Only build a message if there are observers.
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_6, 1);
        message.data.write(object->GetStfDetailFile());
        message.data.write<uint32_t>(0);
        message.data.write(object->GetStfDetailString());

        object->AddDeltasUpdate(message);                
    }
}
boost::optional<BaselinesMessage> IntangibleMessageBuilder::BuildBaseline6(Intangible* object)
{
    auto message = object->CreateBaselinesMessage(object->Object::VIEW_6, 1);
    message.data.write(0);
    message.data.write(object->GetStfDetailFile());
    message.data.write(0);
    message.data.write(object->GetStfDetailString());
    return boost::optional<BaselinesMessage>(std::move(message));
}
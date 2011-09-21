
#include "swganh/object/intangible/intangible_baselines_builder.h"

#include <cstdint>

#include "swganh/object/object.h"
#include "swganh/object/intangible/intangible.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::intangible;

void IntangibleBaselinesBuilder::BuildStfDetailDelta(Intangible* object)
{
    // Only build a message if there are observers.
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(1); // update type
        message.data.write(object->GetStfDetailFile());
        message.data.write<uint32_t>(0);
        message.data.write(object->GetStfDetailString());

        object->AddDeltasUpdate(message);                
    }
}
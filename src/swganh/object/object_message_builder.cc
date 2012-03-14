
#include "swganh/object/object_message_builder.h"

#include <cstdint>

#include "swganh/object/object.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using boost::optional;

void ObjectMessageBuilder::BuildComplexityDelta(Object* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 0);
        message.data.write(object->complexity_);

        object->AddDeltasUpdate(message);                
    }
}
void ObjectMessageBuilder::BuildStfNameDelta(Object* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 1);
        message.data.write(object->stf_name_file_);
        message.data.write<uint32_t>(0);
        message.data.write(object->stf_name_string_);

        object->AddDeltasUpdate(message);                
    }
}
void ObjectMessageBuilder::BuildCustomNameDelta(Object* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 2);
        message.data.write(object->custom_name_);

        object->AddDeltasUpdate(message);                
    }
}
void ObjectMessageBuilder::BuildVolumeDelta(Object* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_3, 3);
        message.data.write(object->volume_);

        object->AddDeltasUpdate(message);                
    }
}

void ObjectMessageBuilder::BuildServerIDDelta(Object* object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = object->CreateDeltasMessage(Object::VIEW_6, 1);
        //@TODO: Add server id
        message.data.write(0);

        object->AddDeltasUpdate(message);                
    }
}

optional<BaselinesMessage> ObjectMessageBuilder::BuildBaseline3(Object* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_3);
    message.data.write(object->complexity_);
    message.data.write(object->stf_name_file_);
    // spacer
    message.data.write<uint32_t>(0);
    message.data.write(object->stf_name_string_);
    message.data.write(object->custom_name_);
    message.data.write(object->volume_);

    return optional<BaselinesMessage>(move(message));
}

optional<BaselinesMessage> ObjectMessageBuilder::BuildBaseline6(Object* object)
{
    auto message = object->CreateBaselinesMessage(Object::VIEW_6);
    // server ID
    message.data.write(0);

    return optional<BaselinesMessage>(move(message));
}

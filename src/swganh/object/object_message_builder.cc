#include "object_message_builder.h"

#include <cstdint>

#include "swganh/object/object.h"
#include "swganh/messages/baselines_message.h"
#include "swganh/messages/deltas_message.h"

using namespace anh;
using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

using boost::optional;

void ObjectMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Object::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        //CreateBaselinesMessage(value_event->Get());
        // loop through all baselines and send out...
    });
    event_dispatcher->Subscribe("Object::CustomName", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildCustomNameDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Object::StfName", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildStfNameDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Object::Complexity", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildComplexityDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Object::Volume", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildVolumeDelta(value_event->Get());
    });
}

void ObjectMessageBuilder::BuildComplexityDelta(shared_ptr<Object> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 0);
        message.data.write(object->GetComplexity());

        object->AddDeltasUpdate(message);                
    }
}
void ObjectMessageBuilder::BuildStfNameDelta(shared_ptr<Object> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 1);
        message.data.write(object->GetStfNameFile());
        message.data.write<uint32_t>(0);
        message.data.write(object->GetStfNameString());

        object->AddDeltasUpdate(message);                
    }
}
void ObjectMessageBuilder::BuildCustomNameDelta(shared_ptr<Object> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 2);
        message.data.write(object->GetCustomName());

        object->AddDeltasUpdate(message);                
    }
}
void ObjectMessageBuilder::BuildVolumeDelta(shared_ptr<Object> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 3);
        message.data.write(object->GetVolume());

        object->AddDeltasUpdate(message);                
    }
}

void ObjectMessageBuilder::BuildServerIDDelta(shared_ptr<Object> object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_6, 1);
        //@TODO: Add server id
        message.data.write(0);

        object->AddDeltasUpdate(message);                
    }
}

optional<BaselinesMessage> ObjectMessageBuilder::BuildBaseline3(shared_ptr<Object> object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_3);
    message.data.write(object->GetComplexity());
    message.data.write(object->GetStfNameFile());
    // spacer
    message.data.write<uint32_t>(0);
    message.data.write(object->GetStfNameString());
    message.data.write(object->GetCustomName());
    message.data.write(object->GetVolume());

    return optional<BaselinesMessage>(move(message));
}

optional<BaselinesMessage> ObjectMessageBuilder::BuildBaseline6(shared_ptr<Object> object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_6);
    // server ID
    message.data.write(0);

    return optional<BaselinesMessage>(move(message));
}

BaselinesMessage ObjectMessageBuilder::CreateBaselinesMessage(shared_ptr<Object> object, uint8_t view_type, uint16_t opcount)
{
    BaselinesMessage message;
    message.object_id = object->GetObjectId();
    message.object_type = object->GetType();
    message.view_type = view_type;
    message.object_opcount = opcount;

    return message;
}

DeltasMessage ObjectMessageBuilder::CreateDeltasMessage(shared_ptr<Object> object, uint8_t view_type, uint16_t update_type, uint16_t update_count)
{
    DeltasMessage message;
    message.object_id = object->GetObjectId();
    message.object_type = object->GetType();
    message.view_type = view_type;
    message.update_count = update_count;
    message.update_type = update_type;
    return message;
}
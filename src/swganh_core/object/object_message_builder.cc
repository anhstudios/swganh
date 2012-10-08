// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object_message_builder.h"

#include <cstdint>

#include "swganh_core/object/object.h"
#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/update_containment_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

void ObjectMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Object::CustomName", [this] (const shared_ptr<EventInterface>& incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildCustomNameDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Object::StfName", [this] (const shared_ptr<EventInterface>& incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildStfNameDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Object::Complexity", [this] (const shared_ptr<EventInterface>& incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildComplexityDelta(value_event->Get());
    });
    event_dispatcher->Subscribe("Object::Volume", [this] (const shared_ptr<EventInterface>& incoming_event)
    {
        auto value_event = static_pointer_cast<ObjectEvent>(incoming_event);
        BuildVolumeDelta(value_event->Get());
    });
}

void ObjectMessageBuilder::SendEndBaselines(const shared_ptr<Object>& object, const shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    swganh::messages::SceneEndBaselines scene_end_baselines;
    scene_end_baselines.object_id = object->GetObjectId();
	observer->Notify(&scene_end_baselines);
}

void ObjectMessageBuilder::BuildComplexityDelta(const shared_ptr<Object>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 0);
        message.data.write(object->GetComplexity());

        object->AddDeltasUpdate(&message);                
    }
}
void ObjectMessageBuilder::BuildStfNameDelta(const shared_ptr<Object>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 1);
        message.data.write(object->GetStfNameFile());
        message.data.write<uint32_t>(0);
        message.data.write(object->GetStfNameString());

        object->AddDeltasUpdate(&message);                
    }
}
void ObjectMessageBuilder::BuildCustomNameDelta(const shared_ptr<Object>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 2);
        message.data.write(object->GetCustomName());

        object->AddDeltasUpdate(&message);                
    }
}
void ObjectMessageBuilder::BuildVolumeDelta(const shared_ptr<Object>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_3, 3);
        message.data.write(object->GetVolume());

        object->AddDeltasUpdate(&message);                
    }
}

void ObjectMessageBuilder::BuildServerIDDelta(const shared_ptr<Object>& object)
{
    if (object->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(object, Object::VIEW_6, 1);
        
		message.data.write(object->GetSceneId());

        object->AddDeltasUpdate(&message);                
    }
}

BaselinesMessage ObjectMessageBuilder::BuildBaseline3(const shared_ptr<Object>& object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_3);
    message.data.write(object->GetComplexity());
    message.data.write(object->GetStfNameFile());
    message.data.write<uint32_t>(0); // spacer
    message.data.write(object->GetStfNameString());
    message.data.write(object->GetCustomName());
    message.data.write(object->GetVolume());
    return BaselinesMessage(move(message));
}

BaselinesMessage ObjectMessageBuilder::BuildBaseline6(const shared_ptr<Object>& object)
{
    auto message = CreateBaselinesMessage(object, Object::VIEW_6);
	message.data.write(object->GetSceneId());
    return BaselinesMessage(move(message));
}

BaselinesMessage ObjectMessageBuilder::CreateBaselinesMessage(const shared_ptr<Object>& object, uint8_t view_type, uint16_t opcount)
{
    BaselinesMessage message;
    message.object_id = object->GetObjectId();
    message.object_type = object->GetType();
    message.view_type = view_type;
    message.object_opcount = opcount;

    return message;
}

DeltasMessage ObjectMessageBuilder::CreateDeltasMessage(const shared_ptr<Object>& object, uint8_t view_type, uint16_t update_type, uint16_t update_count)
{
    DeltasMessage message;
    message.object_id = object->GetObjectId();
    message.object_type = object->GetType();
    message.view_type = view_type;
    message.update_count = update_count;
    message.update_type = update_type;
    return message;
}
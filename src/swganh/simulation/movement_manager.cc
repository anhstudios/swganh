
#include "swganh/simulation/movement_manager.h"

#include "anh/logger.h"

#include "anh/event_dispatcher.h"

#include "swganh/object/object.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/object_controller.h"

#include "swganh/messages/controllers/data_transform.h"
#include "swganh/messages/controllers/data_transform_with_parent.h"
#include "swganh/messages/update_containment_message.h"
#include "swganh/messages/update_transform_message.h"
#include "swganh/messages/update_transform_with_parent_message.h"

using namespace anh::event_dispatcher;
using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::simulation;

MovementManager::MovementManager(anh::EventDispatcher* event_dispatcher)
{
    RegisterEvents(event_dispatcher);
}

void MovementManager::HandleDataTransform(
    const shared_ptr<ObjectController>& controller, 
    const ObjControllerMessage& message)
{
    DataTransform transform;
    transform.Deserialize(message.data);
    
    auto object = controller->GetObject();
    
    if (!ValidateCounter_(object->GetObjectId(), transform.counter))
    {
        return;
    }

    counter_map_[object->GetObjectId()] = transform.counter;
    
    object->SetPosition(transform.position);
    object->SetOrientation(transform.orientation);
    
    SendUpdateDataTransformMessage(object);
}

void MovementManager::HandleDataTransformWithParent(
    const std::shared_ptr<ObjectController>& controller, 
    const ObjControllerMessage& message)
{
    throw std::runtime_error("Cell movement currently disabled");

    DataTransformWithParent transform;
    transform.Deserialize(message.data);
    
    auto object = controller->GetObject();
        
    if (!ValidateCounter_(object->GetObjectId(), transform.counter))
    {
        return;
    }

    counter_map_[object->GetObjectId()] = transform.counter;
    
    object->SetPosition(transform.position);
    object->SetOrientation(transform.orientation);
        
    SendUpdateDataTransformWithParentMessage(object);
}

void MovementManager::SendDataTransformMessage(const shared_ptr<Object>& object, uint32_t unknown)
{
    auto creature = static_pointer_cast<Creature>(object);

    DataTransform transform;
    transform.counter = ++counter_map_[object->GetObjectId()];
    transform.orientation = object->GetOrientation();
    transform.position = object->GetPosition();
    transform.speed = creature->GetWalkingSpeed();
    
    ObjControllerMessage message(unknown, move(transform));

    object->GetController()->Notify(message);
}

void MovementManager::SendUpdateDataTransformMessage(const shared_ptr<Object>& object)
{
    UpdateTransformMessage transform_update;
    transform_update.object_id = object->GetObjectId();
    transform_update.heading = object->GetHeading();
    transform_update.position = object->GetPosition();
    transform_update.update_counter = ++counter_map_[object->GetObjectId()];
    
    object->NotifyObservers(transform_update);
}

void MovementManager::SendDataTransformWithParentMessage(const shared_ptr<Object>& object, uint32_t unknown)
{    
    auto creature = static_pointer_cast<Creature>(object);

    DataTransformWithParent transform;
    transform.cell_id       = object->GetContainer()->GetObjectId();
    transform.counter       = ++counter_map_[object->GetObjectId()];
    transform.orientation   = object->GetOrientation();
    transform.position      = object->GetPosition();
    transform.speed         = creature->GetWalkingSpeed();
    
    ObjControllerMessage message(unknown, move(transform));

    object->GetController()->Notify(message);
}

void MovementManager::SendUpdateDataTransformWithParentMessage(const shared_ptr<Object>& object)
{    
    UpdateTransformWithParentMessage transform_update;
    transform_update.object_id = object->GetObjectId();
    transform_update.cell_id = object->GetContainer()->GetObjectId();
    transform_update.heading = object->GetHeading();
    transform_update.position = object->GetPosition();
    transform_update.update_counter = ++counter_map_[object->GetObjectId()];
    
    object->NotifyObservers(transform_update);
}

void MovementManager::RegisterEvents(anh::EventDispatcher* event_dispatcher)
{
    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<anh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();
        
        if (counter_map_.find(object->GetObjectId()) == counter_map_.end())
        {
            counter_map_[object->GetObjectId()] = 0;
        }

        if (object->GetContainer())
        {
            SendDataTransformWithParentMessage(object);
        }
        else
        {
            SendDataTransformMessage(object);
        }
    });
}

bool MovementManager::ValidateCounter_(uint64_t object_id, uint32_t counter)
{    
    return counter > counter_map_[object_id];
}


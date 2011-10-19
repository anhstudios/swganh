
#include "swganh/simulation/movement_manager.h"

#include <glog/logging.h>

#include "swganh/object/object.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/object_controller.h"

#include "swganh/messages/controllers/data_transform.h"
#include "swganh/messages/controllers/data_transform_with_parent.h"
#include "swganh/messages/update_containment_message.h"
#include "swganh/messages/update_transform_message.h"
#include "swganh/messages/update_transform_with_parent_message.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::simulation;

MovementManager::MovementManager()
{}

void MovementManager::HandleDataTransform(
    const shared_ptr<swganh::object::ObjectController>& controller, 
    const swganh::messages::ObjControllerMessage& message)
{
    DataTransform transform;
    transform.Deserialize(message.data);
    
    auto object = controller->GetObject();
    
    object->SetPosition(transform.position);
    object->SetOrientation(transform.orientation);
    
    UpdateTransformMessage transform_update;
    transform_update.object_id = object->GetObjectId();
    transform_update.heading = object->GetHeading();
    transform_update.position = object->GetPosition();
    transform_update.update_counter = transform.counter+1;
    
    object->NotifyObservers(transform_update);
}

void MovementManager::HandleDataTransformWithParent(
    const std::shared_ptr<swganh::object::ObjectController>& controller, 
    const swganh::messages::ObjControllerMessage& message)
{
    DataTransformWithParent transform;
    transform.Deserialize(message.data);
    
    auto object = controller->GetObject();
    
    object->SetPosition(transform.position);
    object->SetOrientation(transform.orientation);
        
    UpdateTransformWithParentMessage transform_update;
    transform_update.object_id = object->GetObjectId();
    transform_update.cell_id = transform.cell_id;
    transform_update.heading = object->GetHeading();
    transform_update.position = object->GetPosition();
    transform_update.update_counter = transform.counter+1;
    
    object->NotifyObservers(transform_update);
}

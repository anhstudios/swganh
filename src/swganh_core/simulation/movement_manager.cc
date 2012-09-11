// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "movement_manager.h"

#include "swganh/logger.h"

#include "swganh/event_dispatcher.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/service/service_manager.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh/observer/observer_interface.h"

#include "swganh_core/messages/update_containment_message.h"
#include "swganh_core/messages/update_transform_message.h"
#include "swganh_core/messages/update_transform_with_parent_message.h"

#include "swganh/simulation/spatial_provider_interface.h"
#include "swganh/simulation/simulation_service_interface.h"

using namespace swganh::event_dispatcher;
using namespace std;
using namespace swganh::messages;
using namespace swganh::messages::controllers;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::simulation;

MovementManager::MovementManager(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
	simulation_service_ = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");

	RegisterEvents(kernel_->GetEventDispatcher());
}

void MovementManager::HandleDataTransform(
    const shared_ptr<Object>& object, 
    DataTransform message)
{
    
    if (!ValidateCounter_(object->GetObjectId(), message.counter))
    {
        return;
    }

    counter_map_[object->GetObjectId()] = message.counter;
    
	glm::vec3 old_position = object->GetPosition();

	object->SetPosition(message.position);
    object->SetOrientation(message.orientation);

	//If the object was inside a container we need to move it out
	if(object->GetContainer() != spatial_provider_)
		object->GetContainer()->TransferObject(object, object, spatial_provider_);
	else
		spatial_provider_->UpdateObject(object, old_position, message.position);

    SendUpdateDataTransformMessage(object);
}

void MovementManager::HandleDataTransformWithParent(
    const shared_ptr<Object>& object, 
    DataTransformWithParent message)
{
    
	auto container = simulation_service_->GetObjectById(message.cell_id);
	if(container != nullptr)
	{
		if (!ValidateCounter_(object->GetObjectId(), message.counter))
		{
			return;
		}

		counter_map_[object->GetObjectId()] = message.counter;

		//Set the new position and orientation
		object->SetPosition(message.position);
		object->SetOrientation(message.orientation);
    
		//Perform the transfer
		if(object->GetContainer() != container)
			object->GetContainer()->TransferObject(object, object, container);

		//Send the update transform
		SendUpdateDataTransformWithParentMessage(object);
	}
	else
	{
		LOG(error) << "Cell ID: " << message.cell_id << " not found.";
	}
}

void MovementManager::SendDataTransformMessage(const shared_ptr<Object>& object, uint32_t unknown)
{
    auto creature = static_pointer_cast<Creature>(object);

    DataTransform transform;
    transform.counter = ++counter_map_[object->GetObjectId()];
    transform.orientation = object->GetOrientation();
    transform.position = object->GetPosition();
    transform.speed = creature->GetWalkingSpeed();

    object->GetController()->Notify(&transform);
}

void MovementManager::SendUpdateDataTransformMessage(const shared_ptr<Object>& object)
{
    UpdateTransformMessage transform_update;
    transform_update.object_id = object->GetObjectId();
    transform_update.heading = object->GetHeading();
    transform_update.position = object->GetPosition();
    transform_update.update_counter = ++counter_map_[object->GetObjectId()];
    
    object->NotifyObservers(&transform_update);
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

    object->GetController()->Notify(&transform);
}

void MovementManager::SendUpdateDataTransformWithParentMessage(const shared_ptr<Object>& object)
{    
    UpdateTransformWithParentMessage transform_update;
    transform_update.object_id = object->GetObjectId();
    transform_update.cell_id = object->GetContainer()->GetObjectId();
    transform_update.heading = object->GetHeading();
    transform_update.position = object->GetPosition();
    transform_update.update_counter = ++counter_map_[object->GetObjectId()];
    
    object->NotifyObservers(&transform_update);
}

void MovementManager::RegisterEvents(swganh::EventDispatcher* event_dispatcher)
{
    event_dispatcher->Subscribe(
        "ObjectReadyEvent",
        [this] (shared_ptr<swganh::EventInterface> incoming_event)
    {
        const auto& object = static_pointer_cast<swganh::ValueEvent<shared_ptr<Object>>>(incoming_event)->Get();
        
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

void MovementManager::SetSpatialProvider(std::shared_ptr<swganh::simulation::SpatialProviderInterface> spatial_provider)
{
	spatial_provider_ = spatial_provider;
}

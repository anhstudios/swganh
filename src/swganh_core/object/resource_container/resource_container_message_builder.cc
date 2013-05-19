// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "resource_container_message_builder.h"

#include <memory>

#include "swganh/logger.h"
#include "swganh/crc.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/object/object_events.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void ResourceContainerMessageBuilder::RegisterEventHandlers()
{
	event_dispatcher->Subscribe("ResourceContainer::CurrentQuantity", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ResourceContainerEvent>(incoming_event);
        BuildCurrentUnitsDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ResourceContainer::GlobalResourceId", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ResourceContainerEvent>(incoming_event);
        BuildGlobalResourceIdDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ResourceContainer::MaxQuantity", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ResourceContainerEvent>(incoming_event);
        BuildMaxQuantityDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ResourceContainer::ResourceType", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ResourceContainerEvent>(incoming_event);
        BuildResourceTypeDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ResourceContainer::ResourceName", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ResourceContainerEvent>(incoming_event);
        BuildVariationNameDelta(value_event->Get());
    });
}

void ResourceContainerMessageBuilder::BuildCurrentUnitsDelta(const std::shared_ptr<ResourceContainer>& resource_container)
{
	if (resource_container->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(resource_container, Object::VIEW_3, 11);
		message.data.write(resource_container->GetCurrentQuantity());
		resource_container->AddDeltasUpdate(&message);
	}
}

void ResourceContainerMessageBuilder::BuildGlobalResourceIdDelta(const std::shared_ptr<ResourceContainer>& resource_container)
{
	if (resource_container->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(resource_container, Object::VIEW_3, 12);
		message.data.write(resource_container->GetGlobalResource());
		resource_container->AddDeltasUpdate(&message);
	}
}

void ResourceContainerMessageBuilder::BuildMaxQuantityDelta(const std::shared_ptr<ResourceContainer>& resource_container)
{
	if (resource_container->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(resource_container, Object::VIEW_6, 2);
		message.data.write(resource_container->GetMaxQuantity());
		resource_container->AddDeltasUpdate(&message);
	}
}

void ResourceContainerMessageBuilder::BuildResourceTypeDelta(const std::shared_ptr<ResourceContainer>& resource_container)
{
	if (resource_container->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(resource_container, Object::VIEW_6, 3);
		message.data.write(resource_container->GetResourceType());
		resource_container->AddDeltasUpdate(&message);
	}
}

void ResourceContainerMessageBuilder::BuildVariationNameDelta(const std::shared_ptr<ResourceContainer>& resource_container)
{
	if (resource_container->HasObservers())
    {
		DeltasMessage message = CreateDeltasMessage(resource_container, Object::VIEW_6, 4);
		message.data.write(resource_container->GetResourceName());
		resource_container->AddDeltasUpdate(&message);
	}
}

// baselines
boost::optional<BaselinesMessage> ResourceContainerMessageBuilder::BuildBaseline3(const std::shared_ptr<ResourceContainer>& resource_container, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(resource_container, lock, Object::VIEW_3, 13);
	message.data.append((*TangibleMessageBuilder::BuildBaseline3(resource_container, lock)).data);
	message.data.write(resource_container->GetCurrentQuantity(lock));
	message.data.write(resource_container->GetGlobalResource(lock));
    return BaselinesMessage(move(message));
}

boost::optional<BaselinesMessage> ResourceContainerMessageBuilder::BuildBaseline6(const std::shared_ptr<ResourceContainer>& resource_container, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(resource_container, lock, Object::VIEW_6, 5);
	message.data.append((*TangibleMessageBuilder::BuildBaseline6(resource_container, lock)).data);
	message.data.write(resource_container->GetMaxQuantity(lock));
	message.data.write(resource_container->GetResourceType(lock));
	message.data.write(resource_container->GetResourceName(lock));
    return BaselinesMessage(move(message));
}
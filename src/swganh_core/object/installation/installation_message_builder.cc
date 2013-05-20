// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "installation_message_builder.h"

#include "installation.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

void InstallationMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("Installation::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<Installation>(controller_event->object), controller_event->observer);
    });
    event_dispatcher->Subscribe("Installation::Active", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildActiveDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::PowerReserve", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildPowerReserveDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::PowerCost", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildPowerCostDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::AvailableResource", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildAvailableResourceDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::DisplayedMaxExtraction", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildDisplayedMaxExtractionDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::MaxExtraction", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildMaxExtractionDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::CurrentExtraction", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildCurrentExtractionDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::CurrentHopperSize", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildCurrentHopperSizeDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::MaxHopperSize", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildMaxHopperSizeDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::Hopper", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildHopperDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::IsUpdating", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
        BuildIsUpdatingDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("Installation::ConditionPercent", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<InstallationEvent>(incoming_event);
		BuildConditionPercentDelta(value_event->Get());
    });
}

void InstallationMessageBuilder::BuildActiveDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message11 = CreateDeltasMessage(installation, Object::VIEW_3, 11);
		message11.data.write<uint8_t>((installation->IsActive()) ? 1 : 0);
        installation->AddDeltasUpdate(&message11);

		DeltasMessage message6 = CreateDeltasMessage(installation, Object::VIEW_7, 6);
		message6.data.write<uint8_t>((installation->IsActive()) ? 1 : 0);
        installation->AddDeltasUpdate(&message6);
    }
}

void InstallationMessageBuilder::BuildPowerReserveDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_3, 12);
		
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildPowerCostDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_3, 13);
		
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildAvailableResourceDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message1 = CreateDeltasMessage(installation, Object::VIEW_7, 1);
        installation->SerializeResourceIds(&message1);
		installation->AddDeltasUpdate(&message1);

		DeltasMessage message2 = CreateDeltasMessage(installation, Object::VIEW_7, 2);
		installation->SerializeResourceIds(&message2);
        installation->AddDeltasUpdate(&message2);

		DeltasMessage message3 = CreateDeltasMessage(installation, Object::VIEW_7, 3);
		installation->SerializeResourceNames(&message3);
		installation->AddDeltasUpdate(&message3);

		DeltasMessage message4 = CreateDeltasMessage(installation, Object::VIEW_7, 4);
		installation->SerializeResourceTypes(&message4);
		installation->AddDeltasUpdate(&message4);
    }
}

void InstallationMessageBuilder::BuildSelectedResourceDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 5);
		message.data.write(installation->GetSelectedResourceId());
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildDisplayedMaxExtractionDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 7);
		message.data.write(installation->GetDisplayedMaxExtractionRate());
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildMaxExtractionDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 8);
		message.data.write(installation->GetMaxExtractionRate());
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildCurrentExtractionDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 9);
		message.data.write(installation->GetCurrentExtractionRate());
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildCurrentHopperSizeDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 10);
		message.data.write(installation->GetCurrentHopperSize());
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildMaxHopperSizeDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 11);
		message.data.write(installation->GetMaxHopperSize());
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildIsUpdatingDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
		DeltasMessage message0 = CreateDeltasMessage(installation, Object::VIEW_7, 0);
		message0.data.write<uint8_t>((installation->IsUpdating()) ? 1 : 0);
        installation->AddDeltasUpdate(&message0);

        DeltasMessage message12 = CreateDeltasMessage(installation, Object::VIEW_7, 12);
		message12.data.write<uint8_t>((installation->IsUpdating()) ? 1 : 0);
        installation->AddDeltasUpdate(&message12);
    }
}

void InstallationMessageBuilder::BuildHopperDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 13);
		installation->SerializeHopperContents(&message);
        installation->AddDeltasUpdate(&message);
    }
}

void InstallationMessageBuilder::BuildConditionPercentDelta(const std::shared_ptr<Installation>& installation)
{
	if (installation->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(installation, Object::VIEW_7, 14);
		message.data.write(installation->GetConditionPercentage()); //TODO: calculate me
        installation->AddDeltasUpdate(&message);
    }
}

boost::optional<BaselinesMessage> InstallationMessageBuilder::BuildBaseline3(const std::shared_ptr<Installation>& installation, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(installation, lock, Object::VIEW_3, 14);
    message.data.append((*TangibleMessageBuilder::BuildBaseline3(installation, lock)).data);
	message.data.write<uint8_t>((installation->IsActive(lock)) ? 1 : 0);
	message.data.write(installation->GetPowerReserve(lock));
	message.data.write(installation->GetPowerCost(lock));
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> InstallationMessageBuilder::BuildBaseline6(const std::shared_ptr<Installation>& installation, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(installation, lock, Object::VIEW_6, 2);
    message.data.append((*TangibleMessageBuilder::BuildBaseline6(installation, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> InstallationMessageBuilder::BuildBaseline7(const std::shared_ptr<Installation>& installation, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(installation, lock, Object::VIEW_7, 11);
	message.data.write<uint8_t>((installation->IsUpdating(lock)) ? 1 : 0);
	installation->SerializeResourceIds(&message, lock);
	installation->SerializeResourceIds(&message, lock);
	installation->SerializeResourceNames(&message, lock);
	installation->SerializeResourceTypes(&message, lock);
	message.data.write(installation->GetSelectedResourceId(lock));
	message.data.write<uint8_t>((installation->IsActive(lock)) ? 1 : 0);
	message.data.write(installation->GetDisplayedMaxExtractionRate(lock));
	message.data.write(installation->GetMaxExtractionRate(lock));
	message.data.write(installation->GetCurrentExtractionRate(lock));
	message.data.write(installation->GetCurrentHopperSize(lock));
	message.data.write(installation->GetMaxHopperSize(lock));
	message.data.write<uint8_t>((installation->IsUpdating(lock)) ? 1 : 0);
	installation->SerializeHopperContents(&message, lock);
	message.data.write(installation->GetConditionPercentage(lock));
    return BaselinesMessage(std::move(message));
}

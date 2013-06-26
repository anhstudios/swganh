// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "manufacture_schematic_message_builder.h"

#include <algorithm>

#include "manufacture_schematic.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace std;
using namespace swganh;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object;


void ManufactureSchematicMessageBuilder::RegisterEventHandlers()
{
    event_dispatcher->Subscribe("ManufactureSchematic::Baselines", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto controller_event = static_pointer_cast<ObserverEvent>(incoming_event);
        SendBaselines(static_pointer_cast<ManufactureSchematic>(controller_event->object), controller_event->observer);
    });

	event_dispatcher->Subscribe("ManufactureSchematic::Quantity", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildSchematicQuantityDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::Property", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildPropertyDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::CreatorName", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildCreatorNameDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::Complexity", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildSchematicComplexityDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::Customization", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildCustomizationDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::DataSize", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildDataSizeDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::CustomizationString", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildCustomizationStringDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::CustomizationModel", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildCustomizationModelDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::PrototypeModel", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildPrototypeModelDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::Active", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildActiveDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::SlotCount", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildSlotCountDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::Slot", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildSlotDelta(value_event->Get());
    });
	event_dispatcher->Subscribe("ManufactureSchematic::Experiment", [this] (shared_ptr<EventInterface> incoming_event)
    {
        auto value_event = static_pointer_cast<ManufactureSchematicEvent>(incoming_event);
        BuildExperimentDelta(value_event->Get());
    });
}

void ManufactureSchematicMessageBuilder::BuildSchematicQuantityDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_3, 4);
		message.data.write<uint32_t>(manufacture_schematic->GetSchematicQuantity());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildPropertyDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_3, 5);
		//manufacture_schematic->GetProperties().Serialize(message);

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildCreatorNameDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_3, 6);
		message.data.write<std::wstring>(manufacture_schematic->GetCreatorName());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildSchematicComplexityDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_3, 7);
		message.data.write<uint32_t>(manufacture_schematic->GetSchematicComplexity());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildDataSizeDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_3, 8);
		message.data.write<float>(manufacture_schematic->GetSchematicDataSize());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildCustomizationStringDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_6, 1);
		message.data.write<std::string>(manufacture_schematic->GetCustomizationString());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildCustomizationModelDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_6, 2);
		message.data.write<std::string>(manufacture_schematic->GetCustomizationModel());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildPrototypeModelDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_6, 3);
		message.data.write<uint32_t>(manufacture_schematic->GetPrototypeCrc());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildActiveDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_6, 4);
		message.data.write<uint32_t>(manufacture_schematic->IsActive() ? 1 : 0);

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildSlotCountDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_6, 5);
		message.data.write<uint8_t>(manufacture_schematic->GetSlotCount());

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildSlotDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	if (manufacture_schematic->HasObservers())
	{
		// SlotNameList
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_7, 0);
		auto slots = manufacture_schematic->GetSlots();
		for (auto& slot : slots)
		{
			
		}
		// TODO THIS IS A BITCH
		manufacture_schematic->AddDeltasUpdate(&message);

	}
}
void ManufactureSchematicMessageBuilder::BuildExperimentDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{
	// TODO THIS IS A BITCH
	if (manufacture_schematic->HasObservers())
	{
		DeltasMessage message = CreateDeltasMessage(manufacture_schematic, Object::VIEW_7, 1);
		manufacture_schematic->SerializeSlots(&message);

		manufacture_schematic->AddDeltasUpdate(&message);
	}
}
void ManufactureSchematicMessageBuilder::BuildCustomizationDelta(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic)
{

}

boost::optional<swganh::messages::BaselinesMessage> ManufactureSchematicMessageBuilder::BuildBaseline3(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic, boost::unique_lock<boost::mutex>& lock)
{
	return CreateBaselinesMessage(manufacture_schematic, lock, 3, 9);
}

boost::optional<swganh::messages::BaselinesMessage> ManufactureSchematicMessageBuilder::BuildBaseline6(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic, boost::unique_lock<boost::mutex>& lock)
{
	return CreateBaselinesMessage(manufacture_schematic, lock, 6, 6);
}

boost::optional<swganh::messages::BaselinesMessage> ManufactureSchematicMessageBuilder::BuildBaseline7(const std::shared_ptr<ManufactureSchematic>& manufacture_schematic, boost::unique_lock<boost::mutex>& lock)
{
	return CreateBaselinesMessage(manufacture_schematic, lock, 7, 21);
}

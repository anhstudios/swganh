// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "manufacture_schematic.h"

#include "swganh_core/messages/deltas_message.h"
#include "swganh/crc.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;
using namespace containers;


ManufactureSchematic::ManufactureSchematic()
	: properties_(NetworkArray<Property>(14))
	, creator_(L"")
	, complexity_(0)
	, schematic_data_size_(0.0f)
	, customization_()
	, customization_model_()
	, prototype_model_()
	, is_active_(false)
	, slot_count_(0)
	, slots_(NetworkSortedVector<Slot>(8))
	, experiments_(NetworkSortedVector<Experiment>(12))
	, customizations_(NetworkSortedVector<Customization>(14))
	, risk_factor_(0.0f)
	, is_ready_(true)
{}

uint32_t ManufactureSchematic::GetType() const
{ 
    return ManufactureSchematic::type; 
}

uint32_t ManufactureSchematic::GetSchematicQuantity() const
{
	return generic_int_;
}

void ManufactureSchematic::SetSchematicQuantity(uint32_t quantity)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		generic_int_ = (quantity > 1000) ? 1000 : quantity;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Quantity", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::IncrementSchematicQuantity(int32_t increment_by)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		uint32_t tmp = generic_int_ + increment_by;
		generic_int_ = (tmp > 1000) ? 1000 : tmp;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Quantity", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

//NetworkArray<Property> ManufactureSchematic::GetProperties() const
//{
//	return properties_;
//}

void ManufactureSchematic::AddProperty(
    std::string property_stf_file,
    std::string property_stf_name,
    float value)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		//properties_.Add(property_stf_file + ":" + property_stf_name, value);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Property", static_pointer_cast<ManufactureSchematic>(shared_from_this())));

}

void ManufactureSchematic::RemoveProperty(
    std::string property_stf_file,
    std::string property_stf_name,
    float value)
{
	{
		/*std::string stf = property_stf_file + ":" + property_stf_name;
		auto iter = properties_.Find(stf);
		if (iter != properties_.end())
			properties_.Remove(iter);
		else*/
			return;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Property", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::UpdateProperty(
    std::string property_stf_file,
    std::string property_stf_name,
    float value)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		/*std::string stf = property_stf_file + ":" + property_stf_name;
		if (properties_.Contains(stf))
			properties_.Update(stf, value);
		else*/
			return;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Property", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

std::wstring ManufactureSchematic::GetCreatorName() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return creator_;
}

void ManufactureSchematic::SetCreatorName(std::wstring creator)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		creator_ = move(creator);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::CreatorName", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

uint32_t ManufactureSchematic::GetSchematicComplexity() const
{
    return complexity_;
}

void ManufactureSchematic::SetSchematicComplexity(uint32_t complexity)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		complexity_ = complexity;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Complexity", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

float ManufactureSchematic::GetSchematicDataSize() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return schematic_data_size_;
}

void ManufactureSchematic::SetSchematicDataSize(float schematic_data_size)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		schematic_data_size_ = schematic_data_size;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::DataSize", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

std::string ManufactureSchematic::GetCustomizationString() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return customization_;
}

void ManufactureSchematic::SetCustomizationString(const std::string& customization_string)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		customization_.append(customization_string);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::CustomizationString", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

std::string ManufactureSchematic::GetCustomizationModel() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return customization_model_;
}

void ManufactureSchematic::SetCustomizationModel(std::string customization_model)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		customization_model_ = customization_model;
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::CustomizationModel", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

std::string ManufactureSchematic::GetPrototypeModel() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return prototype_model_;
}

uint32_t ManufactureSchematic::GetPrototypeCrc() const
{
    return swganh::memcrc(prototype_model_);
}

void ManufactureSchematic::SetPrototypeModel(std::string prototype_model)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		prototype_model_ = move(prototype_model);
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::PrototypeModel", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

bool ManufactureSchematic::IsActive() const
{
    return is_active_;
}

void ManufactureSchematic::Activate()
{
    if (!IsActive())
    {
        ToggleActive();
    }
}

void ManufactureSchematic::Deactive()
{
    if (IsActive())
    {
        ToggleActive();
    }
}

void ManufactureSchematic::ToggleActive()
{
	{
		is_active_ ^= true;
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Active", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

uint8_t ManufactureSchematic::GetSlotCount() const
{
    return slot_count_;
}

void ManufactureSchematic::IncreaseSlotCount()
{
	{
		++slot_count_;
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::SlotCount", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::DecreaseSlotCount()
{
	--slot_count_;

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::SlotCount", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ResetSlotCount(uint8_t slot_count)
{
	slot_count_ = slot_count;

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::SlotCount", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

NetworkSortedVector<ManufactureSchematic::Slot> ManufactureSchematic::GetSlots() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return slots_;
}

void ManufactureSchematic::RemoveSlot(uint16_t index)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto found = slots_.At(index);
		auto iter = slots_.Find(found);
		slots_.Remove(iter);
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Slot", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddSlot(
    std::string slot_stf_file,
    std::string slot_stf_name,
    uint32_t type,
    uint64_t ingredient,
    uint32_t ingredient_quantity,
    uint32_t clean)
{
	int16_t index;
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		Slot slot;    
		slot.index = slots_.Size() + 1;
		index = slot.index;
		slot.slot_stf_file = move(slot_stf_file);
		slot.slot_stf_name = move(slot_stf_name);
		slot.type = type;
		slot.ingredient = ingredient;
		slot.ingredient_quantity = ingredient_quantity;
		slot.clean = clean;

		slots_.Add(slot);
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Slot", static_pointer_cast<ManufactureSchematic>(shared_from_this())));

    return index;
}

void ManufactureSchematic::UpdateSlot(
    uint16_t index,
    std::string slot_stf_file,
    std::string slot_stf_name,
    uint32_t type,
    uint64_t ingredient,
    uint32_t ingredient_quantity,
    uint32_t clean)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			slots_.begin(),
			slots_.end(),
			[index] (const Slot& slot)
		{
			return index == slot.index;
		});

		if (find_iter == slots_.end())
		{
			// Not in the list.
			return;
		}
    
		find_iter->slot_stf_file = move(slot_stf_file);
		find_iter->slot_stf_name = move(slot_stf_name);
		find_iter->type = type;
		find_iter->ingredient = ingredient;
		find_iter->ingredient_quantity = ingredient_quantity;
		find_iter->clean = clean;
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Slot", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ResetSlots(std::vector<ManufactureSchematic::Slot> slots)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		slots_.Clear();
		for(auto& s : slots)
		{
			slots_.Add(s);
		}
		slots_.Reinstall();
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Slot", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ClearAllSlots()
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		slots_.Clear();
	}

	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
	("ManufactureSchematic::Slot", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

NetworkSortedVector<ManufactureSchematic::Experiment> ManufactureSchematic::GetExperiments() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return experiments_;
}

void ManufactureSchematic::RemoveExperiment(uint16_t index)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			experiments_.begin(),
			experiments_.end(),
			[index] (const Experiment& experiment)
		{
			return index == experiment.index;
		});

		if (find_iter == experiments_.end())
		{
			// Not in the list.
			return;
		}

		experiments_.Erase(find_iter);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Experiment", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

/**
 * @return The index the experiment was added into.
 */
uint16_t ManufactureSchematic::AddExperiment(
    std::string experiment_stf_file, 
    std::string experiment_stf_name,
    float value,
    float offset,
    float size,
    float max_value)
{
	int32_t index = 0;
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			experiments_.begin(),
			experiments_.end(),
			[&experiment_stf_name] (const Experiment& experiment)
		{
			return experiment.experiment_stf_name.compare(experiment_stf_name) == 0;
		});

		if (find_iter != experiments_.end())
		{
			// Already in the list.
			return 0;
		}

		Experiment experiment;    
		experiment.index = experiments_.Size()+ 1;
		index = experiment.index;
		experiment.experiment_stf_file = move(experiment_stf_file);
		experiment.experiment_stf_name = move(experiment_stf_name);
		experiment.value = value;
		experiment.offset = offset;
		experiment.size = size;
		experiment.max_value = max_value;

		experiments_.Add(experiment);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Experiment", static_pointer_cast<ManufactureSchematic>(shared_from_this())));

    return index;
}

void ManufactureSchematic::UpdateExperiment(
    uint16_t index,
    std::string experiment_stf_file, 
    std::string experiment_stf_name,
    float value,
    float offset,
    float size,
    float max_value)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			experiments_.begin(),
			experiments_.end(),
			[index] (const Experiment& experiment)
		{
			return index == experiment.index;
		});

		if (find_iter == experiments_.end())
		{
			// Not in the list.
			return;
		}

		find_iter->experiment_stf_file = move(experiment_stf_file);
		find_iter->experiment_stf_name = move(experiment_stf_name);
		find_iter->value = value;
		find_iter->offset = offset;
		find_iter->size = size;
		find_iter->max_value = max_value;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Experiment", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ResetExperiments(std::vector<ManufactureSchematic::Experiment> experiments)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		experiments_.Clear();
		for (auto& e : experiments)
		{
			experiments_.Add(e);
		}
		experiments_.Reinstall();
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Experiment", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ClearAllExperiments()
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		experiments_.Clear();
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Experiment", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

NetworkSortedVector<ManufactureSchematic::Customization> ManufactureSchematic::GetCustomizations() const
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    return customizations_;
}

void ManufactureSchematic::RemoveCustomization(uint16_t index)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			customizations_.begin(),
			customizations_.end(),
			[index] (const Customization& customization)
		{
			return index == customization.index;
		});

		if (find_iter == customizations_.end())
		{
			// Not in the list.
			return;
		}

		customizations_.Erase(find_iter);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Experiment", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddCustomization(
    std::string name,
    uint32_t pallet_selection,
    uint32_t pallet_start_index,
    uint32_t pallet_end_index)
{
	int32_t index;
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			customizations_.begin(),
			customizations_.end(),
			[name] (const Customization& customization)
		{
			return customization.name.compare(name) == 0;
		});

		if (find_iter != customizations_.end())
		{
			// Already in the list.
			return 0;
		}

		Customization customization;    
		customization.index = customizations_.Size() + 1;
		index = customization.index;
		customization.name = move(name);
		customization.pallet_selection = pallet_selection;
		customization.pallet_start_index = pallet_start_index;
		customization.pallet_end_index = pallet_end_index;

		customizations_.Add(customization);
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Customization", static_pointer_cast<ManufactureSchematic>(shared_from_this())));

    return index;
}

void ManufactureSchematic::UpdateCustomization(
    uint16_t index,
    std::string name,
    uint32_t pallet_selection,
    uint32_t pallet_start_index,
    uint32_t pallet_end_index)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		auto find_iter = find_if(
			customizations_.begin(),
			customizations_.end(),
			[index] (const Customization& customization)
		{
			return index == customization.index;
		});

		if (find_iter == customizations_.end())
		{
			// Not in the list.
			return;
		}
    
		find_iter->name = move(name);
		find_iter->pallet_selection = pallet_selection;
		find_iter->pallet_start_index = pallet_start_index;
		find_iter->pallet_end_index = pallet_end_index;
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Customization", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ResetCustomizations(std::vector<Customization> customizations)
{
	{
		boost::lock_guard<boost::mutex> lock(object_mutex_);
		customizations_.Clear();
		for (auto& c : customizations)
		{
			customizations_.Add(c);
		}
		customizations_.Reinstall();
	}
	GetEventDispatcher()->Dispatch(make_shared<ManufactureSchematicEvent>
		("ManufactureSchematic::Customization", static_pointer_cast<ManufactureSchematic>(shared_from_this())));
}

void ManufactureSchematic::ClearAllCustomizations()
{
	boost::lock_guard<boost::mutex> lock(object_mutex_);
    customizations_.Clear();
}

bool ManufactureSchematic::IsReady() const
{
    return is_ready_;
}

void ManufactureSchematic::ToggleReady()
{
    is_ready_ = !is_ready_;
}


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

uint32_t ManufactureSchematic::GetSchematicQuantity() const { return GetSchematicQuantity(AcquireLock()); }
uint32_t ManufactureSchematic::GetSchematicQuantity(boost::unique_lock<boost::mutex>& lock) const
{
	return generic_int_;
}

void ManufactureSchematic::SetSchematicQuantity(uint32_t quantity) { SetSchematicQuantity(quantity, AcquireLock()); }
void ManufactureSchematic::SetSchematicQuantity(uint32_t quantity, boost::unique_lock<boost::mutex>& lock)
{
	generic_int_ = (quantity > 1000) ? 1000 : quantity;
	DISPATCH(ManufactureSchematic, Quantity);
}

void ManufactureSchematic::IncrementSchematicQuantity(int32_t increment_by) { IncrementSchematicQuantity(increment_by, AcquireLock()); }
void ManufactureSchematic::IncrementSchematicQuantity(int32_t increment_by, boost::unique_lock<boost::mutex>& lock)
{
	uint32_t tmp = generic_int_ + increment_by;
	generic_int_ = (tmp > 1000) ? 1000 : tmp;
	DISPATCH(ManufactureSchematic, Quantity);
}

//NetworkArray<Property> ManufactureSchematic::GetProperties() const
//{
//	return properties_;
//}

void ManufactureSchematic::AddProperty(std::string property_stf_file,std::string property_stf_name,float value) { AddProperty(property_stf_file, property_stf_name, value, AcquireLock()); }
void ManufactureSchematic::AddProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock)
{

	//properties_.Add(property_stf_file + ":" + property_stf_name, value);
	DISPATCH(ManufactureSchematic, Property);

}

void ManufactureSchematic::RemoveProperty(std::string property_stf_file,std::string property_stf_name,float value) { RemoveProperty(property_stf_file, property_stf_name, value, AcquireLock()); }
void ManufactureSchematic::RemoveProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock)
{
	/*std::string stf = property_stf_file + ":" + property_stf_name;
	auto iter = properties_.Find(stf);
	if (iter != properties_.end())
		properties_.Remove(iter);
	else*/
		return;
	DISPATCH(ManufactureSchematic, Property);
}

void ManufactureSchematic::UpdateProperty(std::string property_stf_file,std::string property_stf_name,float value) { UpdateProperty(property_stf_file, property_stf_name, value, AcquireLock()); }
void ManufactureSchematic::UpdateProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock)
{
	/*std::string stf = property_stf_file + ":" + property_stf_name;
	if (properties_.Contains(stf))
		properties_.Update(stf, value);
	else*/
		return;
	DISPATCH(ManufactureSchematic, Property);
}

std::wstring ManufactureSchematic::GetCreatorName() const { return GetCreatorName(AcquireLock()); }
std::wstring ManufactureSchematic::GetCreatorName(boost::unique_lock<boost::mutex>& lock) const
{
    return creator_;
}

void ManufactureSchematic::SetCreatorName(std::wstring creator) { SetCreatorName(creator, AcquireLock()); }
void ManufactureSchematic::SetCreatorName(std::wstring creator, boost::unique_lock<boost::mutex>& lock)
{

	creator_ = move(creator);
	DISPATCH(ManufactureSchematic, CreatorName);
}

uint32_t ManufactureSchematic::GetSchematicComplexity() const { return GetSchematicComplexity(AcquireLock()); }
uint32_t ManufactureSchematic::GetSchematicComplexity(boost::unique_lock<boost::mutex>& lock) const
{
    return complexity_;
}

void ManufactureSchematic::SetSchematicComplexity(uint32_t complexity) { SetSchematicComplexity(complexity, AcquireLock()); }
void ManufactureSchematic::SetSchematicComplexity(uint32_t complexity, boost::unique_lock<boost::mutex>& lock)
{
	complexity_ = complexity;
	DISPATCH(ManufactureSchematic, Complexity);
}

float ManufactureSchematic::GetSchematicDataSize() const { return GetSchematicDataSize(AcquireLock()); }
float ManufactureSchematic::GetSchematicDataSize(boost::unique_lock<boost::mutex>& lock) const
{
    return schematic_data_size_;
}

void ManufactureSchematic::SetSchematicDataSize(float schematic_data_size) { SetSchematicDataSize(schematic_data_size, AcquireLock()); }
void ManufactureSchematic::SetSchematicDataSize(float schematic_data_size, boost::unique_lock<boost::mutex>& lock)
{
	schematic_data_size_ = schematic_data_size;
	DISPATCH(ManufactureSchematic, DataSize);
}

std::string ManufactureSchematic::GetCustomizationString() const { return GetCustomizationString(AcquireLock()); }
std::string ManufactureSchematic::GetCustomizationString(boost::unique_lock<boost::mutex>& lock) const
{
    return customization_;
}

void ManufactureSchematic::SetCustomizationString(const std::string& customization_string) { SetCustomizationString(customization_string, AcquireLock()); }
void ManufactureSchematic::SetCustomizationString(const std::string& customization_string, boost::unique_lock<boost::mutex>& lock)
{

	customization_.append(customization_string);
	DISPATCH(ManufactureSchematic, CustomizationString);
}

std::string ManufactureSchematic::GetCustomizationModel() const { return GetCustomizationModel(AcquireLock()); }
std::string ManufactureSchematic::GetCustomizationModel(boost::unique_lock<boost::mutex>& lock) const
{
    return customization_model_;
}

void ManufactureSchematic::SetCustomizationModel(std::string customization_model) { SetCustomizationModel(customization_model, AcquireLock()); }
void ManufactureSchematic::SetCustomizationModel(std::string customization_model, boost::unique_lock<boost::mutex>& lock)
{
	customization_model_ = customization_model;
	DISPATCH(ManufactureSchematic, CustomizationModel);
}

std::string ManufactureSchematic::GetPrototypeModel() const { return GetPrototypeModel(AcquireLock()); }
std::string ManufactureSchematic::GetPrototypeModel(boost::unique_lock<boost::mutex>& lock) const
{
    return prototype_model_;
}

uint32_t ManufactureSchematic::GetPrototypeCrc() const { return GetPrototypeCrc(AcquireLock()); }
uint32_t ManufactureSchematic::GetPrototypeCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return swganh::memcrc(prototype_model_);
}

void ManufactureSchematic::SetPrototypeModel(std::string prototype_model) { SetPrototypeModel(prototype_model, AcquireLock()); }
void ManufactureSchematic::SetPrototypeModel(std::string prototype_model, boost::unique_lock<boost::mutex>& lock)
{
	prototype_model_ = move(prototype_model);
	DISPATCH(ManufactureSchematic, PrototypeModel);
}

bool ManufactureSchematic::IsActive() const { return IsActive(AcquireLock()); }
bool ManufactureSchematic::IsActive(boost::unique_lock<boost::mutex>& lock) const
{
    return is_active_;
}

void ManufactureSchematic::Activate() { Activate(AcquireLock()); }
void ManufactureSchematic::Activate(boost::unique_lock<boost::mutex>& lock)
{
    if (!IsActive(lock))
    {
        ToggleActive(lock);
    }
}

void ManufactureSchematic::Deactive() { Deactive(AcquireLock()); }
void ManufactureSchematic::Deactive(boost::unique_lock<boost::mutex>& lock)
{
    if (IsActive(lock))
    {
        ToggleActive(lock);
    }
}

void ManufactureSchematic::ToggleActive() { ToggleActive(AcquireLock()); }
void ManufactureSchematic::ToggleActive(boost::unique_lock<boost::mutex>& lock)
{
	is_active_ ^= true;
	DISPATCH(ManufactureSchematic, Active);
}

uint8_t ManufactureSchematic::GetSlotCount() const { return GetSlotCount(AcquireLock()); }
uint8_t ManufactureSchematic::GetSlotCount(boost::unique_lock<boost::mutex>& lock) const
{
    return slot_count_;
}

void ManufactureSchematic::IncreaseSlotCount() { IncreaseSlotCount(AcquireLock()); }
void ManufactureSchematic::IncreaseSlotCount(boost::unique_lock<boost::mutex>& lock)
{
	++slot_count_;
	DISPATCH(ManufactureSchematic, SlotCount);
}

void ManufactureSchematic::DecreaseSlotCount() { DecreaseSlotCount(AcquireLock()); }
void ManufactureSchematic::DecreaseSlotCount(boost::unique_lock<boost::mutex>& lock)
{
	--slot_count_;
	DISPATCH(ManufactureSchematic, SlotCount);
}

void ManufactureSchematic::ResetSlotCount(uint8_t slot_count) { ResetSlotCount(slot_count, AcquireLock()); }
void ManufactureSchematic::ResetSlotCount(uint8_t slot_count, boost::unique_lock<boost::mutex>& lock)
{
	slot_count_ = slot_count;
	DISPATCH(ManufactureSchematic, SlotCount);
}

std::vector<ManufactureSchematic::Slot> ManufactureSchematic::GetSlots() const { return GetSlots(AcquireLock()); }
std::vector<ManufactureSchematic::Slot> ManufactureSchematic::GetSlots(boost::unique_lock<boost::mutex>& lock) const
{
    return std::move(slots_.Get());
}

void ManufactureSchematic::RemoveSlot(uint16_t index) { RemoveSlot(index); }
void ManufactureSchematic::RemoveSlot(uint16_t index, boost::unique_lock<boost::mutex>& lock)
{
	auto found = slots_.At(index);
	auto iter = slots_.Find(found);
	slots_.Remove(iter);
	DISPATCH(ManufactureSchematic, Slot);
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddSlot( std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean) 
{ AddSlot(slot_stf_file, slot_stf_name, type, ingredient, ingredient_quantity, clean, AcquireLock()); } 

uint16_t ManufactureSchematic::AddSlot( std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean, boost::unique_lock<boost::mutex>& lock)
{
	int16_t index;
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
	DISPATCH(ManufactureSchematic, Slot);
    return index;
}

void ManufactureSchematic::UpdateSlot( uint16_t index, std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean) 
{ UpdateSlot(index, slot_stf_file, slot_stf_name, type, ingredient, ingredient_quantity, clean, AcquireLock()); } 

void ManufactureSchematic::UpdateSlot( uint16_t index, std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean, boost::unique_lock<boost::mutex>& lock) 
{
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
	DISPATCH(ManufactureSchematic, Slot);
}

void ManufactureSchematic::ResetSlots(std::vector<ManufactureSchematic::Slot> slots) { ResetSlots(slots, AcquireLock()); }
void ManufactureSchematic::ResetSlots(std::vector<ManufactureSchematic::Slot> slots, boost::unique_lock<boost::mutex>& lock)
{
	slots_.Clear();
	for(auto& s : slots)
	{
		slots_.Add(s);
	}
	slots_.Reinstall();
	DISPATCH(ManufactureSchematic, Slot);
}

void ManufactureSchematic::ClearAllSlots() { ClearAllSlots(AcquireLock()); }
void ManufactureSchematic::ClearAllSlots(boost::unique_lock<boost::mutex>& lock)
{
	slots_.Clear();
	DISPATCH(ManufactureSchematic, Slot);
}

std::vector<ManufactureSchematic::Experiment> ManufactureSchematic::GetExperiments() const { return GetExperiments(AcquireLock()); }
std::vector<ManufactureSchematic::Experiment> ManufactureSchematic::GetExperiments(boost::unique_lock<boost::mutex>& lock) const
{
    return std::move(experiments_.Get());
}

void ManufactureSchematic::RemoveExperiment(uint16_t index) { RemoveExperiment(index, AcquireLock()); }
void ManufactureSchematic::RemoveExperiment(uint16_t index, boost::unique_lock<boost::mutex>& lock)
{
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
	DISPATCH(ManufactureSchematic, Experiment);
}

/**
 * @return The index the experiment was added into.
 */
uint16_t ManufactureSchematic::AddExperiment(std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value) 
{ AddExperiment(experiment_stf_file, experiment_stf_name, value, offset, size, max_value, AcquireLock()); }

uint16_t ManufactureSchematic::AddExperiment(std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value, boost::unique_lock<boost::mutex>& lock)
{
	int32_t index = 0;
	{
		auto lock = AcquireLock();
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
	DISPATCH(ManufactureSchematic, Experiment);

    return index;
}

void ManufactureSchematic::UpdateExperiment( uint16_t index, std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value) 
{ UpdateExperiment(index, experiment_stf_file, experiment_stf_name, value, offset, size, max_value, AcquireLock()); }
void ManufactureSchematic::UpdateExperiment( uint16_t index, std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value, boost::unique_lock<boost::mutex>& lock)
{
	{
		auto lock = AcquireLock();
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
	DISPATCH(ManufactureSchematic, Experiment);
}

void ManufactureSchematic::ResetExperiments(std::vector<ManufactureSchematic::Experiment> experiments)
{ ResetExperiments(experiments, AcquireLock()); }

void ManufactureSchematic::ResetExperiments(std::vector<ManufactureSchematic::Experiment> experiments, boost::unique_lock<boost::mutex>& lock)
{
	experiments_.Clear();
	for (auto& e : experiments)
	{
		experiments_.Add(e);
	}
	experiments_.Reinstall();
	DISPATCH(ManufactureSchematic, Experiment);
}

void ManufactureSchematic::ClearAllExperiments() { ClearAllExperiments(AcquireLock()); }
void ManufactureSchematic::ClearAllExperiments(boost::unique_lock<boost::mutex>& lock)
{
	experiments_.Clear();
	DISPATCH(ManufactureSchematic, Experiment);
}

std::vector<ManufactureSchematic::Customization> ManufactureSchematic::GetCustomizations() const { return GetCustomizations(AcquireLock()); }
std::vector<ManufactureSchematic::Customization> ManufactureSchematic::GetCustomizations(boost::unique_lock<boost::mutex>& lock) const
{
    return std::move(customizations_.Get());
}

void ManufactureSchematic::RemoveCustomization(uint16_t index) { RemoveCustomization(index, AcquireLock()); }
void ManufactureSchematic::RemoveCustomization(uint16_t index, boost::unique_lock<boost::mutex>& lock)
{
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
	DISPATCH(ManufactureSchematic, Experiment);
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddCustomization( std::string name, 
	uint32_t pallet_selection, uint32_t pallet_start_index, uint32_t pallet_end_index) 
{ AddCustomization(name, pallet_selection, pallet_start_index, pallet_end_index, AcquireLock()); }

uint16_t ManufactureSchematic::AddCustomization( std::string name, 
	uint32_t pallet_selection, uint32_t pallet_start_index, uint32_t pallet_end_index, boost::unique_lock<boost::mutex>& lock)
{
	int32_t index;
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
	DISPATCH(ManufactureSchematic, Customization);
    return index;
}

void ManufactureSchematic::UpdateCustomization( uint16_t index, std::string name, 
	uint32_t pallet_selection, uint32_t pallet_start_index, uint32_t pallet_end_index) 
{ UpdateCustomization(index, name, pallet_selection, pallet_start_index, pallet_end_index, AcquireLock()); }

void ManufactureSchematic::UpdateCustomization( uint16_t index, std::string name, 
	uint32_t pallet_selection, uint32_t pallet_start_index, uint32_t pallet_end_index, boost::unique_lock<boost::mutex>& lock) 
{
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
	DISPATCH(ManufactureSchematic, Customization);
}

void ManufactureSchematic::ResetCustomizations(std::vector<Customization> customizations) { ResetCustomizations(customizations, AcquireLock()); }
void ManufactureSchematic::ResetCustomizations(std::vector<Customization> customizations, boost::unique_lock<boost::mutex>& lock)
{
	{
		auto lock = AcquireLock();
		customizations_.Clear();
		for (auto& c : customizations)
		{
			customizations_.Add(c);
		}
		customizations_.Reinstall();
	}
	DISPATCH(ManufactureSchematic, Customization);
}

void ManufactureSchematic::ClearAllCustomizations() { ClearAllCustomizations(AcquireLock()); }
void ManufactureSchematic::ClearAllCustomizations(boost::unique_lock<boost::mutex>& lock)
{
	auto lock = AcquireLock();
    customizations_.Clear();
}

bool ManufactureSchematic::IsReady() const { return IsReady(AcquireLock()); }
bool ManufactureSchematic::IsReady(boost::unique_lock<boost::mutex>& lock) const
{
    return is_ready_;
}

void ManufactureSchematic::ToggleReady() { ToggleReady(AcquireLock()); }
void ManufactureSchematic::ToggleReady(boost::unique_lock<boost::mutex>& lock)
{
    is_ready_ = !is_ready_;
}

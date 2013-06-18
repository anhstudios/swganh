// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "manufacture_schematic.h"

#include "swganh_core/messages/deltas_message.h"
#include "swganh/crc.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

ManufactureSchematic::ManufactureSchematic()
	: creator_(L"")
	, complexity_(0)
	, schematic_data_size_(0.0f)
	, customization_()
	, customization_model_()
	, prototype_model_()
	, is_active_(false)
	, slot_count_(0)
	, risk_factor_(0.0f)
	, is_ready_(true)
{}

uint32_t ManufactureSchematic::GetType() const
{ 
    return ManufactureSchematic::type; 
}

uint32_t ManufactureSchematic::GetSchematicQuantity() const {
    auto lock = AcquireLock();
    return GetSchematicQuantity(lock);
}

uint32_t ManufactureSchematic::GetSchematicQuantity(boost::unique_lock<boost::mutex>& lock) const
{
	return generic_int_;
}

void ManufactureSchematic::SetSchematicQuantity(uint32_t quantity) {
    auto lock = AcquireLock();
    SetSchematicQuantity(quantity, lock);
}

void ManufactureSchematic::SetSchematicQuantity(uint32_t quantity, boost::unique_lock<boost::mutex>& lock)
{
	generic_int_ = (quantity > 1000) ? 1000 : quantity;
	DISPATCH(ManufactureSchematic, Quantity);
}

void ManufactureSchematic::IncrementSchematicQuantity(int32_t increment_by) {
    auto lock = AcquireLock();
    IncrementSchematicQuantity(increment_by, lock);
}

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

void ManufactureSchematic::AddProperty(std::string property_stf_file,std::string property_stf_name,float value) {
    auto lock = AcquireLock();
    AddProperty(property_stf_file, property_stf_name, value, lock);
}

void ManufactureSchematic::AddProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock)
{

	//properties_.Add(property_stf_file + ":" + property_stf_name, value);
	DISPATCH(ManufactureSchematic, Property);

}

void ManufactureSchematic::RemoveProperty(std::string property_stf_file,std::string property_stf_name,float value) {
    auto lock = AcquireLock();
    RemoveProperty(property_stf_file, property_stf_name, value, lock);
}

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

void ManufactureSchematic::UpdateProperty(std::string property_stf_file,std::string property_stf_name,float value) {
    auto lock = AcquireLock();
    UpdateProperty(property_stf_file, property_stf_name, value, lock);
}

void ManufactureSchematic::UpdateProperty(std::string property_stf_file,std::string property_stf_name,float value, boost::unique_lock<boost::mutex>& lock)
{
	/*std::string stf = property_stf_file + ":" + property_stf_name;
	if (properties_.Contains(stf))
		properties_.Update(stf, value);
	else*/
		return;
	DISPATCH(ManufactureSchematic, Property);
}

std::wstring ManufactureSchematic::GetCreatorName() const {
    auto lock = AcquireLock();
    return GetCreatorName(lock);
}

std::wstring ManufactureSchematic::GetCreatorName(boost::unique_lock<boost::mutex>& lock) const
{
    return creator_;
}

void ManufactureSchematic::SetCreatorName(std::wstring creator) {
    auto lock = AcquireLock();
    SetCreatorName(creator, lock);
}

void ManufactureSchematic::SetCreatorName(std::wstring creator, boost::unique_lock<boost::mutex>& lock)
{

	creator_ = move(creator);
	DISPATCH(ManufactureSchematic, CreatorName);
}

uint32_t ManufactureSchematic::GetSchematicComplexity() const {
    auto lock = AcquireLock();
    return GetSchematicComplexity(lock);
}

uint32_t ManufactureSchematic::GetSchematicComplexity(boost::unique_lock<boost::mutex>& lock) const
{
    return complexity_;
}

void ManufactureSchematic::SetSchematicComplexity(uint32_t complexity) {
    auto lock = AcquireLock();
    SetSchematicComplexity(complexity, lock);
}

void ManufactureSchematic::SetSchematicComplexity(uint32_t complexity, boost::unique_lock<boost::mutex>& lock)
{
	complexity_ = complexity;
	DISPATCH(ManufactureSchematic, Complexity);
}

float ManufactureSchematic::GetSchematicDataSize() const {
    auto lock = AcquireLock();
    return GetSchematicDataSize(lock);
}

float ManufactureSchematic::GetSchematicDataSize(boost::unique_lock<boost::mutex>& lock) const
{
    return schematic_data_size_;
}

void ManufactureSchematic::SetSchematicDataSize(float schematic_data_size) {
    auto lock = AcquireLock();
    SetSchematicDataSize(schematic_data_size, lock);
}

void ManufactureSchematic::SetSchematicDataSize(float schematic_data_size, boost::unique_lock<boost::mutex>& lock)
{
	schematic_data_size_ = schematic_data_size;
	DISPATCH(ManufactureSchematic, DataSize);
}

std::string ManufactureSchematic::GetCustomizationString() const {
    auto lock = AcquireLock();
    return GetCustomizationString(lock);
}

std::string ManufactureSchematic::GetCustomizationString(boost::unique_lock<boost::mutex>& lock) const
{
    return customization_;
}

void ManufactureSchematic::SetCustomizationString(const std::string& customization_string) {
    auto lock = AcquireLock();
    SetCustomizationString(customization_string, lock);
}

void ManufactureSchematic::SetCustomizationString(const std::string& customization_string, boost::unique_lock<boost::mutex>& lock)
{

	customization_.append(customization_string);
	DISPATCH(ManufactureSchematic, CustomizationString);
}

std::string ManufactureSchematic::GetCustomizationModel() const {
    auto lock = AcquireLock();
    return GetCustomizationModel(lock);
}

std::string ManufactureSchematic::GetCustomizationModel(boost::unique_lock<boost::mutex>& lock) const
{
    return customization_model_;
}

void ManufactureSchematic::SetCustomizationModel(std::string customization_model) {
    auto lock = AcquireLock();
    SetCustomizationModel(customization_model, lock);
}

void ManufactureSchematic::SetCustomizationModel(std::string customization_model, boost::unique_lock<boost::mutex>& lock)
{
	customization_model_ = customization_model;
	DISPATCH(ManufactureSchematic, CustomizationModel);
}

std::string ManufactureSchematic::GetPrototypeModel() const {
    auto lock = AcquireLock();
    return GetPrototypeModel(lock);
}

std::string ManufactureSchematic::GetPrototypeModel(boost::unique_lock<boost::mutex>& lock) const
{
    return prototype_model_;
}

uint32_t ManufactureSchematic::GetPrototypeCrc() const {
    auto lock = AcquireLock();
    return GetPrototypeCrc(lock);
}

uint32_t ManufactureSchematic::GetPrototypeCrc(boost::unique_lock<boost::mutex>& lock) const
{
    return swganh::memcrc(prototype_model_);
}

void ManufactureSchematic::SetPrototypeModel(std::string prototype_model) {
    auto lock = AcquireLock();
    SetPrototypeModel(prototype_model, lock);
}

void ManufactureSchematic::SetPrototypeModel(std::string prototype_model, boost::unique_lock<boost::mutex>& lock)
{
	prototype_model_ = move(prototype_model);
	DISPATCH(ManufactureSchematic, PrototypeModel);
}

bool ManufactureSchematic::IsActive() const {
    auto lock = AcquireLock();
    return IsActive(lock);
}

bool ManufactureSchematic::IsActive(boost::unique_lock<boost::mutex>& lock) const
{
    return is_active_;
}

void ManufactureSchematic::Activate() {
    auto lock = AcquireLock();
    Activate(lock);
}

void ManufactureSchematic::Activate(boost::unique_lock<boost::mutex>& lock)
{
    if (!IsActive(lock))
    {
        ToggleActive(lock);
    }
}

void ManufactureSchematic::Deactive() {
    auto lock = AcquireLock();
    Deactive(lock);
}

void ManufactureSchematic::Deactive(boost::unique_lock<boost::mutex>& lock)
{
    if (IsActive(lock))
    {
        ToggleActive(lock);
    }
}

void ManufactureSchematic::ToggleActive() {
    auto lock = AcquireLock();
    ToggleActive(lock);
}

void ManufactureSchematic::ToggleActive(boost::unique_lock<boost::mutex>& lock)
{
	is_active_ ^= true;
	DISPATCH(ManufactureSchematic, Active);
}

uint8_t ManufactureSchematic::GetSlotCount() const {
    auto lock = AcquireLock();
    return GetSlotCount(lock);
}

uint8_t ManufactureSchematic::GetSlotCount(boost::unique_lock<boost::mutex>& lock) const
{
    return slot_count_;
}

void ManufactureSchematic::IncreaseSlotCount() {
    auto lock = AcquireLock();
    IncreaseSlotCount(lock);
}

void ManufactureSchematic::IncreaseSlotCount(boost::unique_lock<boost::mutex>& lock)
{
	++slot_count_;
	DISPATCH(ManufactureSchematic, SlotCount);
}

void ManufactureSchematic::DecreaseSlotCount() {
    auto lock = AcquireLock();
    DecreaseSlotCount(lock);
}

void ManufactureSchematic::DecreaseSlotCount(boost::unique_lock<boost::mutex>& lock)
{
	--slot_count_;
	DISPATCH(ManufactureSchematic, SlotCount);
}

void ManufactureSchematic::ResetSlotCount(uint8_t slot_count) {
    auto lock = AcquireLock();
    ResetSlotCount(slot_count, lock);
}

void ManufactureSchematic::ResetSlotCount(uint8_t slot_count, boost::unique_lock<boost::mutex>& lock)
{
	slot_count_ = slot_count;
	DISPATCH(ManufactureSchematic, SlotCount);
}

std::vector<Slot> ManufactureSchematic::GetSlots() {
    auto lock = AcquireLock();
    return GetSlots(lock);
}

std::vector<Slot> ManufactureSchematic::GetSlots(boost::unique_lock<boost::mutex>& lock)
{
    return slots_.raw();
}

void ManufactureSchematic::RemoveSlot(uint16_t index) {
    auto lock = AcquireLock();
    RemoveSlot(index, lock);
}

void ManufactureSchematic::RemoveSlot(uint16_t index, boost::unique_lock<boost::mutex>& lock)
{
	slots_.remove(index);
	DISPATCH(ManufactureSchematic, Slot);
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddSlot( std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean) 
{
    auto lock = AcquireLock();
    return AddSlot(slot_stf_file, slot_stf_name, type, ingredient, ingredient_quantity, clean, lock);
}

uint16_t ManufactureSchematic::AddSlot( std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean, boost::unique_lock<boost::mutex>& lock)
{
	int16_t index;
	Slot slot;    
	slot.index = slots_.size() + 1;
	index = slot.index;
	slot.slot_stf_file = move(slot_stf_file);
	slot.slot_stf_name = move(slot_stf_name);
	slot.type = type;
	slot.ingredient = ingredient;
	slot.ingredient_quantity = ingredient_quantity;
	slot.clean = clean;

	slots_.add(slot);
	DISPATCH(ManufactureSchematic, Slot);
    return index;
}

void ManufactureSchematic::UpdateSlot( uint16_t index, std::string slot_stf_file, std::string slot_stf_name,
    uint32_t type, uint64_t ingredient, uint32_t ingredient_quantity, uint32_t clean) 
{
    auto lock = AcquireLock();
    UpdateSlot(index, slot_stf_file, slot_stf_name, type, ingredient, ingredient_quantity, clean, lock);
}

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

void ManufactureSchematic::ResetSlots(std::vector<Slot> slots) {
    auto lock = AcquireLock();
    ResetSlots(slots, lock);
}

void ManufactureSchematic::ResetSlots(std::vector<Slot> slots, boost::unique_lock<boost::mutex>& lock)
{
	slots_.reset(slots);
	DISPATCH(ManufactureSchematic, Slot);
}

void ManufactureSchematic::ClearAllSlots() {
    auto lock = AcquireLock();
    ClearAllSlots(lock);
}

void ManufactureSchematic::ClearAllSlots(boost::unique_lock<boost::mutex>& lock)
{
	slots_.clear();
	DISPATCH(ManufactureSchematic, Slot);
}

std::vector<Experiment> ManufactureSchematic::GetExperiments() {
    auto lock = AcquireLock();
    return GetExperiments(lock);
}

std::vector<Experiment> ManufactureSchematic::GetExperiments(boost::unique_lock<boost::mutex>& lock)
{
    return experiments_.raw();
}

void ManufactureSchematic::RemoveExperiment(uint16_t index) {
    auto lock = AcquireLock();
    RemoveExperiment(index, lock);
}

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

	experiments_.remove(find_iter);
	DISPATCH(ManufactureSchematic, Experiment);
}

/**
 * @return The index the experiment was added into.
 */
uint16_t ManufactureSchematic::AddExperiment(std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value) 
{
    auto lock = AcquireLock();
    return AddExperiment(experiment_stf_file, experiment_stf_name, value, offset, size, max_value, lock);
}

uint16_t ManufactureSchematic::AddExperiment(std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value, boost::unique_lock<boost::mutex>& lock)
{
	int32_t index = 0;
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
	experiment.index = experiments_.size()+ 1;
	index = experiment.index;
	experiment.experiment_stf_file = move(experiment_stf_file);
	experiment.experiment_stf_name = move(experiment_stf_name);
	experiment.value = value;
	experiment.offset = offset;
	experiment.size = size;
	experiment.max_value = max_value;

	experiments_.add(experiment);
	DISPATCH(ManufactureSchematic, Experiment);

    return index;
}

void ManufactureSchematic::UpdateExperiment( uint16_t index, std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value) 
{
    auto lock = AcquireLock();
    UpdateExperiment(index, experiment_stf_file, experiment_stf_name, value, offset, size, max_value, lock);
}

void ManufactureSchematic::UpdateExperiment( uint16_t index, std::string experiment_stf_file,  std::string experiment_stf_name,
    float value, float offset, float size, float max_value, boost::unique_lock<boost::mutex>& lock)
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

	find_iter->experiment_stf_file = move(experiment_stf_file);
	find_iter->experiment_stf_name = move(experiment_stf_name);
	find_iter->value = value;
	find_iter->offset = offset;
	find_iter->size = size;
	find_iter->max_value = max_value;
	DISPATCH(ManufactureSchematic, Experiment);
}

void ManufactureSchematic::ResetExperiments(std::vector<Experiment> experiments)
{
    auto lock = AcquireLock();
    ResetExperiments(experiments, lock);
}

void ManufactureSchematic::ResetExperiments(std::vector<Experiment> experiments, boost::unique_lock<boost::mutex>& lock)
{
	experiments_.reset(experiments);
	DISPATCH(ManufactureSchematic, Experiment);
}

void ManufactureSchematic::ClearAllExperiments() {
    auto lock = AcquireLock();
    ClearAllExperiments(lock);
}

void ManufactureSchematic::ClearAllExperiments(boost::unique_lock<boost::mutex>& lock)
{
	experiments_.clear();
	DISPATCH(ManufactureSchematic, Experiment);
}

std::vector<Customization> ManufactureSchematic::GetCustomizations() {
    auto lock = AcquireLock();
    return GetCustomizations(lock);
}

std::vector<Customization> ManufactureSchematic::GetCustomizations(boost::unique_lock<boost::mutex>& lock) 
{
    return customizations_.raw();
}

void ManufactureSchematic::RemoveCustomization(uint16_t index) {
    auto lock = AcquireLock();
    RemoveCustomization(index, lock);
}

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

	customizations_.remove(find_iter);
	DISPATCH(ManufactureSchematic, Experiment);
}

/**
 * @return The index the slot was added into.
 */
uint16_t ManufactureSchematic::AddCustomization( std::string name, 
	uint32_t pallet_selection, uint32_t pallet_start_index, uint32_t pallet_end_index) 
{
    auto lock = AcquireLock();
    return AddCustomization(name, pallet_selection, pallet_start_index, pallet_end_index, lock);
}

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
	customization.index = customizations_.size() + 1;
	index = customization.index;
	customization.name = move(name);
	customization.pallet_selection = pallet_selection;
	customization.pallet_start_index = pallet_start_index;
	customization.pallet_end_index = pallet_end_index;

	customizations_.add(customization);
	DISPATCH(ManufactureSchematic, Customization);
    return index;
}

void ManufactureSchematic::UpdateCustomization( uint16_t index, std::string name, 
	uint32_t pallet_selection, uint32_t pallet_start_index, uint32_t pallet_end_index) 
{
    auto lock = AcquireLock();
    UpdateCustomization(index, name, pallet_selection, pallet_start_index, pallet_end_index, lock);
}

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

void ManufactureSchematic::ResetCustomizations(std::vector<Customization> customizations) {
    auto lock = AcquireLock();
    ResetCustomizations(customizations, lock);
}

void ManufactureSchematic::ResetCustomizations(std::vector<Customization> customizations, boost::unique_lock<boost::mutex>& lock)
{
	customizations_.reset(customizations);
	DISPATCH(ManufactureSchematic, Customization);
}

void ManufactureSchematic::ClearAllCustomizations() {
    auto lock = AcquireLock();
    ClearAllCustomizations(lock);
}

void ManufactureSchematic::ClearAllCustomizations(boost::unique_lock<boost::mutex>& lock)
{
    customizations_.clear();
}

bool ManufactureSchematic::IsReady() const {
    auto lock = AcquireLock();
    return IsReady(lock);
}

bool ManufactureSchematic::IsReady(boost::unique_lock<boost::mutex>& lock) const
{
    return is_ready_;
}

void ManufactureSchematic::ToggleReady() {
    auto lock = AcquireLock();
    ToggleReady(lock);
}

void ManufactureSchematic::ToggleReady(boost::unique_lock<boost::mutex>& lock)
{
    is_ready_ = !is_ready_;
}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <atomic>
#include <set>
#include <string>
#include <vector>
#include <list>

#include "swganh_core/object/object.h"

#include "swganh_core/messages/containers/network_list.h"
#include "swganh_core/messages/containers/network_sorted_vector.h"

namespace swganh {
namespace object {


enum StaticType {
    MOVEABLE = 0,
    STATIC
};

struct Defender
{
    Defender()
        : object_id(0)
    {}

    Defender(uint64_t object_id_)
        : object_id(object_id_)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(object_id);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(object_id);
    }

    bool operator==(const Defender& other)
    {
        return other.object_id == object_id;
    }

    uint64_t object_id;
};

struct ComponentCustomization
{
    ComponentCustomization(uint32_t component_customization_crc_)
        : component_customization_crc(component_customization_crc_)
    {}

    ~ComponentCustomization(void)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(component_customization_crc);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(component_customization_crc);
    }

    bool operator==(const ComponentCustomization& other)
    {
        return component_customization_crc == other.component_customization_crc;
    }

    uint32_t component_customization_crc;
};

class TangibleFactory;
class TangibleMessageBuilder;

enum OptionsMask
{
	OPTION_PLAYER = 0x01,
	OPTION_VENDOR = 0x02,
	OPTION_TRAINER = 0x08,
	OPTION_NO_HAM = 0x100,
	OPTION_QUEST_NPC = 0x800,
	OPTION_TIE_XWING = 0x8000
};

/**
 * 
 */
class Tangible : public swganh::object::Object
{
public:
    typedef TangibleFactory FactoryType;
    typedef TangibleMessageBuilder MessageBuilderType;

    // TANO
    virtual uint32_t GetType() const { return Tangible::type; }
    const static uint32_t type = 0x54414e4f;

    Tangible();
    Tangible(const std::string& customization, std::vector<uint32_t> component_customization, uint32_t bitmask_options,
        uint32_t counter, uint32_t damage_amount, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders);

    // Customization
    void AddCustomization(const std::string& customization);
	void AddCustomization(const std::string& customization, boost::unique_lock<boost::mutex>& lock);

    void SetCustomization(const std::string& customization);
	void SetCustomization(const std::string& customization, boost::unique_lock<boost::mutex>& lock);

	void SetCustomizationFromInts(std::vector<int> customization_ints);
	void SetCustomizationFromInts(std::vector<int> customization_ints, boost::unique_lock<boost::mutex>& lock);

    std::string GetCustomization();
	std::string GetCustomization(boost::unique_lock<boost::mutex>& lock);

    // Component Customization
    std::list<ComponentCustomization> GetComponentCustomization();
	std::list<ComponentCustomization> GetComponentCustomization(boost::unique_lock<boost::mutex>& lock);

    void AddComponentCustomization(uint32_t customization_crc);
	void AddComponentCustomization(uint32_t customization_crc, boost::unique_lock<boost::mutex>& lock);

    void RemoveComponentCustomization(uint32_t customization_crc);
	void RemoveComponentCustomization(uint32_t customization_crc, boost::unique_lock<boost::mutex>& lock);

	void SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message);
	void SerializeComponentCustomization(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    void ClearComponentCustomization();
	void ClearComponentCustomization(boost::unique_lock<boost::mutex>& lock);
    
    // Options Mask
    uint32_t GetOptionsMask();
	uint32_t GetOptionsMask(boost::unique_lock<boost::mutex>& lock);

    void ToggleOption(uint32_t option);
	void ToggleOption(uint32_t option, boost::unique_lock<boost::mutex>& lock);

    void SetOptionsMask(uint32_t options);
	void SetOptionsMask(uint32_t options, boost::unique_lock<boost::mutex>& lock);

    // Counter
	// Used for both item counters and incap_timer
    uint32_t GetCounter();
	uint32_t GetCounter(boost::unique_lock<boost::mutex>& lock);

	// Used for both item counters and incap_timer
    void SetCounter(uint32_t counter);
	void SetCounter(uint32_t counter, boost::unique_lock<boost::mutex>& lock);

    // Condition
    uint32_t GetCondition();
	uint32_t GetCondition(boost::unique_lock<boost::mutex>& lock);

    void SetConditionDamage(uint32_t damage_amount);
	void SetConditionDamage(uint32_t damage_amount, boost::unique_lock<boost::mutex>& lock);


    // Max Condition
    uint32_t GetMaxCondition();
	uint32_t GetMaxCondition(boost::unique_lock<boost::mutex>& lock);

    void SetMaxCondition(uint32_t max_condition);
	void SetMaxCondition(uint32_t max_condition, boost::unique_lock<boost::mutex>& lock);

    // Static
    bool IsStatic();
	bool IsStatic(boost::unique_lock<boost::mutex>& lock);

    void SetStatic(bool is_static);
	void SetStatic(bool is_static, boost::unique_lock<boost::mutex>& lock);

    // Defenders
    void AddDefender(uint64_t defender);
	void AddDefender(uint64_t defender, boost::unique_lock<boost::mutex>& lock);

    void RemoveDefender(uint64_t defender);
	void RemoveDefender(uint64_t defender, boost::unique_lock<boost::mutex>& lock);

    void ResetDefenders(std::vector<uint64_t> defenders);
	void ResetDefenders(std::vector<uint64_t> defenders, boost::unique_lock<boost::mutex>& lock);

    bool IsDefending(uint64_t defender);
	bool IsDefending(uint64_t defender, boost::unique_lock<boost::mutex>& lock);

    std::vector<Defender> GetDefenders();
	std::vector<Defender> GetDefenders(boost::unique_lock<boost::mutex>& lock);

	void SerializeDefenders(swganh::messages::BaseSwgMessage* message);
	void SerializeDefenders(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock);

    void ClearDefenders();
	void ClearDefenders(boost::unique_lock<boost::mutex>& lock);

    void ActivateAutoAttack();
	void ActivateAutoAttack(boost::unique_lock<boost::mutex>& lock);

    void ClearAutoAttack();
	void ClearAutoAttack(boost::unique_lock<boost::mutex>& lock);

    bool IsAutoAttacking();
	bool IsAutoAttacking(boost::unique_lock<boost::mutex>& lock);

    virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);

private:
    typedef swganh::ValueEvent<std::shared_ptr<Tangible>> TangibleEvent;

    std::string customization_;                                                                                      // update 3 variable 4
    swganh::messages::containers::NetworkList<ComponentCustomization> component_customization_list_;                 // update 3 variable 5
    uint32_t options_bitmask_;                                                                          // update 3 variable 6
	// Uses for uses counter on items and incap_timer for creature
    uint32_t counter_;                                                                                  // update 3 variable 7
    uint32_t condition_damage_;                                                                         // update 3 variable 8
    uint32_t max_condition_;                                                                            // update 3 variable 9
    bool is_static_;                                                                                    // update 3 variable 10
    swganh::messages::containers::NetworkSortedVector<Defender> defender_list_;                                      // update 6 variable 1
    
	// Flag to help out in combat situations
    bool auto_attack_;
};
    
}}  // namespace swganh::object

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
#include "swganh_core/messages/containers/network_sorted_vector.h"
#include "swganh_core/messages/containers/network_list.h"

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
        uint32_t incap_timer, uint32_t damage_amount, uint32_t max_condition, bool is_static, std::vector<uint64_t> defenders);

    // Customization
    void AddCustomization(const std::string& customization);
    void SetCustomization(const std::string& customization);
    std::string GetCustomization();

    // Component Customization
    swganh::messages::containers::NetworkList<ComponentCustomization> GetComponentCustomization(void);
    void AddComponentCustomization(uint32_t customization_crc);
    void RemoveComponentCustomization(uint32_t customization_crc);
    void ClearComponentCustomization();
    
    // Options Mask
    uint32_t GetOptionsMask();
    void ToggleOption(uint32_t option);
    void SetOptionsMask(uint32_t options);

    // Incap Timer
    uint32_t GetIncapTimer();
    void SetIncapTimer(uint32_t incap_timer);

    // Condition
    uint32_t GetCondition();
    void SetConditionDamage(uint32_t damage_amount);

    // Max Condition
    uint32_t GetMaxCondition();
    void SetMaxCondition(uint32_t max_condition);

    // Static
    bool IsStatic();
    void SetStatic(bool is_static);

    // Defenders
    void AddDefender(uint64_t defender);
    void RemoveDefender(uint64_t defender);
    void ResetDefenders(std::vector<uint64_t> defenders);
    bool IsDefending(uint64_t defender);
    swganh::messages::containers::NetworkSortedVector<Defender> GetDefenders();
    void ClearDefenders();

    void ActivateAutoAttack();
    void ClearAutoAttack();
    bool IsAutoAttacking();

    virtual void CreateBaselines(std::shared_ptr<swganh::observer::ObserverInterface> observer);
private:
    typedef swganh::ValueEvent<std::shared_ptr<Tangible>> TangibleEvent;

    std::string customization_;                                                                                      // update 3 variable 4
    swganh::messages::containers::NetworkList<ComponentCustomization> component_customization_list_;                 // update 3 variable 5
    std::atomic<uint32_t> options_bitmask_;                                                                          // update 3 variable 6
    std::atomic<uint32_t> incap_timer_;                                                                              // update 3 variable 7
    std::atomic<uint32_t> condition_damage_;                                                                         // update 3 variable 8
    std::atomic<uint32_t> max_condition_;                                                                            // update 3 variable 9
    std::atomic<bool> is_static_;                                                                                    // update 3 variable 10
    swganh::messages::containers::NetworkSortedVector<Defender> defender_list_;                                      // update 6 variable 1
    // Flag to help out in combat situations
    std::atomic<bool> auto_attack_;
};
    
}}  // namespace swganh::object

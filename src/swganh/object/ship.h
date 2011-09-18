#ifndef SWGANH_OBJECT_SHIP_H_
#define SWGANH_OBJECT_SHIP_H_

#include <string>

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {

template<class ValueType> 
struct ShipList {
    uint8_t delta;
    uint32_t component_index;
    ValueType value;
};
    
class Ship : public BaseObject
{
public:
    // SHIP
    virtual uint32_t GetType() { Ship::type; }
    const static uint32_t type = 0x6FB799E0; 
    
protected:
    virtual void OnReliableUpdate() {
    }
private:

    std::vector<ShipList<float>> ukn1;                              // update 1
    std::vector<ShipList<float>> ukn2;                              // update 1
    std::vector<ShipList<float>> energy_drain_;                     // update 1
    std::vector<ShipList<float>> mass_;                             // update 1
    std::vector<ShipList<std::wstring>> custom_component_names_;    // update 1
    uint32_t ukn3_list;                                             // update 1
    std::vector<ShipList<float>> weapon_max_;                       // update 1
    std::vector<ShipList<float>> weapon_min_;                       // update 1
    std::vector<ShipList<float>> dmg_vs_shields_;                   // update 1
    std::vector<ShipList<float>> dmg_vs_armor_;                     // update 1
    std::vector<ShipList<float>> energy_per_shot_;                  // update 1
    std::vector<ShipList<float>> refire_rate_;                      // update 1
    std::vector<ShipList<uint32_t>> ukn4;                           // update 1
    std::vector<ShipList<uint32_t>> ukn5;                           // update 1
    float total_mass_;                                              // update 1
    float shield_regeneration_rate_;                                // update 1
    float capacitor_max_;                                           // update 1
    float unkow1;                                                   // update 1
    float unkow2;                                                   // update 1
    float unkow3;                                                   // update 1
    float yaw_acceleration_;                                        // update 1
    float pitch_acceleration_;                                      // update 1
    float roll_acceleration_;                                       // update 1
    float yaw_max_;                                                 // update 1
    float pitch_max_;                                               // update 1
    float roll_max_;                                                // update 1
    float max_speed;                                                // update 1
    float total_energy_;                                            // update 1
    float booster_energy_;                                          // update 1
    float booster_recharge_rate_;                                   // update 1
    float booster_consumption_rate_;                                // update 1
    float booster_acceleration_;                                    // update 1
    float top_booster_speed_;                                       // update 1
    float droid_command_speed_;                                     // update 1
    uint64_t navicomputer_id_;                                      // update 1
    uint32_t unkow4;                                                // update 1
    uint32_t unkow5;                                                // update 1
    uint32_t unkow6;                                                // update 1
    uint32_t unkow7;                                                // update 1
    uint32_t unkow8;                                                // update 1
};
    
}}  // namespace

#endif  // SWGANH_OBJECT_SHIP_H_

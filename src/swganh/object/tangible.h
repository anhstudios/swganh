
#ifndef SWGANH_OBJECT_TANGIBLE_H_
#define SWGANH_OBJECT_TANGIBLE_H_

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {
    
class Tangible : public BaseObject
{
    std::vector<uint8_t> customization;                 // update 3
    std::vector<uint32_t> component_customization_list; // update 3
    uint32_t component_customization_list_counter;      // update 3
    uint32_t options_bitmask;                           // update 3
    uint32_t incap_timer;                               // update 3
    uint32_t condition_damage;                          // update 3
    uint32_t max_condition;                             // update 3
    bool is_static;                                     // update 3
    std::set<uint64_t> defender_list;                   // update 6
    uint32_t defender_list_counter;                     // update 6
};
    
}}  // namespace

#endif  // SWGANH_OBJECT_TANGIBLE_H_

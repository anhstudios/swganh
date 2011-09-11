
#ifndef SWGANH_OBJECT_TANGIBLE_H_
#define SWGANH_OBJECT_TANGIBLE_H_

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {
    
class Tangible : public BaseObject
{
private:
    std::vector<uint8_t> customization_;                 // update 3
    std::vector<uint32_t> component_customization_list_; // update 3
    uint32_t component_customization_list_counter_;      // update 3
    uint32_t options_bitmask_;                           // update 3
    uint32_t incap_timer_;                               // update 3
    uint32_t condition_damage_;                          // update 3
    uint32_t max_condition_;                             // update 3
    bool is_static_;                                     // update 3
    std::set<uint64_t> defender_list_;                   // update 6
    uint32_t defender_list_counter_;                     // update 6
	// crafting session
	// Both can be set to 0 ??
	// uint64_t manufacturing_schematic_;
	// uint64_t prototype_object_;
};
    
}}  // namespace

#endif  // SWGANH_OBJECT_TANGIBLE_H_

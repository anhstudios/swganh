
#ifndef SWGANH_COMMAND_COMMAND_PROPERTIES_H_
#define SWGANH_COMMAND_COMMAND_PROPERTIES_H_

#include <cstdint>
#include <string>
#include <map>

namespace swganh {
namespace command {

	struct CommandProperties
    {
        std::string name;
        uint32_t name_crc;
        std::string ability;
        uint32_t ability_crc;
        uint64_t deny_in_states;
        std::string script_hook;
        std::string fail_script_hook;
        uint64_t default_time;
        uint8_t command_group;
        float max_range_to_target;
        uint8_t add_to_combat_queue;
        uint32_t health_cost;
        uint32_t health_cost_multiplier;
        uint32_t action_cost;
        uint32_t action_cost_multiplier;
        uint32_t mind_cost;
        uint32_t mind_cost_multiplier;
        float damage_multiplier;
        float delay_multiplier;
        uint32_t force_cost;
        uint32_t force_cost_multiplier;
        uint32_t animation_crc;
        uint32_t required_weapon_group;
        std::string combat_spam;
        uint8_t trail1;
        uint8_t trail2;
        uint64_t allow_in_posture;
        float health_hit_chance;
        float action_hit_chance;
        float mind_hit_chance;
        float knockdown_hit_chance;
        float dizzy_hit_chance;
        float blind_chance;
        float stun_chance;
        float intimidate_chance;
        float posture_down_chance;
        float extended_range;
        float cone_angle;
        uint64_t deny_in_locomotion;
        std::map<float, std::string> getStates() { 
            std::map<float, std::string> states;
            states.insert(std::make_pair(knockdown_hit_chance, "knockdown"));
            states.insert(std::make_pair(dizzy_hit_chance, "dizzy"));
            states.insert(std::make_pair(blind_chance, "blind"));
            states.insert(std::make_pair(stun_chance, "stun"));
            states.insert(std::make_pair(intimidate_chance, "intimidate"));
            states.insert(std::make_pair(posture_down_chance, "posture_down"));
            return states;
        }
    };

	typedef std::map<
		uint32_t,
		CommandProperties
	> CommandPropertiesMap;

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_PROPERTIES_H_

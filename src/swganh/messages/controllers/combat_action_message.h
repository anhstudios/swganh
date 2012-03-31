// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_COMBAT_ACTION_MESSAGE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMBAT_ACTION_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    struct CombatDefender{
        uint64_t defender_id;
        uint8_t defender_end_posture;
        uint8_t hit_type;
        uint8_t defender_special_move_effect;
    };
    
    class CombatActionMessage : public ObjControllerMessage
    {
    public:
        explicit CombatActionMessage(uint32_t controller_type = 0x0000001B)
            : ObjControllerMessage(controller_type, message_type())
            , action_crc(0)
            , attacker_id(0)
            , attacker_end_posture(0)
            , trails_bit_flag(0xFF)
            , combat_special_move_effect(0xFF)
    		, defender_list(std::vector<CombatDefender>())
        {}

        explicit CombatActionMessage(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

    	static uint32_t message_type() { return 0x000000CC; }
        
    	uint32_t action_crc;
    	uint64_t attacker_id;
    	uint64_t weapon_id;
    	uint8_t attacker_end_posture;
    	uint8_t trails_bit_flag;
    	uint8_t combat_special_move_effect;
    
    	std::vector<CombatDefender> defender_list;                         
    
    	void OnControllerSerialize(anh::ByteBuffer& buffer) const {
    		buffer.write(action_crc);
    		buffer.write(attacker_id);
    		buffer.write(weapon_id);
    		buffer.write(attacker_end_posture);
    		buffer.write(trails_bit_flag);
    		buffer.write(combat_special_move_effect);
    
            std::for_each(begin(defender_list), end(defender_list),[&buffer](CombatDefender defender){
                buffer.write<uint32_t>(0);
                buffer.write(defender.defender_id);
                buffer.write(defender.defender_end_posture);
                buffer.write(defender.hit_type);
                buffer.write(defender.defender_special_move_effect);
            });
    	}
    
    	void OnControllerDeserialize(anh::ByteBuffer buffer) {
    		action_crc = buffer.read<uint32_t>();
    		attacker_id = buffer.read<uint64_t>();
    		weapon_id = buffer.read<uint64_t>();
    		attacker_end_posture = buffer.read<uint8_t>();
    		trails_bit_flag = buffer.read<uint8_t>();
    		combat_special_move_effect = buffer.read<uint8_t>();
    		int defender_size = buffer.read<uint16_t>();
    		for(int i = 0; i< defender_size; i++)
    		{
                CombatDefender list;
                list.defender_id = buffer.read<uint64_t>();
                list.defender_end_posture = buffer.read<uint8_t>();
                list.hit_type = buffer.read<uint8_t>();
                list.defender_special_move_effect = buffer.read<uint8_t>();
    			defender_list.push_back(list);
    		}
    	}
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_COMBAT_ACTION_MESSAGE_H_

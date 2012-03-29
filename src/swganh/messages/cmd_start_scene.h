// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CMD_START_SCENE_H_
#define SWGANH_MESSAGES_CMD_START_SCENE_H_

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct CmdStartScene : public BaseSwgMessage<CmdStartScene>
    {
        static uint16_t Opcount() { return 8; }
        static uint32_t Opcode() { return 0x3AE6DFAE; }
        
        uint8_t ignore_layout;
        uint64_t character_id;
        std::string terrain_map;
        glm::vec3 position;
        std::string shared_race_template;
        uint64_t galaxy_time;
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
        	buffer.write(ignore_layout);
        	buffer.write(character_id);
        	buffer.write(terrain_map);
        	buffer.write(position.x);
        	buffer.write(position.y);
        	buffer.write(position.z);
        	buffer.write(shared_race_template);
        	buffer.write(galaxy_time);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
        	ignore_layout = buffer.read<uint8_t>();
        	character_id = buffer.read<uint64_t>();
        	terrain_map = buffer.read<std::string>();
        	position.x = buffer.read<float>();
        	position.y = buffer.read<float>();
        	position.z = buffer.read<float>();
        	shared_race_template = buffer.read<std::string>();
        	galaxy_time = buffer.read<uint64_t>();    	
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_CMD_START_SCENE_H_

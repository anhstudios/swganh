/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
 
#ifndef SWGANH_SCENE_MESSAGES_CMD_START_SCENE_H_
#define SWGANH_SCENE_MESSAGES_CMD_START_SCENE_H_

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace scene {
namespace messages {
    
struct CmdStartScene : public swganh::base::SwgMessage<CmdStartScene> {
    static uint16_t opcount() { return 8; }
    static uint32_t opcode() { return 0x3AE6DFAE; }
    
    uint8_t ignore_layout;
    uint64_t character_id;
    std::string terrain_map;
    glm::vec3 position;
    std::string shared_race_template;
    uint64_t galaxy_time;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
    	buffer.write(ignore_layout);
    	buffer.write(character_id);
    	buffer.write(terrain_map);
    	buffer.write(position.x);
    	buffer.write(position.y);
    	buffer.write(position.z);
    	buffer.write(shared_race_template);
    	buffer.write(galaxy_time);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
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

}}}  // namespace swganh::scene::messages

#endif  // SWGANH_SCENE_MESSAGES_CMD_START_SCENE_H_

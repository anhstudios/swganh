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
 
#ifndef SWGANH_ZONE_SCENE_MESSAGES_SCENE_CREATE_OBJECT_BY_CRC_H_
#define SWGANH_ZONE_SCENE_MESSAGES_SCENE_CREATE_OBJECT_BY_CRC_H_

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace zone {
namespace scene {
namespace messages {
    
struct SceneCreateObjectByCrc : public swganh::base::SwgMessage<SceneCreateObjectByCrc> {
    static const uint16_t opcount = 5;
    static const uint32_t opcode = 0xFE89DDEA;
    
    uint64_t object_id;
    glm::quat orientation;
    glm::vec3 position;
    uint32_t object_crc;
    uint8_t byte_flag;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
    	buffer.write(object_id);
        buffer.write(orientation.x);
        buffer.write(orientation.y);
        buffer.write(orientation.z);
        buffer.write(orientation.w);
        buffer.write(position.x);
        buffer.write(position.y);
        buffer.write(position.z);
        buffer.write(object_crc);
        buffer.write(byte_flag);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
    	object_id = buffer.read<uint64_t>();
        orientation.x = buffer.read<float>();
        orientation.y = buffer.read<float>();
        orientation.z = buffer.read<float>();
        orientation.w = buffer.read<float>();
        position.x = buffer.read<float>();
        position.y = buffer.read<float>();
        position.z = buffer.read<float>();
        object_crc = buffer.read<uint32_t>();
        byte_flag = buffer.read<uint8_t>();
    }
};

}}}}  // namespace swganh::zone::scene::messages

#endif  // SWGANH_ZONE_SCENE_MESSAGES_SCENE_CREATE_OBJECT_BY_CRC_H_

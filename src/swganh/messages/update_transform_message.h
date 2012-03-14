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
 
#ifndef SWGANH_MESSAGES_UPDATE_TRANSFORM_MESSAGE_H_
#define SWGANH_MESSAGES_UPDATE_TRANSFORM_MESSAGE_H_

#include <cstdint>
#include <glm/glm.hpp>

#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {
    
struct UpdateTransformMessage : public swganh::messages::BaseSwgMessage<UpdateTransformMessage> {
    static uint16_t opcount() { return 8; }
    static uint32_t opcode() { return 0x1B24F808; }
    
    uint64_t object_id;
    glm::vec3 position;
    uint32_t update_counter;
    uint8_t posture_id;
    uint8_t heading;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(object_id);
        buffer.write<int16_t>(static_cast<int16_t>(position.x * 4.0f + 0.5f));
        buffer.write<int16_t>(static_cast<int16_t>(position.y * 4.0f + 0.5f));
        buffer.write<int16_t>(static_cast<int16_t>(position.z * 4.0f + 0.5f));
        buffer.write(update_counter);
        buffer.write(static_cast<uint8_t>(0));
        buffer.write(heading);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        object_id = buffer.read<uint64_t>();
        position.x = buffer.read<int16_t>();
        position.y = buffer.read<int16_t>();
        position.z = buffer.read<int16_t>();
        update_counter = buffer.read<uint32_t>();
        posture_id = buffer.read<uint8_t>();
        heading = buffer.read<uint8_t>();
    }
};

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_UPDATE_TRANSFORM_MESSAGE_H_

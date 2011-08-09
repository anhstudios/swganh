/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SWGANH_SCENE_MESSAGES_UDPATE_CONTAINMENT_MESSAGE_H_
#define SWGANH_SCENE_MESSAGES_UDPATE_CONTAINMENT_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

// Originates on Server
// http://wiki.swganh.org/index.php/UpdateContainmentMessage
namespace swganh {
namespace scene {
namespace messages {
    
struct UpdateContainmentMessage : public swganh::base::SwgMessage<UpdateContainmentMessage> {
    static uint16_t opcount() { return 4; }
    static uint32_t opcode() { return 0x56CBDE9E; }
    
    uint64_t object_id;
    uint64_t container_id;
    int32_t slot_index;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
    	buffer.write(object_id);
        buffer.write(container_id);
        buffer.write(slot_index);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
    	object_id = buffer.read<uint64_t>();
        container_id = buffer.read<uint64_t>();
        slot_index = buffer.read<int32_t>();
    }
};

}}} // swganh::scene::messages

#endif // SWGANH_SCENE_MESSAGES_UDPATE_CONTAINMENT_MESSAGE_H_
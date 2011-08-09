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

#ifndef SWGANH_CHARACTER_MESSAGES_DELETE_CHARACTER_MESSAGE_H_
#define SWGANH_CHARACTER_MESSAGES_DELETE_CHARACTER_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace character {
namespace messages {
    
struct DeleteCharacterMessage : public swganh::base::SwgMessage<DeleteCharacterMessage> {
    static uint16_t opcount() { return 3; }
    static uint32_t opcode() { return 0xE87AD031; }

    int32_t server_id;
    uint64_t character_id;
    
    void onDeserialize(anh::ByteBuffer buffer) {
        server_id = buffer.read<int32_t>();
        character_id = buffer.read<uint64_t>();
    }
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(server_id);
        buffer.write(character_id);
    }
};

}}}  // namespace swganh::character::messages

#endif  // SWGANH_CHARACTER_MESSAGES_DELETE_CHARACTER_MESSAGE_H_

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

#ifndef SWGANH_CONNECTION_MESSAGES_CLIENT_PERMISSIONS_MESSAGE_H_
#define SWGANH_CONNECTION_MESSAGES_CLIENT_PERMISSIONS_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace connection {
namespace messages {
    
struct ClientPermissionsMessage : public swganh::base::SwgMessage<ClientPermissionsMessage> {
    static uint16_t opcount() { return 4; }
    static uint32_t opcode() { return 0xE00730E5; }
    
    uint8_t galaxy_available;
    uint8_t available_character_slots;
    uint8_t unlimited_characters;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
    	buffer.write(galaxy_available);
    	buffer.write(available_character_slots);
    	buffer.write(unlimited_characters);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
    	galaxy_available = buffer.read<uint8_t>();
    	available_character_slots = buffer.read<uint8_t>();
    	unlimited_characters = buffer.read<uint8_t>();
    }
};

}}}  // namespace swganh::connection::messages

#endif  // SWGANH_CONNECTION_MESSAGES_CLIENT_PERMISSIONS_MESSAGE_H_

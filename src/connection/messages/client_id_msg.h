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

#ifndef CONNECTION_MESSAGES_CLIENT_ID_MSG_H_
#define CONNECTION_MESSAGES_CLIENT_ID_MSG_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace connection {
namespace messages {
    
struct ClientIdMsg : public swganh::base::SwgMessage<ClientIdMsg> {
    static const uint16_t opcount = 3;
    static const uint32_t opcode = 0xD5899226;    
    
    std::string session_hash;


    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(session_hash);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        int temp = buffer.read<uint32_t>();
        int size = buffer.read<uint32_t>();
        session_hash = buffer.read<std::string>();
    }
};

}}  // namespace connection::messages

#endif  // CONNECTION_MESSAGES_CLIENT_ID_MSG_H_

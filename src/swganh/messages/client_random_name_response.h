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

#ifndef SWGANH_MESSAGES_CLIENT_RANDOM_NAME_RESPONSE_H_
#define SWGANH_MESSAGES_CLIENT_RANDOM_NAME_RESPONSE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {
    
struct ClientRandomNameResponse : public swganh::messages::BaseSwgMessage<ClientRandomNameResponse> {
    static uint16_t opcount() { return 4; }
    static uint32_t opcode() { return 0xE85FB868; }
    
    std::string player_race_iff;
    std::wstring random_name;
    std::string stf_file;
    std::string approval_string;

    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(player_race_iff);	
        buffer.write(random_name);
        buffer.write(stf_file);
        buffer.write(0);
        buffer.write(approval_string);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
    	player_race_iff = buffer.read<std::string>();
        random_name = buffer.read<std::wstring>();
        stf_file = buffer.read<std::string>();
        buffer.read<int>(); // spacer, unused
        approval_string = buffer.read<std::string>();
    }
};

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_CLIENT_RANDOM_NAME_RESPONSE_H_

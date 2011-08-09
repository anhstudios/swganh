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

#ifndef SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_FAILED_H_
#define SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_FAILED_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace character {
namespace messages {
    
struct ClientCreateCharacterFailed : public swganh::base::SwgMessage<ClientCreateCharacterFailed> {
    static uint16_t opcount() { return 3; }
    static uint32_t opcode() { return 0xDF333C6E; }
    
    std::wstring unk1;
    std::string stf_file;
    std::string error_string;

    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(unk1);
        buffer.write(stf_file);
        buffer.write(0);
        buffer.write(error_string);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
    	unk1 = buffer.read<std::wstring>();
        stf_file = buffer.read<std::string>();
        buffer.read<uint32_t>();
        error_string = buffer.read<std::string>();
    }
};

}}}  // namespace swganh::character::messages

#endif  // SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_FAILED_H_

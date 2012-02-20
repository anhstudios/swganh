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

#ifndef SWGANH_MESSAGES_CHAT_SYSTEM_MESSAGE_H_
#define SWGANH_MESSAGES_CHAT_SYSTEM_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {
    
struct ChatSystemMessage : public swganh::messages::BaseSwgMessage<ChatSystemMessage> {
    static uint16_t opcount() { return 4; }
    static uint32_t opcode() { return 0x6D2A6413; }

    uint8_t display_type;
    std::wstring message;
    std::wstring out_of_band;

    void AddProsePackage(const anh::ByteBuffer* prose_package)
    {
        prose_package_ = prose_package;
    }

    void onSerialize(anh::ByteBuffer& buffer) const 
    {
        buffer.write(display_type);
        if (message.size())
        {
            buffer.write(message);
            buffer.write<uint32_t>(0);
        }
        if (prose_package_->size())
        {
            buffer.write<uint32_t>(0);
            buffer.append(*prose_package_);
        }
        else {
            buffer.write(out_of_band);
        }
    }
    void onDeserialize(anh::ByteBuffer buffer) 
    {
        display_type = buffer.read<uint8_t>();
        message = buffer.read<std::wstring>();
        buffer.read<uint32_t>();
        out_of_band = buffer.read<std::wstring>();
    }

private :
    const anh::ByteBuffer* prose_package_;
};

}}

#endif // SWGANH_MESSAGES_CHAT_SYSTEM_MESSAGE_H_
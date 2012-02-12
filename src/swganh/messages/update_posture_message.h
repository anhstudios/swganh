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
 
#ifndef SWGANH_MESSAGES_UPDATE_POSTURE_MESSAGE_H_
#define SWGANH_MESSAGES_UPDATE_POSTURE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {
class UpdatePostureMessage {
public:
    static uint32_t header() { return 0x131; }
    
    uint8_t posture_id;
    
    void Serialize(anh::ByteBuffer& buffer) const {
        buffer.write(posture_id);
		buffer.write<uint8_t>(1);
    }

    void Deserialize(anh::ByteBuffer buffer) {
        posture_id = buffer.read<uint8_t>();
    }
};

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_UPDATE_POSTURE_MESSAGE_H_

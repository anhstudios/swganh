/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef PACKETS_BASE_ANH_PACKET_H_
#define PACKETS_BASE_ANH_PACKET_H_

#include <anh/byte_buffer.h>
#include <anh/hash_string.h>

namespace packets {

template<typename T>
struct BaseANHPacket {
    // header
    uint32_t process_id;
    uint16_t priority_id;
    uint64_t time_stamp_id;
    
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write(T::hash_type());
        buffer.write(priority_id);
        buffer.write(process_id);
        buffer.write(time_stamp_id);
        
        onSerialize(buffer);
    }
    void deserialize(anh::ByteBuffer buffer) {
        anh::HashString hash_type = buffer.read<anh::HashString>();
        priority_id = buffer.read<uint16_t>();
        process_id = buffer.read<uint32_t>();
        time_stamp_id = buffer.read<uint64_t>();
        
        if (hash_type != T::hash_type())
        {
            return;
        }

        onDeserialize(std::move(buffer));
    }
    virtual void onSerialize(anh::ByteBuffer& buffer) const = 0;
    virtual void onDeserialize(anh::ByteBuffer buffer) = 0;
};

}
#endif // PACKETS_BASE_ANH_PACKET_H_
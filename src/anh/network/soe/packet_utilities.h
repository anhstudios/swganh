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

#ifndef ANH_NETWORK_CLUSTER_PACKET_UTILITIES_
#define ANH_NETWORK_CLUSTER_PACKET_UTILITIES_

#include <cstdint>
#include <algorithm>
#include <list>
#include "anh/byte_buffer.h"
#include "anh/utilities.h"

namespace anh {
namespace network {
namespace soe {

inline anh::ByteBuffer packDataChannelMessages(std::list<anh::ByteBuffer>& data_list) 
{
    anh::ByteBuffer output_buffer;
    // If there is only one message then no need to pack, just move the message
    // into the output buffer.
    if (data_list.size() == 1) {        
        output_buffer.append(data_list.front());
        return output_buffer;
    }

    // Otherwise we need to prepend a header to the whole thing (0x0019)
    output_buffer.write<uint16_t>(anh::bigToHost<uint16_t>(0x19));
    
    // Then loop through each message and append it to the output buffer with a size prefix.
    std::for_each(
        data_list.begin(), 
        data_list.end(), 
        [=, &output_buffer](anh::ByteBuffer& item)
    {
        // For messages with a size greater than 255 an 8 byte int is not large enough to
        // hold the size value, in this case we need a little endian uint16_t size.
        if (item.size() > 255) {
            output_buffer.write<uint16_t>(anh::hostToLittle<uint16_t>(item.size()));
        } else {
            output_buffer.write<uint8_t>(item.size());
        }

        output_buffer.append(item);
    });

    return output_buffer;
}

}  // namespace soe
}  // namespace network
}  // namespace anh
#endif // ANH_NETWORK_CLUSTER_PACKET_UTILITIES_
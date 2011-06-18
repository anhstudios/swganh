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
 
#include "anh/network/soe/packet_utilities.h"

#include <algorithm>
#include "anh/utilities.h"

using namespace anh;
using namespace std;

namespace anh {
namespace network {
namespace soe {

ByteBuffer BuildDataChannelHeader(uint16_t sequence) {
    ByteBuffer data_channel_header;

    data_channel_header.write<uint16_t>(hostToBig<uint16_t>(0x09));
    data_channel_header.write<uint16_t>(hostToBig<uint16_t>(sequence));

    return data_channel_header;
}

ByteBuffer PackDataChannelMessages(list<shared_ptr<ByteBuffer>>& data_list) {
    ByteBuffer output_buffer;

    // If there is only one message then no need to pack, just move the message
    // into the output buffer.
    if (data_list.size() == 1) {        
        output_buffer.append(*(data_list.front()));
        return output_buffer;
    }

    // Otherwise we need to prepend a header to the whole thing (0x0019)
    output_buffer.write<uint16_t>(hostToBig<uint16_t>(0x19));
    
    // Then loop through each message and append it to the output buffer with a size prefix.
    std::for_each(
        data_list.begin(), 
        data_list.end(), 
        [=, &output_buffer](shared_ptr<ByteBuffer>& item)
    {
        // For messages with a size greater than 254 bytes an 8 byte int is not large enough to
        // hold the size value, in this case we need a little endian uint16_t size.
        if (item->size() >= 255) {
            output_buffer.write<uint8_t>(0xFF);
            output_buffer.write<uint16_t>(hostToBig<uint16_t>(item->size()));
        } else {
            output_buffer.write<uint8_t>(item->size());
        }

        output_buffer.append(*item);
    });

    return output_buffer;
}

}}}  // namespace anh::network::soe

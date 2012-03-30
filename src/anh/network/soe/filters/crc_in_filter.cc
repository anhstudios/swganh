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

#include "crc_in_filter.h"

#include "anh/byte_buffer.h"
#include "anh/crc.h"
#include "anh/network/soe/session.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;

void CrcInFilter::operator()(Session* session, ByteBuffer* message) const
{
    uint32_t crc_length = session->crc_length();
    
    // If crc length is 0 then crc testing is not being used, return the packet.
    if (crc_length == 0)
    {
        return;
    }
        
    // Peel off the crc bits from the packet data.
    uint32_t message_size = message->size() - crc_length;
    vector<uint8_t> crc_bits(message->data() + message_size, message->data() + message->size());
    message->resize(message_size);
    
    uint32_t packet_crc = memcrc(message->data(), message->size(), session->crc_seed());
    uint32_t test_crc = 0;
    uint32_t mask = 0;

    for (uint32_t i = 0; i < crc_length; ++i) {
        test_crc |= (crc_bits[i] << (((crc_length - 1) -  i) * 8));
        mask <<= 8;
        mask |= 0xFF;
    }

    packet_crc &= mask;

    if (test_crc != packet_crc) 
    {
        throw runtime_error("Crc mismatch");
    }
}

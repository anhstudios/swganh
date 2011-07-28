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

#include "anh/network/soe/filters/crc_in_filter.h"

#include <algorithm>
#include <glog/logging.h>

#include "anh/byte_buffer.h"
#include "anh/crc.h"
#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

using namespace anh::network::soe;
using namespace filters;
using namespace std;

CrcInFilter::CrcInFilter(void) {}

shared_ptr<Packet> CrcInFilter::operator()(shared_ptr<Packet> packet) const {
    if (!packet) { return nullptr; }


    auto session = packet->session();

    // Don't do any processing on messages from non-connected sessions.
    if (!session->connected()) {
        return packet;
    }

    auto crc_length = session->crc_length();

    // If crc length is 0 then crc testing is not being used, return the packet.
    if (crc_length == 0) {
        return packet;
    }
    
    auto message = packet->message();

    // Verify the message size
    if (! (message->size() > crc_length)) {
        DLOG(WARNING) << "Invalid message received\n\n" << *message;
        return nullptr;
    }

    try {
        // Peel off the crc bits from the packet data.
        ByteBuffer packet_data(message->data(), message->size() - crc_length);
        vector<uint8_t> crc_bits(message->data() + message->size() - crc_length, message->data() + message->size());
        
        uint32_t packet_crc = memcrc(packet_data.data(), packet_data.size(), session->crc_seed());
        uint32_t test_crc = 0;
        uint32_t mask = 0;

        for (uint32_t i = 0; i < crc_length; ++i) {
            test_crc |= (crc_bits[i] << (((crc_length - 1) -  i) * 8));
            mask <<= 8;
            mask |= 0xFF;
        }

        packet_crc &= mask;

        if (test_crc != packet_crc) {       
            LOG(WARNING) << "Crc mismatch: packet[" << hex << packet_crc << "] test[" << hex << test_crc << "]\n\n" << *message;
            return nullptr;
        }
        
        message->swap(packet_data);
    } catch(...) {
        DLOG(WARNING) << "Error while performing crc checking on incoming packet\n\n" << *message;
    }

    return packet;
}

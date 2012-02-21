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

#include "anh/network/soe/filters/decompression_filter.h"

#include <array>

#include <glog/logging.h>
#include <zlib.h>

#include "anh/byte_buffer.h"
#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;

std::shared_ptr<Packet> DecompressionFilter::operator()(const std::shared_ptr<Packet>& packet) const {
    if (!packet) { return nullptr; }

    auto session = packet->session();
    
    // Don't do any processing on messages from non-connected sessions.
    if (!session->connected()) {
        return packet;
    }
    
    auto message = packet->message();

    try {        
        uint8_t compressed = message->peekAt<uint8_t>(message->size() - 1);

        ByteBuffer tmp(message->data(), message->size() - 1);
        message->swap(tmp);

        if(compressed == 1) {
            Decompress_(message);
        }
    } catch(...) {
        DLOG(WARNING) << "Exception thrown while decompressing message \n\n" << *message;
        return nullptr;
    }
    
    return packet;
}

void DecompressionFilter::Decompress_(shared_ptr<ByteBuffer>& buffer) const {
    std::vector<uint8_t>& packet_data = buffer->raw();

    uint16_t offset = (packet_data[0] == 0x00) ? 2 : 1;
    
    z_stream zstream_;

    zstream_.zalloc = Z_NULL;
    zstream_.zfree = Z_NULL;
    zstream_.opaque = Z_NULL;
    zstream_.avail_in= Z_NULL;
    zstream_.next_in = Z_NULL;
    
    inflateInit(&zstream_);

    std::vector<uint8_t> decompression_output(496);
    
    zstream_.next_in   = reinterpret_cast<Bytef *>(&packet_data[offset]);
    zstream_.avail_in  = packet_data.size() - offset;
    zstream_.next_out  = reinterpret_cast<Bytef *>(&decompression_output[0]);
    zstream_.avail_out = 496;

    inflate(&zstream_, Z_FINISH); // Decompress Data
    
    ByteBuffer decompressed(&packet_data[0], offset);
    decompressed.write(&decompression_output[0], zstream_.total_out);
    
    inflateEnd(&zstream_);

    buffer->swap(decompressed);
}

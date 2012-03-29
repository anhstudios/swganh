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

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;

DecompressionFilter::DecompressionFilter(uint32_t max_message_size)
    : max_message_size_(max_message_size)
    , decompression_output_(max_message_size_)
{}

void DecompressionFilter::operator()(Session* session, ByteBuffer* message)
{
    uint32_t size_without_compression_bit = message->size() - 1;
    uint8_t compressed_bit = message->peekAt<uint8_t>(size_without_compression_bit);
   
    message->resize(size_without_compression_bit);

    if(compressed_bit == 1) {
        Decompress_(message);
    }
}

void DecompressionFilter::Decompress_(ByteBuffer* buffer) 
{
    auto packet_data = buffer->raw();    

    uint16_t offset = (packet_data[0] == 0x00) ? 2 : 1;
    
    zstream_.zalloc = Z_NULL;
    zstream_.zfree = Z_NULL;
    zstream_.opaque = Z_NULL;
    zstream_.avail_in= Z_NULL;
    zstream_.next_in = Z_NULL;
    
    inflateInit(&zstream_);

    zstream_.next_in   = reinterpret_cast<Bytef *>(&packet_data[offset]);
    zstream_.avail_in  = packet_data.size() - offset;
    zstream_.next_out  = reinterpret_cast<Bytef *>(&decompression_output_[0]);
    zstream_.avail_out = decompression_output_.size();

    inflate(&zstream_, Z_FINISH); // Decompress Data
    
    buffer->write_position(offset);
    buffer->write(&decompression_output_[0], zstream_.total_out);
    
    inflateEnd(&zstream_);
}

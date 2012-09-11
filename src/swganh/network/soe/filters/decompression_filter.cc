// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/network/soe/filters/decompression_filter.h"

#include "swganh/byte_buffer.h"
#include "swganh/network/soe/session.h"

using namespace swganh;
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

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "decompression_filter.h"

#include "swganh/network/session.h"

using namespace swganh::network;
using swganh::ByteBuffer;

DecompressionFilter::DecompressionFilter(uint32_t max_message_size)
    : max_message_size_(max_message_size)
{}

void DecompressionFilter::operator()(Session* session, ByteBuffer* message)
{
    uint32_t size_without_compression_bit = message->size() - 1;
    uint8_t compressed_bit = message->peekAt<uint8_t>(size_without_compression_bit);
   
    message->resize(size_without_compression_bit);

    if(compressed_bit != 1) return;

    uint16_t offset = (message->peek<uint8_t>() == 0x00) ? 2 : 1;
    
    zbuffer_.resize(max_message_size_);
    zbuffer_.write(0, message->data(), offset);

    memset((void*)&zstream_, 0, sizeof(z_stream));    
    inflateInit(&zstream_);

    zstream_.next_in   = reinterpret_cast<Bytef *>(message->data() + offset);
    zstream_.avail_in  = message->size() - offset;
    zstream_.next_out  = reinterpret_cast<Bytef *>(zbuffer_.data() + offset);
    zstream_.avail_out = zbuffer_.size() - offset;

    inflate(&zstream_, Z_FINISH); // Decompress Data
    
    zbuffer_.resize(zstream_.total_out + offset);

    inflateEnd(&zstream_);

    message->swap(zbuffer_);
}

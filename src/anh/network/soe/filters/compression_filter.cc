// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "compression_filter.h"

#include <vector>

#include <zlib.h>

#include "swganh/byte_buffer.h"
#include "swganh/network/soe/session.h"

using namespace swganh;
using namespace network::soe;
using namespace filters;
using namespace std;

void CompressionFilter::operator()(Session* session, ByteBuffer* message)
{
    if(message->size() > session->receive_buffer_size() - 20)
    {
        Compress_(message);

        message->write<uint8_t>(1); // compressed
    } 
    else 
    {        
        message->write<uint8_t>(0); // not compressed
    }
}

void CompressionFilter::Compress_(ByteBuffer* message) 
{
    vector<uint8_t>& packet_data = message->raw();
    uint32_t packet_size = message->size();

    // Determine the offset to begin compressing data at
    uint16_t offset = (packet_data[0] == 0x00) ? 2 : 1;
    
    z_stream zstream_;

    zstream_.zalloc = Z_NULL;
    zstream_.zfree = Z_NULL;
    zstream_.opaque = Z_NULL;
    zstream_.avail_in = Z_NULL;
    zstream_.next_in = Z_NULL;

    deflateInit(&zstream_, Z_DEFAULT_COMPRESSION);

    vector<uint8_t> compression_output(packet_size);

    zstream_.next_in = reinterpret_cast<Bytef *>(&packet_data[offset]);
    zstream_.avail_in = packet_size - offset;
    zstream_.next_out = reinterpret_cast<Bytef *>(&compression_output[0]);
    zstream_.avail_out = packet_size;

    deflate(&zstream_, Z_FINISH);

    ByteBuffer compressed(&packet_data[0], offset);
    compressed.write(&compression_output[0], zstream_.total_out);
    
    deflateEnd(&zstream_);

    message->swap(compressed);
}

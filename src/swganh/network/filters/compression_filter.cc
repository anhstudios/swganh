// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "compression_filter.h"

#include <vector>

#include "swganh/network/session.h"

using namespace swganh::network;
using swganh::ByteBuffer;

void CompressionFilter::operator()(Session* session, ByteBuffer* message)
{
    if(message->size() <= session->receive_buffer_size() - 20) {
        message->write<uint8_t>(0); // not compressed
        return;
    }

    // Determine the offset to begin compressing data at
    uint16_t offset = (message->peek<uint8_t>() == 0x00) ? 2 : 1;
    
    memset((void*)&zstream_, 0, sizeof(z_stream));

    zbuffer_.resize(session->receive_buffer_size());
    zbuffer_.write(0, message->data(), offset);

    deflateInit(&zstream_, Z_DEFAULT_COMPRESSION);
    
    zstream_.next_in = reinterpret_cast<Bytef *>(message->data() + offset);
    zstream_.avail_in = message->size() - offset;
    zstream_.next_out = reinterpret_cast<Bytef *>(zbuffer_.data() + offset);
    zstream_.avail_out = zbuffer_.size() - offset;

    deflate(&zstream_, Z_FINISH);

    zbuffer_.resize(zstream_.total_out + offset);
    
    deflateEnd(&zstream_);

    message->swap(zbuffer_);
    message->write_position(message->size());
    message->write<uint8_t>(1);
}

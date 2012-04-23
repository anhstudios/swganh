// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "crc_out_filter.h"

#include "anh/byte_buffer.h"
#include "anh/crc.h"
#include "anh/network/soe/session.h"

using namespace anh;
using namespace network::soe;
using namespace filters;
using namespace std;

void CrcOutFilter::operator()(Session* session, ByteBuffer* message) 
{
    uint32_t packet_crc = memcrc(message->data(), message->size(), session->crc_seed());
    
    uint8_t crc_low = (uint8_t)packet_crc;
    uint8_t crc_high = (uint8_t)(packet_crc >> 8);
    
    message->write<uint8_t>(crc_high);
    message->write<uint8_t>(crc_low);
}

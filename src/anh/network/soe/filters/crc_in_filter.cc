// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "crc_in_filter.h"

#include "swganh/byte_buffer.h"
#include "swganh/crc.h"
#include "swganh/network/soe/session.h"

using namespace swganh;
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

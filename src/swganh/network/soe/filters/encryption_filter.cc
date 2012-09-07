// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/network/soe/filters/encryption_filter.h"

#include "swganh/byte_buffer.h"
#include "swganh/network/soe/session.h"

using namespace swganh;
using namespace network::soe;
using namespace filters;
using namespace std;

void EncryptionFilter::operator()(Session* session, ByteBuffer* message)
{
    uint16_t offset = (message->peek<uint8_t>() == 0x00) ? 2 : 1;
            
    Encrypt_(
        (char*)message->data() + offset,
        message->size() - offset, 
        session->crc_seed());
}

void EncryptionFilter::Encrypt_(char* data, uint32_t len, uint32_t seed) const
{
    uint32_t blockCount = (len / 4);
    uint32_t byteCount = (len % 4);

    for(uint32_t count = 0; count < blockCount; count++) {
        ((uint32_t*)data)[count] ^= seed;
        seed = ((uint32_t*)data)[count];
    }

    for(uint32_t count = blockCount * 4; count < blockCount * 4 + byteCount; count++) {
        data[count] ^= seed;
    }
}

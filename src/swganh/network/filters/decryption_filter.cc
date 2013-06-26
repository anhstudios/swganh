// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "decryption_filter.h"

#include "swganh/byte_buffer.h"
#include "swganh/network/session.h"

using namespace swganh::network;
using swganh::ByteBuffer;

void DecryptionFilter::operator()(Session* session, ByteBuffer* message) const
{    
    if (message->size() <= 2)
    {
        throw std::runtime_error("Invalid message size");
    }

    uint16_t offset = (message->peek<uint8_t>() == 0x00) ? 2 : 1;

    Decrypt_((char*)message->data() + offset, 
            message->size() - offset, 
            session->crc_seed());
}

int DecryptionFilter::Decrypt_(
    char* buffer, 
    uint32_t len, 
    uint32_t seed) const 
{
    int retVal = 0;

    uint32_t tempSeed = 0;
    uint32_t blockCount = (len / 4);
    uint32_t byteCount = (len % 4);

    for(uint32_t count = 0; count < blockCount; count++)
    {
        tempSeed = ((uint32_t*)buffer)[count];
        ((uint32_t*)buffer)[count] ^= seed;
        seed = tempSeed;
    }

    for(uint32_t count = blockCount * 4; 
        count < blockCount * 4 + byteCount; 
        count++)
    {
        buffer[count] ^= seed;
    }

    return retVal;
}

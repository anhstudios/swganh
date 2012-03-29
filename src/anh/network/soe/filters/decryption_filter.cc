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

#include "anh/network/soe/filters/decryption_filter.h"

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"

using namespace anh;
using namespace anh::network::soe;
using namespace filters;
using namespace std;

void DecryptionFilter::operator()(Session* session, ByteBuffer* message) const
{    
    if (message->size() <= 2)
    {
        throw runtime_error("Invalid message size");
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

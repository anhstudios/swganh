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

#include <iostream>
#include <glog/logging.h>

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/packet.h"
#include "anh/network/soe/protocol_opcodes.h"

using namespace anh::network::soe;
using namespace filters;
using namespace std;

shared_ptr<Packet> DecryptionFilter::operator()(shared_ptr<Packet> packet) const {
    if (!packet) { return nullptr; }

    auto session = packet->session();
    
    // Don't do any processing on messages from non-connected sessions.
    if (!session->connected()) {
        return packet;
    }
    
    auto message = packet->message();

    try {
        if (! (message->size() > 2)) {
            DLOG(WARNING) << "Invalid message size\n\n" << *message;
            return nullptr;
        }

        uint16_t offset = (message->peek<uint8_t>() == 0x00) ? 2 : 1;

        Decrypt_((char*)message->data() + offset, 
                message->size() - offset, 
                packet->session()->crc_seed());
    } catch(...) {
        DLOG(WARNING) << "Error while decrypting packet\n\n" << *message;
    }

    return packet;
}

int DecryptionFilter::Decrypt_(char* buffer, uint32_t len, uint32_t seed) const {
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

    for(uint32_t count = blockCount * 4; count < blockCount * 4 + byteCount; count++)
    {
        buffer[count] ^= seed;
    }

    return retVal;
}

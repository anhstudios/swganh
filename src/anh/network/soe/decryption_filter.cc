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

#include "anh/network/soe/decryption_filter.h"

#include <iostream>
#include <glog/logging.h>

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/incoming_packet.h"
#include "anh/network/soe/protocol_opcodes.h"

using namespace anh::network::soe;
using namespace std;

shared_ptr<IncomingPacket> DecryptionFilter::operator()(shared_ptr<IncomingPacket> packet) const {
	Decrypt_((char*)packet->message()->data()+2, packet->message()->size()-4, packet->session()->crc_seed());
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

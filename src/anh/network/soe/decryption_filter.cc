/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#include <anh/byte_buffer.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/incoming_packet.h>
#include <anh/network/soe/decryption_filter.h>
#include <anh/network/soe/protocol_opcodes.h>

#ifdef ERROR
#undef ERROR
#endif

#include <iostream>
#include <glog/logging.h>

namespace anh {
namespace network {
namespace soe {

DecryptionFilter::DecryptionFilter(Service* service)
	: service_(service)
{
}

DecryptionFilter::~DecryptionFilter(void)
{
}

IncomingPacket* DecryptionFilter::operator()(IncomingPacket* item) const
{
	IncomingPacket* packet = (IncomingPacket*)item;
	Decrypt_((char*)packet->message()->data()+2, packet->message()->size()-4, service_->crc_seed_);
	return packet;
}

int DecryptionFilter::Decrypt_(char* buffer, uint32_t len, uint32_t seed) const
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

    for(uint32_t count = blockCount * 4; count < blockCount * 4 + byteCount; count++)
    {
        buffer[count] ^= seed;
    }

    return retVal;
}

} // namespace soe
} // namespace network
} // namespace anh
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

#include <anh/network/soe/service.h>
#include <anh/network/soe/crc_in_filter.h>
#include <anh/network/soe/incoming_packet.h>
#include <anh/byte_buffer.h>
#include <anh/crc.h>


#include <glog/logging.h>

namespace anh {
namespace network {
namespace soe {

CrcInFilter::CrcInFilter(Service* service)
	: tbb::filter(parallel)
	, service_(service)
{
}

CrcInFilter::~CrcInFilter(void)
{
}

void* CrcInFilter::operator()(void* item)
{
	// TODO: ENDIANNESS?
	IncomingPacket* packet = (IncomingPacket*)item;
	
	uint32_t packet_crc = anh::memcrc((const char*)packet->message()->data(), packet->message()->size()-2, service_->crc_seed_);
	uint8_t crc_low = (uint8_t)*(packet->message()->data() + (packet->message()->size() - 1));
	uint8_t crc_high = (uint8_t)*(packet->message()->data() + (packet->message()->size() - 2));

	if(crc_low != (uint8_t)packet_crc || crc_high != (uint8_t)(packet_crc >> 8))
	{
		LOG(WARNING) << "Crc Mismatch [packet_crc = "<< std::hex << packet_crc << " high_byte = " << std::hex << crc_high << " low_byte = " << std::hex << crc_low << "]";
		delete packet;
		return NULL;
	}

	return packet;
}

} // namespace soe
} // namespace network
} // namespace anh
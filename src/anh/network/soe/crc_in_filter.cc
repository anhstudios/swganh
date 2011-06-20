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

#include "anh/network/soe/crc_in_filter.h"

#include <glog/logging.h>

#include "anh/byte_buffer.h"
#include "anh/crc.h"
#include "anh/network/soe/incoming_packet.h"
#include "anh/network/soe/session.h"

using namespace anh::network::soe;
using namespace std;

CrcInFilter::CrcInFilter(void) {}

shared_ptr<IncomingPacket> CrcInFilter::operator()(shared_ptr<IncomingPacket> packet) const {
	// TODO: ENDIANNESS?
	
	uint32_t packet_crc = anh::memcrc((const char*)packet->message()->data(), packet->message()->size()-2, packet->session()->crc_seed());
	uint8_t crc_low = (uint8_t)*(packet->message()->data() + (packet->message()->size() - 1));
	uint8_t crc_high = (uint8_t)*(packet->message()->data() + (packet->message()->size() - 2));

	if(crc_low != (uint8_t)packet_crc || crc_high != (uint8_t)(packet_crc >> 8))
	{
		LOG(WARNING) << "Crc Mismatch [packet_crc = "<< std::hex << packet_crc << " high_byte = " << std::hex << crc_high << " low_byte = " << std::hex << crc_low << "]";
		return nullptr;
	}

	return packet;
}

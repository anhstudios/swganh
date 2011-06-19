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

#include <anh/network/soe/crc_out_filter.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/outgoing_packet.h>
#include <anh/crc.h>

using namespace std;

namespace anh {
namespace network {
namespace soe {

CrcOutFilter::CrcOutFilter(Service* service)
	: service_(service)
{
}

CrcOutFilter::~CrcOutFilter(void)
{
}

shared_ptr<OutgoingPacket> CrcOutFilter::operator()(shared_ptr<OutgoingPacket> packet) const
{
	// TODO: ENDIANNESS?
	uint32_t packet_crc = anh::memcrc((const char*)packet->message()->data(), packet->message()->size()-2, service_->crc_seed_);
	uint8_t crc_low = (uint8_t)packet_crc;
	uint8_t crc_high = (uint8_t)(packet_crc >> 8);

	packet->message()->writeAt<uint8_t>(packet->message()->size() - 1, crc_low);
	packet->message()->writeAt<uint8_t>(packet->message()->size() - 2, crc_high);
	return packet;
}

} // namespace soe
} // namespace network
} // namespace anh
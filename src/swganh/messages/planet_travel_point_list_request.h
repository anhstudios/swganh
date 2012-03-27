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

#ifndef SWGANH_MESSAGES_PLANET_TRAVEL_POINT_LIST_REQUEST_H_
#define SWGANH_MESSAGES_PLANET_TRAVEL_POINT_LIST_REQUEST_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct PlanetTravelPointListRequest : public swganh::messages::BaseSwgMessage<PlanetTravelPointListRequest> {
	static uint16_t opcount() { return 3; }
	static uint32_t opcode() { return 0x96405D4D; }
	
	uint64_t travel_terminal_id;
	std::string planet_name;
	
	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(travel_terminal_id);
		buffer.write(planet_name);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		travel_terminal_id = buffer.read<uint64_t>();
		planet_name = buffer.read<std::string>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLANET_TRAVEL_POINT_LIST_REQUEST_H_
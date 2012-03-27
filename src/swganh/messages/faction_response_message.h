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

#ifndef SWGANH_MESSAGES_FACTION_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_FACTION_RESPONSE_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct FactionResponseMessage : public swganh::messages::BaseSwgMessage<FactionResponseMessage> {
	static uint16_t opcount() { return 6; }
	static uint32_t opcode() { return 0x5DD53957; }
	
	std::string faction_rank;
	uint32_t faction_points_rebel;
	uint32_t faction_points_imperial;
	uint32_t faction_points_hutt;
	std::list<std::string> faction_names;
	std::list<float> faction_points_list;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(faction_rank);
		buffer.write(faction_points_rebel);
		buffer.write(faction_points_imperial);
		buffer.write(faction_points_hutt);
		buffer.write(faction_names.size());
		std::for_each(faction_names.begin(), faction_names.end(), [&buffer] (std::string faction_name) {
			buffer.write(faction_name);
		});
		buffer.write(faction_points_list.size());
		std::for_each(faction_points_list.begin(), faction_points_list.end(), [&buffer] (float faction_points) {
			buffer.write(faction_points);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		faction_rank = buffer.read<std::string>();
		faction_points_rebel = buffer.read<uint32_t>();
		faction_points_imperial = buffer.read<uint32_t>();
		faction_points_hutt = buffer.read<uint32_t>();
		uint32_t faction_names_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < faction_names_count; i++) {
			std::string faction_name;
			faction_name = buffer.read<std::string>();
			faction_names.push_back(faction_name);
		}
		uint32_t faction_points_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < faction_points_count; i++) {
			float faction_points;
			faction_points = buffer.read<float>();
			faction_points_list.push_back(faction_points);
		}
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_FACTION_RESPONSE_MESSAGE_H_
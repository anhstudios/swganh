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

#ifndef SWGANH_MESSAGES_CREATE_CLIENT_PATH_MESSAGE_H_
#define SWGANH_MESSAGES_CREATE_CLIENT_PATH_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <glm/glm.hpp>
#include <list>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct CreateClientPathMessage : public swganh::messages::BaseSwgMessage<CreateClientPathMessage> {
	static uint16_t opcount() { return 5; }
	static uint32_t opcode() { return 0x71957628; }
	
	std::list<glm::vec3> path_coordinates;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(path_coordinates.size());
		std::for_each(path_coordinates.begin(), path_coordinates.end(), [&buffer] (glm::vec3 coordinate){
			buffer.write(coordinate.x);
			buffer.write(coordinate.z);
			buffer.write(coordinate.y);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		uint32_t coordinate_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < coordinate_count; i++) {
			glm::vec3 coordinate;
			coordinate.x = buffer.read<float>();
			coordinate.z = buffer.read<float>();
			coordinate.y = buffer.read<float>();
			path_coordinates.push_back(coordinate);
		}
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CREATE_CLIENT_PATH_MESSAGE_H_
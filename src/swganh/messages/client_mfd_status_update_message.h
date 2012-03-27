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

#ifndef SWGANH_MESSAGES_CLIENT_MFD_STATUS_UPDATE_MESSAGE_H_
#define SWGANH_MESSAGES_CLIENT_MFD_STATUS_UPDATE_MESSAGE_H_

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ClientMfdStatusUpdateMessage : public swganh::messages::BaseSwgMessage<ClientMfdStatusUpdateMessage> {
	static uint16_t opcount() { return 6; }
	static uint32_t opcode() { return 0x2D2D6EE1; }

	std::string planet_name;
	uint64_t object_id;
	glm::vec3 object_location;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(planet_name);
		buffer.write(object_id);
		buffer.write(object_location.x);
		buffer.write(object_location.y);
		buffer.write(object_location.z);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		planet_name = buffer.read<std::string>();
		object_id = buffer.read<uint64_t>();
		object_location.x = buffer.read<float>();
		object_location.y = buffer.read<float>();
		object_location.z = buffer.read<float>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CLIENT_MFD_STATUS_UPDATE_MESSAGE_H_
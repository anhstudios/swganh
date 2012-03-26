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

#ifndef SWGANH_MESSAGES_SERVER_WEATHER_MESSAGE_H_
#define SWGANH_MESSAGES_SERVER_WEATHER_MESSAGE_H_

#include <cstdint>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ServerWeatherMessage : public swganh::messages::BaseSwgMessage<ServerWeatherMessage> {
	static uint16_t opcount() { return 2; }
	static uint32_t opcode() { return 0x486356EA; }

	uint32_t weather_id;
	glm::vec3 cloud_vector;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(weather_id);
		buffer.write(cloud_vector.x);
		buffer.write(cloud_vector.y);
		buffer.write(cloud_vector.z);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		weather_id = buffer.read<uint32_t>();
		cloud_vector.x = buffer.read<float>();
		cloud_vector.y = buffer.read<float>();
		cloud_vector.z = buffer.read<float>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_SERVER_WEATHER_MESSAGE_H_
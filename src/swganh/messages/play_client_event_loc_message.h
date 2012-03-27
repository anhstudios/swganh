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

#ifndef SWGANH_MESSAGES_PLAY_CLIENT_EVENT_LOC_MESSAGE_H_
#define SWGANH_MESSAGES_PLAY_CLIENT_EVENT_LOC_MESSAGE_H_

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct PlayClientEventLocMessage : public swganh::messages::BaseSwgMessage<PlayClientEventLocMessage> {
	static uint16_t opcount() { return 9; }
	static uint32_t opcode() { return 0x0A4E222C; }

	std::string client_effect_file; // uncertain, because PlayClientEffectLocMessage is used for clienteffect files already
	std::string planet_name; // e.g. "naboo"
	glm::vec3 location_coordinates;
	uint64_t unknown1;
	uint32_t unknown2;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(client_effect_file);
		buffer.write(planet_name);
		buffer.write(location_coordinates.x);
		buffer.write(location_coordinates.z);
		buffer.write(location_coordinates.y);
		buffer.write(unknown1);
		buffer.write(unknown2);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		client_effect_file = buffer.read<std::string>();
		planet_name = buffer.read<std::string>();
		location_coordinates.x = buffer.read<float>();
		location_coordinates.z = buffer.read<float>();
		location_coordinates.y = buffer.read<float>();
		unknown1 = buffer.read<uint64_t>();
		unknown2 = buffer.read<uint32_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLAY_CLIENT_EVENT_LOC_MESSAGE_H_
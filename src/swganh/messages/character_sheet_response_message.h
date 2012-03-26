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

#ifndef SWGANH_MESSAGES_CHARACTER_SHEET_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_CHARACTER_SHEET_RESPONSE_MESSAGE_H_

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct CharacterSheetResponseMessage : public swganh::messages::BaseSwgMessage<CharacterSheetResponseMessage> {
	static uint16_t opcount() { return 1; }
	static uint32_t opcode() { return 0x9B3A17C4; }

	uint32_t unknown1;
	uint32_t unknown2;
	glm::vec3 bind_location;
	std::string bind_planet_name; // planet the player is bound to
	glm::vec3 bank_location;
	std::string bank_planet_name; // planet the player's bank is located on
	glm::vec3 home_location;
	std::string home_planet_name; // planet the player's home is located on
	std::wstring spouse_name; // name of character's spouse
	uint32_t available_lots;
	uint32_t faction_crc;
	uint32_t faction_status; // 00 = neutral, 01 = covert, 02 = overt

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(unknown1);
		buffer.write(unknown2);
		buffer.write(bind_location.x);
		buffer.write(bind_location.y);
		buffer.write(bind_location.z);
		buffer.write(bind_planet_name);
		buffer.write(bank_location.x);
		buffer.write(bank_location.y);
		buffer.write(bank_location.z);
		buffer.write(bank_planet_name);
		buffer.write(home_location.x);
		buffer.write(home_location.y);
		buffer.write(home_location.z);
		buffer.write(home_planet_name);
		buffer.write(spouse_name);
		buffer.write(available_lots);
		buffer.write(faction_crc);
		buffer.write(faction_status);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		unknown1 = buffer.read<uint32_t>();
		unknown2 = buffer.read<uint32_t>();
		bind_location.x = buffer.read<float>();
		bind_location.y = buffer.read<float>();
		bind_location.z = buffer.read<float>();
		bind_planet_name = buffer.read<std::string>();
		bank_location.x = buffer.read<float>();
		bank_location.y = buffer.read<float>();
		bank_location.z = buffer.read<float>();
		bank_planet_name = buffer.read<std::string>();
		home_location.x = buffer.read<float>();
		home_location.y = buffer.read<float>();
		home_location.z = buffer.read<float>();
		home_planet_name = buffer.read<std::string>();
		spouse_name = buffer.read<std::wstring>();
		available_lots = buffer.read<uint32_t>();
		faction_crc = buffer.read<uint32_t>();
		faction_status = buffer.read<uint32_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHARACTER_SHEET_RESPONSE_MESSAGE_H_
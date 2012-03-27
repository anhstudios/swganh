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

#ifndef SWGANH_MESSAGES_ENUMERATE_CHARACTER_ID_H_
#define SWGANH_MESSAGES_ENUMERATE_CHARACTER_ID_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"
#include "swganh/character/character_data.h"

namespace swganh {
namespace messages {

struct EnumerateCharacterId : public swganh::messages::BaseSwgMessage<EnumerateCharacterId> {
	static uint16_t opcount() { return 2; }
	static uint32_t opcode() { return 0x65EA4574; }

	std::vector<swganh::character::CharacterData> characters;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(characters.size());
		std::for_each(characters.begin(), characters.end(), [&buffer] (const swganh::character::CharacterData& character) {
			buffer.write(character.name);
			buffer.write(character.race_crc);
			buffer.write(character.character_id);
			buffer.write(character.galaxy_id);
			buffer.write(character.status);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		uint32_t character_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < character_count; i++) {
			swganh::character::CharacterData character;
			character.name = buffer.read<std::wstring>();
			character.race_crc = buffer.read<int32_t>();
			character.character_id = buffer.read<uint64_t>();
			character.galaxy_id = buffer.read<uint32_t>();
			character.status = buffer.read<int32_t>();
			characters.push_back(character);
		}
	}
};

EnumerateCharacterId BuildEnumerateCharacterId(std::vector<swganh::character::CharacterData> characters);

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ENUMERATE_CHARACTER_ID_H_
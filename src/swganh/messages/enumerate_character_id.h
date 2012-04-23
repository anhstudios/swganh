// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_ENUMERATE_CHARACTER_ID_H_
#define SWGANH_MESSAGES_ENUMERATE_CHARACTER_ID_H_

#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"
#include "swganh/character/character_data.h"

namespace swganh {
namespace messages {

    struct EnumerateCharacterId : public BaseSwgMessage<EnumerateCharacterId>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x65EA4574; }

    	std::vector<swganh::character::CharacterData> characters;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write<uint32_t>(characters.size());
    		std::for_each(
    		    characters.begin(),
    		    characters.end(),
    		    [&buffer] (const swganh::character::CharacterData& character)
    		{
    			buffer.write(character.name);
    			buffer.write(character.race_crc);
    			buffer.write(character.character_id);
    			buffer.write(character.galaxy_id);
    			buffer.write(character.status);
    		});
    	}

    	void OnDeserialize(anh::ByteBuffer buffer) {
    		uint32_t character_count = buffer.read<uint32_t>();
    		for (uint32_t i = 0; i < character_count; i++)
    		{
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

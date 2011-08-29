/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SWGANH_LOGIN_MESSAGES_ENUMERATE_CHARACTER_ID_H_
#define SWGANH_LOGIN_MESSAGES_ENUMERATE_CHARACTER_ID_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>

#include "anh/byte_buffer.h"

#include "swganh/base/swg_message.h"

#include "swganh/character/character_data.h"

namespace swganh {
namespace login {
namespace messages {

struct EnumerateCharacterId : public swganh::base::SwgMessage<EnumerateCharacterId> {
    static uint16_t opcount() { return 2; }
    static uint32_t opcode() { return 0x65EA4574; }

    std::vector<swganh::character::CharacterData> characters;

    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint32_t>(characters.size());
        std::for_each(characters.begin(), characters.end(), [&buffer] (const swganh::character::CharacterData& character){
            buffer.write<std::wstring>(character.name);
            buffer.write<uint32_t>(character.race_crc);
            buffer.write<uint64_t>(character.character_id);
            buffer.write<uint32_t>(character.galaxy_id);
            buffer.write<uint32_t>(character.status);
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

}}}  // namespace swganh::login::messages

#endif  // SWGANH_LOGIN_MESSAGES_ENUMERATE_CHARACTER_ID_H_

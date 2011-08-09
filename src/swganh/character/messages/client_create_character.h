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

#ifndef SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_H_
#define SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace character {
namespace messages {
    
struct ClientCreateCharacter : public swganh::base::SwgMessage<ClientCreateCharacter> {
    static uint16_t opcount() { return 0x0C; }
    static uint32_t opcode() { return 0xB97F3074; }
    
    std::string character_customization;
    std::wstring character_name;
    std::string player_race_iff;
    std::string start_location;
    std::string hair_object;
    std::string hair_customization;
    std::string starting_profession;
    uint8_t ukn1;
    float height;
    std::string biography;
    uint8_t tutorial_flag;


    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(character_customization);
        buffer.write(character_name);
        buffer.write(player_race_iff);
        buffer.write(start_location);
        buffer.write(hair_object);
        buffer.write(hair_customization);
        buffer.write(starting_profession);
        buffer.write<uint8_t>(0);
        buffer.write(height);
        std::wstring temp(biography.begin(), biography.end());
        buffer.write(biography);
        buffer.write(tutorial_flag);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        character_customization = buffer.read<std::string>();
        character_name = buffer.read<std::wstring>();
        player_race_iff = buffer.read<std::string>();
        start_location = buffer.read<std::string>();
        hair_object = buffer.read<std::string>();
        hair_customization = buffer.read<std::string>();
        starting_profession = buffer.read<std::string>();
        ukn1 = buffer.read<uint8_t>();
        height = buffer.read<float>();
        std::wstring temp(buffer.read<std::wstring>());
        biography =  std::string(temp.begin(), temp.end());
        tutorial_flag = buffer.read<uint8_t>();
    }
};

}}}  // namespace swganh::character::messages

#endif  // SWGANH_CHARACTER_MESSAGES_CLIENT_CREATE_CHARACTER_H_

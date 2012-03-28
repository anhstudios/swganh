// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CLIENT_CREATE_CHARACTER_H_
#define SWGANH_MESSAGES_CLIENT_CREATE_CHARACTER_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct ClientCreateCharacter : public BaseSwgMessage<ClientCreateCharacter>
    {
        static uint16_t Opcount() { return 0x0C; }
        static uint32_t Opcode() { return 0xB97F3074; }
        
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


        void OnSerialize(anh::ByteBuffer& buffer) const
        {
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

        void OnDeserialize(anh::ByteBuffer buffer)
        {
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

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_CLIENT_CREATE_CHARACTER_H_

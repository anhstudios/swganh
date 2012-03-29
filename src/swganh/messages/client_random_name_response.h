// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CLIENT_RANDOM_NAME_RESPONSE_H_
#define SWGANH_MESSAGES_CLIENT_RANDOM_NAME_RESPONSE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct ClientRandomNameResponse : public BaseSwgMessage<ClientRandomNameResponse>
    {
        static uint16_t Opcount() { return 4; }
        static uint32_t Opcode() { return 0xE85FB868; }
        
        std::string player_race_iff;
        std::wstring random_name;
        std::string stf_file;
        std::string approval_string;

        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(player_race_iff);	
            buffer.write(random_name);
            buffer.write(stf_file);
            buffer.write(0);
            buffer.write(approval_string);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
        	player_race_iff = buffer.read<std::string>();
            random_name = buffer.read<std::wstring>();
            stf_file = buffer.read<std::string>();
            buffer.read<int>(); // spacer, unused
            approval_string = buffer.read<std::string>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_CLIENT_RANDOM_NAME_RESPONSE_H_

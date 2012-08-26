// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct ClientRandomNameRequest : public BaseSwgMessage<ClientRandomNameRequest>
    {
        static uint16_t Opcount() { return 2; }
        static uint32_t Opcode() { return 0xD6D1B6D1; }
        
        std::string player_race_iff;

        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(player_race_iff);	
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
        	player_race_iff = buffer.read<std::string>();
        }
    };

}}  // namespace swganh::messages

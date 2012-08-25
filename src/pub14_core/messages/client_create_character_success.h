// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct ClientCreateCharacterSuccess : public BaseSwgMessage<ClientCreateCharacterSuccess>
    {
        static uint16_t Opcount() { return 2; }
        static uint32_t Opcode() { return 0x1DB575CC; }
        
        uint64_t character_id;

        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(character_id);	
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
        	character_id = buffer.read<uint64_t>();
        }
    };

}}  // namespace swganh::messages

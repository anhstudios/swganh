// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_DELETE_CHARACTER_MESSAGE_H_
#define SWGANH_MESSAGES_DELETE_CHARACTER_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct DeleteCharacterMessage : public BaseSwgMessage<DeleteCharacterMessage>
    {
        static uint16_t Opcount() { return 3; }
        static uint32_t Opcode() { return 0xE87AD031; }

        int32_t server_id;
        uint64_t character_id;
        
        void OnDeserialize(anh::ByteBuffer buffer)
        {
            server_id = buffer.read<int32_t>();
            character_id = buffer.read<uint64_t>();
        }
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(server_id);
            buffer.write(character_id);
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_DELETE_CHARACTER_MESSAGE_H_

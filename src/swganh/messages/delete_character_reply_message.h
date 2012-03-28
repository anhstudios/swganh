// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_DELETE_CHARACTER_REPLY_MESSAGE_H_
#define SWGANH_MESSAGES_DELETE_CHARACTER_REPLY_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct DeleteCharacterReplyMessage : public BaseSwgMessage<DeleteCharacterReplyMessage>
    {
        static uint16_t Opcount() { return 2; }
        static uint32_t Opcode() { return 0x8268989B; }
    
        int32_t failure_flag;
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write<int32_t>(failure_flag);
        }
    
        void OnDeserialize(anh::ByteBuffer buffer)
        {
            failure_flag = buffer.read<int32_t>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_DELETE_CHARACTER_REPLY_MESSAGE_H_

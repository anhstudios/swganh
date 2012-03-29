// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CLIENT_ID_MSG_H_
#define SWGANH_MESSAGES_CLIENT_ID_MSG_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct ClientIdMsg : public BaseSwgMessage<ClientIdMsg>
    {
        static uint16_t Opcount() { return 3; }
        static uint32_t Opcode() { return 0xD5899226; }
        
        std::string session_hash;
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write<uint32_t>(0);
            buffer.write<uint32_t>(0);
            buffer.write(session_hash);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
            buffer.read<uint32_t>();  // unknown
            buffer.read<uint32_t>();  // size @TODO Investigate if this is proper usage.
            session_hash = buffer.read<std::string>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_CLIENT_ID_MSG_H_

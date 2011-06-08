#ifndef ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
#define ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H

#include <cstdint>
#include "anh/byte_buffer.h
#include "packets/base_swg_packet.h"

// Originates on Server
// http://wiki.swganh.org/index.php/DeleteCharacterReplyMessage
namespace packets {
struct DeleteCharacterReplyMessage : public BaseSwgPacket<DeleteCharacterReplyMessage> {
    static const uint16_t opcount = 2;
    static const uint32_t opcode = 0x8268989B;

    int32_t failure_flag;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write<int32_t>(failure_flag);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        failure_flag = buffer.read<int32_t>();
    }
};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
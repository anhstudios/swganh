#ifndef ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
#define ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H

#include <anh/byte_buffer.h>

// Originates on Server
// http://wiki.swganh.org/index.php/DeleteCharacterReplyMessage
namespace packets {
struct DeleteCharacterReplyMessage
{
    DeleteCharacterReplyMessage(int32_t failure_flag_ = 0)
        : failure_flag(failure_flag_) {}
    explicit DeleteCharacterReplyMessage(anh::ByteBuffer buffer)
    {
        deserialize(buffer);
    }
    virtual ~DeleteCharacterReplyMessage() {}
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<int32_t>(failure_flag);
    }

    void deserialize(anh::ByteBuffer buffer) {
        failure_flag = buffer.read<int32_t>();
    }

    int32_t failure_flag;

    static uint16_t operand_count() { return 2; }
    static uint32_t crc() { return 0x8268989B; }
};
} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
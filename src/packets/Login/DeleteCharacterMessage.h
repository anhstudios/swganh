#ifndef ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H
#define ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H

#include <anh/byte_buffer.h>

// Originates on Client
// http://wiki.swganh.org/index.php/DeleteCharacterMessage
namespace packets {
struct DeleteCharacterMessage
{
    DeleteCharacterMessage(int32_t server_id_ = 0, uint64_t character_id_ = 0)
    : server_id(server_id_)
    , character_id(character_id_) {}
    explicit DeleteCharacterMessage(anh::ByteBuffer buffer) {
        deserialize(buffer);
    }
    virtual ~DeleteCharacterMessage() {}

    int32_t server_id;
    uint64_t character_id;

    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<int32_t>(server_id);
        buffer.write<uint64_t>(character_id);
    }
    void deserialize(anh::ByteBuffer buffer) {
        server_id = buffer.read<int32_t>();
        character_id = buffer.read<uint64_t>();
    }

    static uint16_t operand_count() { return 3; }
    static uint32_t crc() { return 0xE87AD031; }

};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H
#ifndef ANH_PACKETS_SELECT_CHARACTER_H
#define ANH_PACKETS_SELECT_CHARACTER_H

#include <anh/byte_buffer.h>

// Originates on Client [ C -> S ]
namespace packets {
struct SelectCharacter
{
    explicit SelectCharacter(anh::ByteBuffer buffer)
    {
        deserialize(buffer);
    }
    SelectCharacter(uint64_t character_id_ = 0)
        : character_id(character_id_) {}

    uint64_t character_id;
    virtual ~SelectCharacter() {}
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<uint64_t>(character_id);
    }
    void deserialize(anh::ByteBuffer buffer) {
        character_id = buffer.read<uint64_t>();
    }
    static uint16_t operand_count() { return 2; }
    static uint32_t crc() { return 0xB5098D76; }
};

} // packets

#endif // ANH_PACKETS_SELECT_CHARACTER_H
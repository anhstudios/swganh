#ifndef ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H
#define ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client
// http://wiki.swganh.org/index.php/DeleteCharacterMessage
namespace packets {
struct DeleteCharacterMessage : public BasePacket {
    static const uint16_t opcount = 2;
    static const uint32_t opcode = 0x8268989B;

    int32_t server_id;
    uint64_t character_id;
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(server_id);
        buffer.write(character_id);
    }

    void onDeserialize(anh::ByteBuffer& buffer) {
        server_id = buffer.read<int32_t>();
        character_id = buffer.read<uint64_t>();
    }
};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H
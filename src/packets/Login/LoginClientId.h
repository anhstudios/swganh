#ifndef ANH_PACKETS_LOGINCLIENTID_H
#define ANH_PACKETS_LOGINCLIENTID_H

#include "packets/base_swg_packet.h"

// Originates on Client
namespace packets {

struct LoginClientId : public BaseSwgPacket<LoginClientId> {
    static const uint16_t opcount = 4;
    static const uint32_t opcode = 0x41131F96;

    std::string username;
    std::string password;
    std::string client_version;

    void onDeserialize(anh::ByteBuffer buffer) {
        username = buffer.read<std::string>();
        password = buffer.read<std::string>();
        client_version = buffer.read<std::string>();
    }
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(username);
        buffer.write(password);
        buffer.write(client_version);
    }
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTID_H
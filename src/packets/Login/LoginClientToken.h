#ifndef ANH_PACKETS_LOGINCLIENTTOKEN_H
#define ANH_PACKETS_LOGINCLIENTTOKEN_H

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "packets/base_swg_packet.h"

// Originates on Client
namespace packets {
struct LoginClientToken : public BaseSwgPacket<LoginClientToken> {
    static const uint16_t opcount = 5;
    static const uint32_t opcode = 0xAAB296C6;

    anh::ByteBuffer session_key;
    int32_t account_id;
    int32_t station_id;
    std::string station_username;
    
	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write<uint32_t>(session_key.size());
		buffer.append(session_key);

		buffer.write(account_id);
		buffer.write(station_id);
		buffer.write(station_username);
	}

    void onDeserialize(anh::ByteBuffer buffer) {
		session_key = anh::ByteBuffer(buffer.data(), buffer.read<uint32_t>());
		buffer.read_position(buffer.read_position() + session_key.size());

        account_id = buffer.read<int32_t>();
        station_id = buffer.read<int32_t>();
        station_username = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTTOKEN_H
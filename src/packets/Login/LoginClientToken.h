#ifndef ANH_PACKETS_LOGINCLIENTTOKEN_H
#define ANH_PACKETS_LOGINCLIENTTOKEN_H

#include <cstdint>
#include <string>

#include "anh/byte_buffer.h"

#include "packets/BasePacket.h"

// Originates on Client
namespace packets {
struct LoginClientToken : public BasePacket
{
    explicit LoginClientToken(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, NONE){}
    
    virtual ~LoginClientToken() {}

	anh::ByteBuffer session_key;
    int32_t account_id;
    int32_t station_id;
    std::string station_username;
    
	void serialize(anh::ByteBuffer& buffer) const {
		buffer.write<uint16_t>(4);
		buffer.write<uint32_t>(0xAAB296C6);
		buffer.write<uint32_t>(session_key.size());
		buffer.append(session_key);

		buffer.write<uint32_t>(account_id);
		buffer.write<uint32_t>(station_id);
		buffer.write<std::string>(station_username);
	}

    void deserialize(anh::ByteBuffer buffer) {
		session_key = anh::ByteBuffer(buffer.data(), buffer.read<uint32_t>());
		buffer.read_position(buffer.read_position() + session_key.size());

        account_id = buffer.read<int32_t>();
        station_id = buffer.read<int32_t>();
        station_username = buffer.read<std::string>();
    }
};

typedef anh::event_dispatcher::BasicEvent<LoginClientToken> LoginClientTokenEvent;

} // packets

#endif // ANH_PACKETS_LOGINCLIENTTOKEN_H
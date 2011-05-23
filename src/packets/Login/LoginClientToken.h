#ifndef ANH_PACKETS_LOGINCLIENTTOKEN_H
#define ANH_PACKETS_LOGINCLIENTTOKEN_H

#include <packets/BasePacket.h>

// Originates on Client
namespace packets {
struct LoginClientToken : public BasePacket
{
    explicit LoginClientToken(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, NONE){}
	anh::ByteBuffer session_key;
    int32_t account_id;
    int32_t station_id;
    std::string station_username;
};

class LoginClientTokenEvent : public anh::event_dispatcher::BasicEvent<LoginClientToken>{
public:
    LoginClientTokenEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LoginClientToken>("LoginClientToken")
	{ 
		account_id = 0xDEADBABE;
		station_id = 0xDDDDBBBB;
		station_username = "username";
		session_key.write<uint64_t>(0xDEADBABEDEADBABE); 
	}

    virtual ~LoginClientTokenEvent() {}

	void serialize(anh::ByteBuffer& buffer) {
		buffer.write<uint16_t>(4);
		buffer.write<uint32_t>(0xAAB296C6);
		buffer.write<uint32_t>(session_key.size());
		buffer.append(session_key);

		buffer.write<uint32_t>(account_id);
		buffer.write<uint32_t>(station_id);
		buffer.write<std::string>(station_username);
	}

    void deserialize(anh::ByteBuffer& buffer) {
		session_key = anh::ByteBuffer(buffer.data(), buffer.read<uint32_t>());
		buffer.read_position(buffer.read_position() + session_key.size());

        account_id = buffer.read<int32_t>();
        station_id = buffer.read<int32_t>();
        station_username = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTTOKEN_H
#ifndef ANH_PACKETS_LOGINCLIENTTOKEN_H
#define ANH_PACKETS_LOGINCLIENTTOKEN_H

#include <anh/byte_buffer.h>

// Originates on Client
namespace packets {
struct LoginClientToken
{
    explicit LoginClientToken(anh::ByteBuffer buffer)
    {
        deserialize(buffer);
    }
    LoginClientToken(anh::ByteBuffer session_key_ = anh::ByteBuffer(), int32_t account_id_ = 0
        , std::string account_username_ = std::string(""))
        : session_key(session_key_)
        , account_id(account_id_)
        , account_username(account_username_) 
    {
    }
    virtual ~LoginClientToken() {}

	anh::ByteBuffer session_key;
    int32_t account_id;
    std::string account_username;

    void serialize(anh::ByteBuffer& buffer) const {
		buffer.write<uint16_t>(operand_count());
		buffer.write<uint32_t>(crc());
		buffer.write<uint32_t>(session_key.size());
		buffer.append(session_key);

		buffer.write<uint32_t>(account_id);
		buffer.write<std::string>(account_username);
	}

    void deserialize(anh::ByteBuffer buffer) {
		session_key = anh::ByteBuffer(buffer.data(), buffer.read<uint32_t>());
		buffer.read_position(buffer.read_position() + session_key.size());

        account_id = buffer.read<int32_t>();
        account_username = buffer.read<std::string>();
    }
    static uint16_t operand_count() { return 4; }
    static uint32_t crc() { return 0xAAB296C6; }
    
};
} // packets

#endif // ANH_PACKETS_LOGINCLIENTTOKEN_H
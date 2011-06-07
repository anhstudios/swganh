#ifndef ANH_PACKETS_LOGINCLIENTID_H
#define ANH_PACKETS_LOGINCLIENTID_H

#include <anh/byte_buffer.h>

// Originates on Client
namespace packets {
struct LoginClientId
{
    explicit LoginClientId(anh::ByteBuffer buffer)
    {
        deserialize(buffer);
    }
    LoginClientId(std::string username_ = std::string(""), std::string password_ = std::string("")
        , std::string client_version_ = std::string(""))
        : username(username_)
        , password(password_)
        , client_version(client_version_) 
    {}
    virtual ~LoginClientId() {}

    std::string username;
    std::string password;
    std::string client_version;

    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<std::string>(username);
        buffer.write<std::string>(password);
        buffer.write<std::string>(client_version);
    }
    void deserialize(anh::ByteBuffer buffer) {
        username = buffer.read<std::string>();
        password = buffer.read<std::string>();
        client_version = buffer.read<std::string>();
    }
    static uint16_t operand_count() { return 4; }
    static uint32_t crc() { return 0x41131F96; }
        
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTID_H
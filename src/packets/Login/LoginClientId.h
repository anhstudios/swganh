#ifndef ANH_PACKETS_LOGINCLIENTID_H
#define ANH_PACKETS_LOGINCLIENTID_H

#include <packets/BasePacket.h>

// Originates on Client
namespace packets {
struct LoginClientId : public BasePacket
{
    explicit LoginClientId(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, NONE){}
    std::string username;
    std::string password;
    std::string client_version;
};

class LoginClientIdEvent : public anh::event_dispatcher::BasicEvent<LoginClientId>{
public:    
    LoginClientIdEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LoginClientId>("LoginClientId"){}
    virtual ~LoginClientIdEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        uint8_t size = buffer.read<uint8_t>();
        uint32_t pack = buffer.read<uint32_t>();
        // TEMP TODO
        username = buffer.read<std::string>();
        password = buffer.read<std::string>();
        client_version = buffer.read<std::string>();
    }
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint8_t>(3);
        //TODO REPLACE!!!!!!!!!!
        buffer.write<uint32_t>(0x41131F96);
        buffer.write<std::string>(username);
        buffer.write<std::string>(password);
        buffer.write<std::string>(client_version);
    }
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTID_H
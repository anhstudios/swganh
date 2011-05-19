#ifndef ANH_PACKETS_LOGINCLIENTID_H
#define ANH_PACKETS_LOGINCLIENTID_H

#include <packets/BasePacket.h>

// Originates on Client
namespace packets {
struct LoginClientId : public BasePacket
{
    explicit LoginClientId(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, NONE){}
    std::string username;
    std::string password;
    std::string client_version;
};

class LoginClientIdEvent : public anh::event_dispatcher::BasicEvent<LoginClientId>{
public:    
    LoginClientIdEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LoginClientId>("LoginClientId"){}
    virtual ~LoginClientIdEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        username = buffer.read<std::string>();
        password = buffer.read<std::string>();
        client_version = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTID_H
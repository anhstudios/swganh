#ifndef ANH_PACKETS_LOGINCLIENTTOKEN_H
#define ANH_PACKETS_LOGINCLIENTTOKEN_H

#include <packets/BasePacket.h>

// Originates on Client
namespace packets {
struct LoginClientToken : public BasePacket
{
    explicit LoginClientToken(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, NONE){}
    int32_t session_key;
    int32_t account_id;
    int32_t station_id;
    std::string station_username;
};

class LoginClientTokenEvent : public anh::event_dispatcher::BasicEvent<LoginClientToken>{
public:    
    LoginClientTokenEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LoginClientToken>("LoginClientToken"){}
    virtual ~LoginClientTokenEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        session_key = buffer.read<int32_t>();
        account_id = buffer.read<int32_t>();
        station_id = buffer.read<int32_t>();
        station_username = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_LOGINCLIENTTOKEN_H
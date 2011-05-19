#ifndef ANH_PACKETS_CLIENT_INACTIVITY_MESSAGE_H
#define ANH_PACKETS_CLIENT_INACTIVITY_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on server
namespace packets {
struct ClientInactivityMessage : public BasePacket
{
    ClientInactivityMessage(std::shared_ptr<network::Session> session_  = nullptr, uint8_t afk_flag_ = 0)
        : BasePacket(session_, CLIENT)
        , afk_flag(afk_flag_)
    {}
    uint8_t afk_flag;
};

class ClientInactivityMessageEvent : public anh::event_dispatcher::BasicEvent<ClientInactivityMessage>{
public:    
    ClientInactivityMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint8_t afk_flag_ = 0) 
        : anh::event_dispatcher::BasicEvent<ClientInactivityMessage>("ClientInactivityMessage"){}
    virtual ~ClientInactivityMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint8_t>(afk_flag);
    }
};

} // packets

#endif // ANH_PACKETS_CLIENT_INACTIVITY_MESSAGE_H
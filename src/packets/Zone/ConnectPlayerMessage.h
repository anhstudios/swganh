#ifndef ANH_PACKETS_CONNECT_PLAYER_MESSAGE_H
#define ANH_PACKETS_CONNECT_PLAYER_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S]
namespace packets {
struct ConnectPlayerMessage : public BasePacket
{
    explicit ConnectPlayerMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}
    uint32_t always_zero;
};

class ConnectPlayerMessageEvent : public anh::event_dispatcher::BasicEvent<ConnectPlayerMessage>{
public:    
    ConnectPlayerMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<ConnectPlayerMessage>("ConnectPlayerMessage"){}
    virtual ~ConnectPlayerMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        always_zero = buffer.read<uint32_t>();
    }
};

} // packets

#endif // ANH_PACKETS_CONNECT_PLAYER_MESSAGE_H
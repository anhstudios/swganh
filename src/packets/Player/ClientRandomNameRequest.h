#ifndef ANH_PACKETS_CLIENT_RANDOM_NAME_REQUEST_H
#define ANH_PACKETS_CLIENT_RANDOM_NAME_REQUEST_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S]
namespace packets {
struct ClientRandomNameRequest : public BasePacket
{
    explicit ClientRandomNameRequest(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}

    std::string player_race_iff;
};

class ClientRandomNameRequestEvent : public anh::event_dispatcher::BasicEvent<ClientRandomNameRequest>{
public:    
    ClientRandomNameRequestEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<ClientRandomNameRequest>("ClientRandomNameRequest"){}
    virtual ~ClientRandomNameRequestEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        player_race_iff = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_CLIENT_RANDOM_NAME_REQUEST_H
#ifndef ANH_PACKETS_LAG_REQUEST_H
#define ANH_PACKETS_LAG_REQUEST_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
namespace packets {
struct LagRequest : public BasePacket
{
    explicit LagRequest(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, ZONE){}
};

class LagRequestEvent : public anh::event_dispatcher::BasicEvent<LagRequest>{
public:    
    LagRequestEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LagRequest>("LagRequest"){}
    virtual ~LagRequestEvent() {}
};

} // packets

#endif // ANH_PACKETS_LAG_REQUEST_H
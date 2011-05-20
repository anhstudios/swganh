#ifndef ANH_PACKETS_DENY_TRADE_MESSAGE_H
#define ANH_PACKETS_DENY_TRADE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct DenyTradeMessage : public BasePacket
{
    DenyTradeMessage(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT)
        {}
};

class DenyTradeMessageEvent : public anh::event_dispatcher::BasicEvent<DenyTradeMessage>{
public:    
    DenyTradeMessageEvent(std::shared_ptr<network::Session> session_  = nullptr)
        : anh::event_dispatcher::BasicEvent<DenyTradeMessage>("DenyTradeMessage"){}
    virtual ~DenyTradeMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_DENY_TRADE_MESSAGE_H
#ifndef ANH_PACKETS_VERIFY_TRADE_MESSAGE_H
#define ANH_PACKETS_VERIFY_TRADE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct VerifyTradeMessage  : public BasePacket
{
    VerifyTradeMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr
        : BasePacket(session_, CLIENT)
        {}
};

class VerifyTradeMessageEvent : public anh::event_dispatcher::BasicEvent<VerifyTradeMessage>{
public:    
    VerifyTradeMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr
        : anh::event_dispatcher::BasicEvent<VerifyTradeMessage>("VerifyTradeMessage"){}
    virtual ~VerifyTradeMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_VERIFY_TRADE_MESSAGE_H
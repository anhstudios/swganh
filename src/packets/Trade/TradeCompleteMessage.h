#ifndef ANH_PACKETS_TRADE_COMPLETE_MESSAGE_H
#define ANH_PACKETS_TRADE_COMPLETE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct TradeCompleteMessage  : public BasePacket
{
    TradeCompleteMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT)
        {}
};

class TradeCompleteMessageEvent : public anh::event_dispatcher::BasicEvent<TradeCompleteMessage>{
public:    
    TradeCompleteMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<TradeCompleteMessage>("TradeCompleteMessage"){}
    virtual ~TradeCompleteMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_TRADE_COMPLETE_MESSAGE_H
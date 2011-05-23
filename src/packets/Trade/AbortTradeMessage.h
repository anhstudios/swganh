#ifndef ANH_ABORT_TRADE_MESSAGE_H
#define ANH_ABORT_TRADE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/AbortTradeMessage
namespace packets {
struct AbortTradeMessage : public BasePacket
{
    explicit AbortTradeMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT){}
};

class AbortTradeMessageEvent : public anh::event_dispatcher::BasicEvent<AbortTradeMessage>{
public:    
    AbortTradeMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<AbortTradeMessage>("AbortTradeMessage"){}
    virtual ~AbortTradeMessageEvent() {}
};

} // packets

#endif // ANH_ABORT_TRADE_MESSAGE_H
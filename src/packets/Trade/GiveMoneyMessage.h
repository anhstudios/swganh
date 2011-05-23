#ifndef ANH_PACKETS_GIVE_MONEY_MESSAGE_H
#define ANH_PACKETS_GIVE_MONEY_MESSAGE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
namespace packets {
struct GiveMoneyMessage  : public BasePacket
{
    GiveMoneyMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t money_ = 0)
        : BasePacket(session_, CLIENT)
        , money(money_)
        {}
    uint32_t money;
};

class GiveMoneyMessageEvent : public anh::event_dispatcher::BasicEvent<GiveMoneyMessage>{
public:    
    GiveMoneyMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t money_ = 0) 
        : anh::event_dispatcher::BasicEvent<GiveMoneyMessage>("GiveMoneyMessage"){}
    virtual ~GiveMoneyMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(money);
    }
};

} // packets

#endif // ANH_PACKETS_GIVE_MONEY_MESSAGE_H
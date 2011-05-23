#ifndef ANH_BEGIN_TRADE_MESSAGE_H
#define ANH_BEGIN_TRADE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/BeginTradeMessage
namespace packets {
struct BeginTradeMessage : public BasePacket
{
    BeginTradeMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_) {}
    uint64_t object_id;    
};

class BeginTradeMessageEvent : public anh::event_dispatcher::BasicEvent<BeginTradeMessage>{
public:    
    BeginTradeMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<BeginTradeMessage>("BeginTradeMessage"){}
    virtual ~BeginTradeMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer)
    {
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_BEGIN_TRADE_MESSAGE_H
#ifndef ANH_PACKETS_GUILD_REQEUEST_MESSAGE_H
#define ANH_PACKETS_GUILD_REQEUEST_MESSAGE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
namespace packets {
struct GuildRequestMessage  : public BasePacket
{
    GuildRequestMessage ()
        : BasePacket() {}
    GuildRequestMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        {}
    uint64_t object_id;
};

class GuildRequestMessageEvent : public anh::event_dispatcher::BasicEvent<GuildRequestMessage>{
public:    
    GuildRequestMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<GuildRequestMessage>("GuildRequestMessage"){}
    virtual ~GuildRequestMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_GUILD_REQEUEST_MESSAGE_H
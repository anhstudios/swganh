#ifndef ANH_PACKETS_SERVER_TIME_MESSAGE_H
#define ANH_PACKETS_SERVER_TIME_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct ServerTimeMessage  : public BasePacket
{
    ServerTimeMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t galactic_time_ = 0)
        : BasePacket(session_, CLIENT)
        , galactic_time(galactic_time_)
        {}
    uint64_t galactic_time;
};

class ServerTimeMessageEvent : public anh::event_dispatcher::BasicEvent<ServerTimeMessage>{
public:    
    ServerTimeMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t galactic_time_ = 0) 
        : anh::event_dispatcher::BasicEvent<ServerTimeMessage>("ServerTimeMessage"){}
    virtual ~ServerTimeMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(galactic_time);
    }
};

} // packets

#endif // ANH_PACKETS_SERVER_TIME_MESSAGE_H
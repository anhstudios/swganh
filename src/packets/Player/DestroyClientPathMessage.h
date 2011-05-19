#ifndef ANH_PACKETS_DESTROYCLIENT_PATH_MESSAGE_H
#define ANH_PACKETS_DESTROYCLIENT_PATH_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct DestroyClientPathMessage  : public BasePacket
{
    DestroyClientPathMessage (std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT)
        {}
};

class DestroyClientPathMessageEvent : public anh::event_dispatcher::BasicEvent<DestroyClientPathMessage >{
public:    
    DestroyClientPathMessageEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<DestroyClientPathMessage >("DestroyClientPathMessage"){}
    virtual ~DestroyClientPathMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_DESTROYCLIENT_PATH_MESSAGE_H
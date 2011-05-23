#ifndef ANH_PACKETS_LOGOUT_MESSAGE_H
#define ANH_PACKETS_LOGOUT_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct LogoutMessage  : public BasePacket
{
    LogoutMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT)
        {}
};

class LogoutMessageEvent : public anh::event_dispatcher::BasicEvent<LogoutMessage >{
public:    
    LogoutMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<LogoutMessage >("LogoutMessage"){}
    virtual ~LogoutMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_LOGOUT_MESSAGE_H
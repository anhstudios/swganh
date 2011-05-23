#ifndef ANH_PACKETS_CLOSE_HOLOCRON_MESSAGE_H
#define ANH_PACKETS_CLOSE_HOLOCRON_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
namespace packets {
struct CloseHolocronMessage : public BasePacket
{
    explicit CloseHolocronMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT){}
};

class CloseHolocronMessageEvent : public anh::event_dispatcher::BasicEvent<CloseHolocronMessage>{
public:    
    CloseHolocronMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<CloseHolocronMessage>("CloseHolocronMessage"){}
    virtual ~CloseHolocronMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_CLOSE_HOLOCRON_MESSAGE_H
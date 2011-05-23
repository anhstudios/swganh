#ifndef ANH_BEGIN_VERIFICATION_MESSAGE_H
#define ANH_BEGIN_VERIFICATION_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/BeginVerificationMessage
namespace packets {
struct BeginVerificationMessage : public BasePacket
{
    explicit BeginVerificationMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT){}
};
class BeginVerificationMessageEvent : public anh::event_dispatcher::BasicEvent<BeginVerificationMessage>{
public:    
    BeginVerificationMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<BeginVerificationMessage>("BeginVerificationMessage"){}
    virtual ~BeginVerificationMessageEvent() {}
};

} // packets

#endif // ANH_BEGIN_VERIFICATION_MESSAGE_H
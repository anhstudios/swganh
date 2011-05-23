#ifndef ANH_ACCEPT_TRANSACTION_MESSAGE_H
#define ANH_ACCEPT_TRANSACTION_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/AcceptTransactionMessage
namespace packets {
struct AcceptTransactionMessage : public BasePacket
{
    explicit AcceptTransactionMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT){}
};
class AcceptTransactionMessageEvent : public anh::event_dispatcher::BasicEvent<AcceptTransactionMessage>{
public:    
    AcceptTransactionMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<AcceptTransactionMessage>("AcceptTransactionMessage"){}
    virtual ~AcceptTransactionMessageEvent() {}
};

} // packets

#endif // ANH_ACCEPT_TRANSACTION_MESSAGE_H
#ifndef ANH_PACKETS_UN_ACCEPT_TRANSACTION_MESSAGE_H
#define ANH_PACKETS_UN_ACCEPT_TRANSACTION_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UnAcceptTransactionMessage  : public BasePacket
{
    UnAcceptTransactionMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT)
        {}
};

class UnAcceptTransactionMessageEvent : public anh::event_dispatcher::BasicEvent<UnAcceptTransactionMessage>{
public:    
    UnAcceptTransactionMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<UnAcceptTransactionMessage>("UnAcceptTransactionMessage"){}
    virtual ~UnAcceptTransactionMessageEvent() {}
};

} // packets

#endif // ANH_PACKETS_UN_ACCEPT_TRANSACTION_MESSAGE_H
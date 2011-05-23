#ifndef ANH_CANCEL_TICKET_RESPONSE_MESSAGE_H
#define ANH_CANCEL_TICKET_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C]
// http://wiki.swganh.org/index.php/CancelTicketResponseMessage
namespace packets {
struct CancelTicketResponseMessage : public BasePacket
{
    CancelTicketResponseMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t message_ = 0
        , uint64_t ticket_number_ = 0)
        : BasePacket(session_, CLIENT)
        , message(message_)
        , ticket_number(ticket_number_)
        {}
    uint32_t message;
    uint64_t ticket_number;
};

class CancelTicketResponseMessageEvent : public anh::event_dispatcher::BasicEvent<CancelTicketResponseMessage>{
public:    
    CancelTicketResponseMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t message_ = 0
        , uint64_t ticket_number_ = 0) 
        : anh::event_dispatcher::BasicEvent<CancelTicketResponseMessage>("CancelTicketResponseMessage"){}
    virtual ~CancelTicketResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer)
    {
        buffer.write<uint32_t>(message);
        buffer.write<uint64_t>(ticket_number);
    }
};

} // packets

#endif // ANH_CANCEL_TICKET_RESPONSE_MESSAGE_H
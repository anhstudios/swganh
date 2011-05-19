#ifndef ANH_PACKETS_ENTER_TICKET_PURCHASE_MODE_MESSAGE_H
#define ANH_PACKETS_ENTER_TICKET_PURCHASE_MODE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct EnterTicketPurchaseModeMessage  : public BasePacket
{
    EnterTicketPurchaseModeMessage (std::shared_ptr<network::Session> session_  = nullptr, std::string& planet_ = std::string()
        , std::string& destination_ = std::string())
        : BasePacket(session_, CLIENT)
        , planet(planet_)
        , destination(destination_)
        {}
    std::string planet;
    std::string destination;
};

class EnterTicketPurchaseModeMessageEvent : public anh::event_dispatcher::BasicEvent<EnterTicketPurchaseModeMessage >{
public:    
    EnterTicketPurchaseModeMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, std::string& planet_ = std::string()
        , std::string& destination_ = std::string()) 
        : anh::event_dispatcher::BasicEvent<EnterTicketPurchaseModeMessage >("EnterTicketPurchaseModeMessage"){}
    virtual ~EnterTicketPurchaseModeMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(planet);
        buffer.write<std::string>(destination);
    }
};

} // packets

#endif // ANH_PACKETS_ENTER_TICKET_PURCHASE_MODE_MESSAGE_H
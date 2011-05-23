#ifndef ANH_PACKETS_RETRIEVE_AUCTION_ITEM_MESSAGE_H
#define ANH_PACKETS_RETRIEVE_AUCTION_ITEM_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct RetrieveAuctionItemMessage  : public BasePacket
{
    RetrieveAuctionItemMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t auction_id_ = 0
        , uint64_t terminal_id_ = 0)
        : BasePacket(session_, CLIENT)
        , auction_id(auction_id_)
        , terminal_id(terminal_id_)
        {}
    uint64_t auction_id;
    uint64_t terminal_id;
};

class RetrieveAuctionItemMessageEvent : public anh::event_dispatcher::BasicEvent<RetrieveAuctionItemMessage >{
public:    
    RetrieveAuctionItemMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t auction_id_ = 0
        , uint64_t terminal_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<RetrieveAuctionItemMessage >("RetrieveAuctionItemMessage"){}
    virtual ~RetrieveAuctionItemMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(auction_id);
        buffer.write<uint64_t>(terminal_id);
    }
};

} // packets

#endif // ANH_PACKETS_RETRIEVE_AUCTION_ITEM_MESSAGE_H
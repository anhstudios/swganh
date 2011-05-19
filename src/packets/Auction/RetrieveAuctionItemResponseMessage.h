#ifndef ANH_PACKETS_RETRIEVE_AUCTION_ITEM_RESPONSE_MESSAGE_H
#define ANH_PACKETS_RETRIEVE_AUCTION_ITEM_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct RetrieveAuctionItemResponseMessage  : public BasePacket
{
    RetrieveAuctionItemResponseMessage (std::shared_ptr<network::Session> session_  = nullptr, uint64_t auction_id_ = 0
        , uint32_t error_id_ = 0)
        : BasePacket(session_, CLIENT)
        , auction_id(auction_id_)
        , error_id(error_id_)
        {}
    uint64_t auction_id;
    uint32_t error_id;
};

class RetrieveAuctionItemResponseMessageEvent : public anh::event_dispatcher::BasicEvent<RetrieveAuctionItemResponseMessage >{
public:    
    RetrieveAuctionItemResponseMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t auction_id_ = 0
        , uint32_t error_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<RetrieveAuctionItemResponseMessage >("RetrieveAuctionItemResponseMessage"){}
    virtual ~RetrieveAuctionItemResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(auction_id);
        buffer.write<uint32_t>(error_id);
    }
};

} // packets

#endif // ANH_PACKETS_RETRIEVE_AUCTION_ITEM_RESPONSE_MESSAGE_H
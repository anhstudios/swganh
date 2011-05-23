#ifndef ANH_BID_AUCTION_RESPONSE_MESSAGE_H
#define ANH_BID_AUCTION_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C]
//http://wiki.swganh.org/index.php/BidAuctionResponseMessage
namespace packets {
struct BidAuctionResponseMessage : public BasePacket
{
    BidAuctionResponseMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t auction_id_ = 0, uint32_t error_id_ = 0)
        : BasePacket(session_, CLIENT)
        , auction_id(auction_id_)
        , error_id(error_id_)
        {}
    uint64_t auction_id;    
    uint32_t error_id;
};

class BidAuctionResponseMessageEvent : public anh::event_dispatcher::BasicEvent<BidAuctionResponseMessage>{
public:    
    BidAuctionResponseMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t auction_id_ = 0, uint32_t error_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<BidAuctionResponseMessage>("BidAuctionResponseMessage"){}
    virtual ~BidAuctionResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(auction_id);
        buffer.write<uint32_t>(error_id);
    }
};

} // packets

#endif // ANH_BID_AUCTION_RESPONSE_MESSAGE_H
#ifndef ANH_BID_AUCTION_MESSAGE_H
#define ANH_BID_AUCTION_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S]
//http://wiki.swganh.org/index.php/BidAuctionMessage
namespace packets {
struct BidAuctionMessage : public BasePacket
{
    explicit BidAuctionMessage(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}

    uint64_t auction_id;    
    uint32_t my_bid;
    uint32_t price_proxy;
};

class BidAuctionMessageEvent : public anh::event_dispatcher::BasicEvent<BidAuctionMessage>{
public:    
    BidAuctionMessageEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<BidAuctionMessage>("BidAuctionMessage"){}
    virtual ~BidAuctionMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer)
    {
        auction_id = buffer.read<uint64_t>();
        my_bid = buffer.read<uint32_t>();
        price_proxy = buffer.read<uint32_t>();
    }
};

} // packets

#endif // ANH_BID_AUCTION_MESSAGE_H
#ifndef ANH_CANCEL_LIVE_AUCTION_RESPONSE_MESSAGE_H
#define ANH_CANCEL_LIVE_AUCTION_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S]
// http://wiki.swganh.org/index.php/CancelLiveAuctionMessage
namespace packets {
struct CancelLiveAuctionMessage : public BasePacket
{
    explicit CancelLiveAuctionMessage(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}
    uint64_t auction_id;    
};

class CancelLiveAuctionMessageEvent : public anh::event_dispatcher::BasicEvent<CancelLiveAuctionMessage>{
public:    
    CancelLiveAuctionMessageEvent(std::shared_ptr<network::Session> session_  = nullptr) : anh::event_dispatcher::BasicEvent<CancelLiveAuctionMessage>("CancelLiveAuctionMessage"){}
    virtual ~CancelLiveAuctionMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer)
    {
        auction_id = buffer.read<uint64_t>();
    }
};

} // packets

#endif // ANH_CANCEL_LIVE_AUCTION_RESPONSE_MESSAGE_H
#ifndef ANH_PACKETS_GET_AUCTION_DETAILS_H
#define ANH_PACKETS_GET_AUCTION_DETAILS_H

#include <packets/BasePacket.h>

// Originates on client
namespace packets {
struct GetAuctionDetails  : public BasePacket
{
    explicit GetAuctionDetails (std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE)
        {}
    uint64_t auction_id;
};

class GetAuctionDetailsEvent : public anh::event_dispatcher::BasicEvent<GetAuctionDetails >{
public:    
    GetAuctionDetailsEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<GetAuctionDetails >("GetAuctionDetails"){}
    virtual ~GetAuctionDetailsEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        auction_id = buffer.read<uint64_t>();
    }
};

} // packets

#endif // ANH_PACKETS_GET_AUCTION_DETAILS_H
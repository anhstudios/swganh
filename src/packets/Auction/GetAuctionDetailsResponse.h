#ifndef ANH_PACKETS_GET_AUCTION_DETAILS_RESPONSE_H
#define ANH_PACKETS_GET_AUCTION_DETAILS_RESPONSE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
namespace packets {
struct AuctionDescription
{
    std::string descriptor;
    std::wstring description;
};
struct GetAuctionDetailsResponse  : public BasePacket
{
    GetAuctionDetailsResponse (std::shared_ptr<network::Session> session_  = nullptr, uint64_t item_id_ = 0,
        std::list<AuctionDescription> descriptions_ = std::list<AuctionDescription>() )
        : BasePacket(session_, CLIENT)
        , item_id(item_id_)
        , descriptions(descriptions_)
        {}
    uint64_t item_id;
    std::list<AuctionDescription> descriptions;
};

class GetAuctionDetailsResponseEvent : public anh::event_dispatcher::BasicEvent<GetAuctionDetailsResponse >{
public:    
    GetAuctionDetailsResponseEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t item_id_ = 0,
        std::list<AuctionDescription> descriptions_ = std::list<AuctionDescription>()) : anh::event_dispatcher::BasicEvent<GetAuctionDetailsResponse >("GetAuctionDetailsResponse"){}
    virtual ~GetAuctionDetailsResponseEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int64_t>(item_id);
        buffer.write<uint32_t>(descriptions.size());
        std::for_each(descriptions.begin(), descriptions.end(), [&buffer] (AuctionDescription desc) {
            buffer.write<std::string>(desc.descriptor);
            buffer.write<std::wstring>(desc.description);
        });
    }
};

} // packets

#endif // ANH_PACKETS_GET_AUCTION_DETAILS_RESPONSE_H
#ifndef ANH_PACKETS_COMMODITIES_ITEM_TYPE_LIST_REQUEST_H
#define ANH_PACKETS_COMMODITIES_ITEM_TYPE_LIST_REQUEST_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S]
namespace packets {
struct CommoditiesItemTypeListRequest : public BasePacket
{
    explicit CommoditiesItemTypeListRequest(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}

    std::string from_string;
};

class CommoditiesItemTypeListRequestEvent : public anh::event_dispatcher::BasicEvent<CommoditiesItemTypeListRequest>{
public:    
    CommoditiesItemTypeListRequestEvent() : anh::event_dispatcher::BasicEvent<CommoditiesItemTypeListRequest>("CommoditiesItemTypeListRequest"){}
    virtual ~CommoditiesItemTypeListRequestEvent() {}
    void deserialize(anh::ByteBuffer& buffer){
        from_string = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_COMMODITIES_ITEM_TYPE_LIST_REQUEST_H
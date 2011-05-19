#ifndef ANH_PACKETS_COMMODITIES_ITEM_TYPE_LIST_RESPONSE_H
#define ANH_PACKETS_COMMODITIES_ITEM_TYPE_LIST_RESPONSE_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S]
namespace packets {
struct CommoditiesItemTypeListResponse : public BasePacket
{
    explicit CommoditiesItemTypeListResponse(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}
    // server.total items ex: Shadowfire.4606
    std::string server_num_items;
    int32_t sub_cat_counter;
    int32_t sub_cat;
    int32_t sub_cat_listings;
    std::string directory;
    int32_t unknown;
    std::wstring type_name;    
};

class CommoditiesItemTypeListResponseEvent : public anh::event_dispatcher::BasicEvent<CommoditiesItemTypeListResponse>{
public:    
    CommoditiesItemTypeListResponseEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<CommoditiesItemTypeListResponse>("CommoditiesItemTypeListResponse"){}
    virtual ~CommoditiesItemTypeListResponseEvent() {}
    void deserialize(anh::ByteBuffer& buffer) { 
        server_num_items = buffer.read<std::string>();
        sub_cat_counter = buffer.read<int32_t>();
        sub_cat = buffer.read<int32_t>();
        sub_cat_listings = buffer.read<int32_t>();
        directory = buffer.read<std::string>();
        unknown = buffer.read<int32_t>();
        type_name = buffer.read<std::wstring>();
    }
};

} // packets

#endif // ANH_PACKETS_COMMODITIES_ITEM_TYPE_LIST_RESPONSE_H
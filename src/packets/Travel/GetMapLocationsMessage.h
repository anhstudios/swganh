#ifndef ANH_PACKETS_GET_MAP_LOCATIONS_MESSAGE_H
#define ANH_PACKETS_GET_MAP_LOCATIONS_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on client
namespace packets {
struct GetMapLocationsMessage  : public BasePacket
{
    GetMapLocationsMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, ZONE)
        {}
    std::string planet;
    float x, z;
    uint8_t category, sub_category, icon;
};

class GetMapLocationsMessageEvent : public anh::event_dispatcher::BasicEvent<GetMapLocationsMessage >{
public:    
    GetMapLocationsMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<GetMapLocationsMessage >("GetMapLocationsMessage"){}
    virtual ~GetMapLocationsMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        planet = buffer.read<std::string>();
        x = buffer.read<float>();
        z = buffer.read<float>();
        category = buffer.read<uint8_t>();
        sub_category = buffer.read<uint8_t>();
        icon = buffer.read<uint8_t>();
    }
};

} // packets

#endif // ANH_PACKETS_GET_MAP_LOCATIONS_MESSAGE_H
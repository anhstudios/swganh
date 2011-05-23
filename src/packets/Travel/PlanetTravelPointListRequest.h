#ifndef ANH_PACKETS_PLANET_TRAVEL_POINT_LIST_REQUEST_H
#define ANH_PACKETS_PLANET_TRAVEL_POINT_LIST_REQUEST_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
namespace packets {
struct PlanetTravelPointListRequest : public BasePacket
{
    explicit PlanetTravelPointListRequest(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, ZONE){}
    uint64_t object_id;
    std::string planet_name;
};

class PlanetTravelPointListRequestEvent : public anh::event_dispatcher::BasicEvent<PlanetTravelPointListRequest>{
public:    
    PlanetTravelPointListRequestEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) : anh::event_dispatcher::BasicEvent<PlanetTravelPointListRequest>("PlanetTravelPointListRequest"){}
    virtual ~PlanetTravelPointListRequestEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        object_id = buffer.read<uint64_t>();
        planet_name = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_PLANET_TRAVEL_POINT_LIST_REQUEST_H
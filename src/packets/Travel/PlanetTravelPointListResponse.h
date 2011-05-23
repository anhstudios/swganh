#ifndef ANH_PACKETS_PLANET_TRAVEL_POINT_LIST_RESPONSE_H
#define ANH_PACKETS_PLANET_TRAVEL_POINT_LIST_RESPONSE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
namespace packets {
struct LocationCoords {
    float x,y,z;
};
struct PlanetTravelPointListResponse  : public BasePacket
{
    PlanetTravelPointListResponse (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& planet_string_ = std::string(),
        std::list<std::string> location_names_ = std::list<std::string>(), std::list<LocationCoords> location_coords_ = std::list<LocationCoords>(), 
        std::list<int32_t> location_taxes_ = std::list<int32_t>(), std::list<uint8_t> location_startport_flag_ = std::list<uint8_t>())
        : BasePacket(session_, CLIENT)
        , planet_string(planet_string_)
        , location_names(location_names_)
        , location_taxes(location_taxes_)
        , location_starport_flag(location_startport_flag_)
        {}
    std::string planet_string;
    std::list<std::string> location_names;
    std::list<LocationCoords> location_coords;
    std::list<int32_t> location_taxes;
    std::list<uint8_t> location_starport_flag;
};

class PlanetTravelPointListResponseEvent : public anh::event_dispatcher::BasicEvent<PlanetTravelPointListResponse >{
public:    
    PlanetTravelPointListResponseEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& planet_string_ = std::string(),
        std::list<std::string> location_names_ = std::list<std::string>(), std::list<LocationCoords> location_coords_ = std::list<LocationCoords>(), 
        std::list<int32_t> location_taxes_ = std::list<int32_t>(), std::list<uint8_t> location_startport_flag_ = std::list<uint8_t>()) 
        : anh::event_dispatcher::BasicEvent<PlanetTravelPointListResponse >("PlanetTravelPointListResponse"){}
    virtual ~PlanetTravelPointListResponseEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(planet_string);
        buffer.write<uint32_t>(location_names.size());
        std::for_each(location_names.begin(), location_names.end(), [&buffer] (std::string loc) {
            buffer.write<std::string>(loc);
        });
        buffer.write<uint32_t>(location_coords.size());
        std::for_each(location_coords.begin(), location_coords.end(), [&buffer] (LocationCoords coords) {
            buffer.write<float>(coords.x);
            buffer.write<float>(coords.y);
            buffer.write<float>(coords.z);
        });
        buffer.write<uint32_t>(location_taxes.size());
        std::for_each(location_taxes.begin(), location_taxes.end(), [&buffer] (int32_t taxes) {
            buffer.write<int32_t>(taxes);
        });
        buffer.write<uint32_t>(location_starport_flag.size());
        std::for_each(location_starport_flag.begin(), location_starport_flag.end(), [&buffer] (uint8_t flag) {
            buffer.write<uint8_t>(flag);
        });
    }
};

} // packets

#endif // ANH_PACKETS_PLANET_TRAVEL_POINT_LIST_RESPONSE_H
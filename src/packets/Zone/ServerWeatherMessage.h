#ifndef ANH_PACKETS_SERVER_WEATHER_MESSAGE_H
#define ANH_PACKETS_SERVER_WEATHER_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct ServerWeatherMessage  : public BasePacket
{
    ServerWeatherMessage (std::shared_ptr<network::Session> session_  = nullptr, uint32_t weather_id_ = 0
        , float cloud_x_ = 0.0f, float cloud_z_ = 0.0f, float cloud_y_ = 0.0f)
        : BasePacket(session_, CLIENT)
        , weather_id(weather_id_)
        , cloud_y(cloud_y_)
        , cloud_x(cloud_x_)
        , cloud_z(cloud_z_)
        {}
    uint32_t weather_id;
    float cloud_x, cloud_z, cloud_y;
};

class ServerWeatherMessageEvent : public anh::event_dispatcher::BasicEvent<ServerWeatherMessage>{
public:    
    ServerWeatherMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint32_t weather_id_ = 0
        , float cloud_x_ = 0.0f, float cloud_z_ = 0.0f, float cloud_y_ = 0.0f) 
        : anh::event_dispatcher::BasicEvent<ServerWeatherMessage>("ServerWeatherMessage"){}
    virtual ~ServerWeatherMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(weather_id);
        buffer.write<float>(cloud_x);
        buffer.write<float>(cloud_z);
        buffer.write<float>(cloud_y);
    }
};

} // packets

#endif // ANH_PACKETS_SERVER_WEATHER_MESSAGE_H
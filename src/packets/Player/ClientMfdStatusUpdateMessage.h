#ifndef ANH_PACKETS_CLIENT_MFDSTATUS_UPDATE_MESSAGE_H
#define ANH_PACKETS_CLIENT_MFDSTATUS_UPDATE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on server
namespace packets {
struct ClientMfdStatusUpdateMessage : public BasePacket
{
    ClientMfdStatusUpdateMessage(std::shared_ptr<network::Session> session_  = nullptr, std::string& planet_ = std::string(),
        uint64_t object_id_ = 0, float x_= 0.0f, float y_= 0.0f, float z_ = 0.0f)
        : BasePacket(session_, CLIENT)
        , planet(planet_)
        , object_id(object_id_)
        , x(x_)
        , y(y_)
        , z(z_)
    {}
    std::string planet;
    uint64_t object_id;
    float x;
    float y;
    float z;
};

class ClientMfdStatusUpdateMessageEvent : public anh::event_dispatcher::BasicEvent<ClientMfdStatusUpdateMessage>{
public:    
    ClientMfdStatusUpdateMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, std::string& planet_ = std::string(),
        uint64_t object_id_ = 0, float x_= 0.0f, float y_= 0.0f, float z_ = 0.0f) : anh::event_dispatcher::BasicEvent<ClientMfdStatusUpdateMessage>("ClientMfdStatusUpdateMessage"){}
    virtual ~ClientMfdStatusUpdateMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(planet);
        buffer.write<uint64_t>(object_id);
        buffer.write<float>(x);
        buffer.write<float>(y);
        buffer.write<float>(z);
    }
};

} // packets

#endif // ANH_PACKETS_CLIENT_MFDSTATUS_UPDATE_MESSAGE_H
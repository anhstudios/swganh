#ifndef ANH_PACKETS_UPDATE_TRANSFORM_MESSAGE_H
#define ANH_PACKETS_UPDATE_TRANSFORM_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UpdateTransformMessage  : public BasePacket
{
    UpdateTransformMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_= 0
        , float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, uint32_t update_counter_ = 0
        , int8_t posture_ = 0, int8_t direction_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        , x(x_)
        , y(y_)
        , z(z_)
        , update_counter(update_counter_)
        , posture(posture_)
        , direction(direction_)
        {}
    uint64_t object_id;
    float x,y,z;
    uint32_t update_counter;
    int8_t posture, direction;
};

class UpdateTransformMessageEvent : public anh::event_dispatcher::BasicEvent<UpdateTransformMessage>{
public:    
    UpdateTransformMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_= 0
        , float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, uint32_t update_counter_ = 0
        , int8_t posture_ = 0, int8_t direction_ = 0 
        : anh::event_dispatcher::BasicEvent<UpdateTransformMessage>("UpdateTransformMessage"){}
    virtual ~UpdateTransformMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
        buffer.write<uint16_t>(x * 4.f + 0.5f);
        buffer.write<uint16_t>(y * 4.f + 0.5f);
        buffer.write<uint16_t>(z * 4.f + 0.5f);
        buffer.write<uint32_t>(update_counter);
        buffer.write<int8_t>(posture);
        buffer.write<int8_t>(direction);
    }
};

} // packets

#endif // ANH_PACKETS_UPDATE_TRANSFORM_MESSAGE_H
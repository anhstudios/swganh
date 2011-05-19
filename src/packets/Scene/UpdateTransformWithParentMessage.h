#ifndef ANH_PACKETS_UPDATE_TRANSFORM_WITH_PARENT_MESSAGE_H
#define ANH_PACKETS_UPDATE_TRANSFORM_WITH_PARENT_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UpdateTransformWithParentMessage  : public BasePacket
{
    UpdateTransformWithParentMessage (std::shared_ptr<network::Session> session_  = nullptr, uint64_t cell_id_ = 0,
        uint64_t object_id_= 0, float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, uint32_t update_counter_ = 0
        , int8_t posture_ = 0, int8_t direction_ = 0)
        : BasePacket(session_, CLIENT)
        , cell_id(cell_id_)
        , object_id(object_id_)
        , x(x_)
        , y(y_)
        , z(z_)
        , update_counter(update_counter_)
        , posture(posture_)
        , direction(direction_)
        {}
    uint64_t cell_id, object_id;
    float x,y,z;
    uint32_t update_counter;
    int8_t posture, direction;
};

class UpdateTransformWithParentMessageEvent : public anh::event_dispatcher::BasicEvent<UpdateTransformWithParentMessage>{
public:    
    UpdateTransformWithParentMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_= 0
        , float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, uint32_t update_counter_ = 0
        , int8_t posture_ = 0, int8_t direction_ = 0 
        : anh::event_dispatcher::BasicEvent<UpdateTransformWithParentMessage>("UpdateTransformWithParentMessage"){}
    virtual ~UpdateTransformWithParentMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(cell_id);
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

#endif // ANH_PACKETS_UPDATE_TRANSFORM_WITH_PARENT_MESSAGE_H
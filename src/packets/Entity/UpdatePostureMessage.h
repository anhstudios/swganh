#ifndef ANH_PACKETS_UPDATE_POSTURE_MESSAGE_H
#define ANH_PACKETS_UPDATE_POSTURE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UpdatePostureMessage  : public BasePacket
{
    UpdatePostureMessage (std::shared_ptr<network::Session> session_  = nullptr, int8_t posture_id_ = 0
        , uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , posture_id(posture_id_)
        , object_id(object_id_)
        {}
    int8_t posture_id;
    uint64_t object_id;
};

class UpdatePostureMessageEvent : public anh::event_dispatcher::BasicEvent<UpdatePostureMessage>{
public:    
    UpdatePostureMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint8_t posture_id_ = 0
        , uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<UpdatePostureMessage>("UpdatePostureMessage"){}
    virtual ~UpdatePostureMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int8_t>(posture_id);
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_UPDATE_POSTURE_MESSAGE_H
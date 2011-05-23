#ifndef ANH_PACKETS_UPDATE_CONTAINMENT_MESSAGE_H
#define ANH_PACKETS_UPDATE_CONTAINMENT_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UpdateContainmentMessage  : public BasePacket
{
    UpdateContainmentMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0
        , uint64_t container_id_ = 0, int32_t slot_index_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        , container_id(container_id_)
        , slot_index(slot_index_)
        {}
    uint64_t object_id;
    uint64_t container_id;
    int32_t slot_index;
};

class UpdateContainmentMessageEvent : public anh::event_dispatcher::BasicEvent<UpdateContainmentMessage>{
public:    
    UpdateContainmentMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0
        , uint64_t container_id_ = 0, int32_t slot_index_ = 0) 
        : anh::event_dispatcher::BasicEvent<UpdateContainmentMessage>("UpdateContainmentMessage"){}
    virtual ~UpdateContainmentMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
        buffer.write<uint64_t>(container_id);
        buffer.write<int32_t>(slot_index);
    }
};

} // packets

#endif // ANH_PACKETS_UPDATE_CONTAINMENT_MESSAGE_H
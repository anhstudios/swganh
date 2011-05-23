#ifndef ANH_PACKETS_DELTAS_MESSAGE_H
#define ANH_PACKETS_DELTAS_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct DeltasMessage : public BasePacket
{
    DeltasMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0, uint32_t object_type_ = 0,
        uint8_t update_type_ = 0, uint32_t object_data_size_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        , object_type(object_type_)
        , update_type(update_type_)
        , object_data_size(object_data_size_)
        {}
    uint64_t object_id;
    uint32_t object_type;
    uint8_t update_type;
    uint32_t object_data_size;
};

class DeltasMessageEvent : public anh::event_dispatcher::BasicEvent<DeltasMessage>{
public:    
    DeltasMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0, uint32_t object_type_ = 0,
        uint8_t update_type_ = 0, uint32_t object_data_size_ = 0) 
        : anh::event_dispatcher::BasicEvent<DeltasMessage>("DeltasMessage"){}
    virtual ~DeltasMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
        buffer.write<uint32_t>(object_type);
        buffer.write<uint8_t>(update_type);
        buffer.write<uint32_t>(object_data_size);
    }
};

} // packets

#endif // ANH_PACKETS_DELTAS_MESSAGE_H
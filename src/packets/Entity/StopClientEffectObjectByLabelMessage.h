#ifndef ANH_PACKETS_STOP_CLIENT_EFFECT_BY_LABEL_MESSAGE_H
#define ANH_PACKETS_STOP_CLIENT_EFFECT_BY_LABEL_MESSAGE_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server
namespace packets {
struct StopClientEffectByLabelMessage  : public BasePacket
{
    StopClientEffectByLabelMessage (std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_ = 0,
        std::string effect_label_ = std::string())
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        , effect_label(effect_label_)
        {}
    uint64_t object_id;
    std::string effect_label;
};

class StopClientEffectByLabelMessageEvent : public anh::event_dispatcher::BasicEvent<StopClientEffectByLabelMessage>{
public:    
    StopClientEffectByLabelMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_ = 0,
        std::string effect_label_ = std::string()) 
        : anh::event_dispatcher::BasicEvent<StopClientEffectByLabelMessage>("StopClientEffectByLabelMessage"){}
    virtual ~StopClientEffectByLabelMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
        buffer.write<std::string>(effect_label);
    }
};

} // packets

#endif // ANH_PACKETS_STOP_CLIENT_EFFECT_BY_LABEL_MESSAGE_H
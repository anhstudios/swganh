#ifndef ANH_PACKETS_SCENE_END_BASELINES_H
#define ANH_PACKETS_SCENE_END_BASELINES_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct SceneEndBaselines  : public BasePacket
{
    SceneEndBaselines (std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        {}
    uint64_t object_id;
};

class SceneEndBaselinesEvent : public anh::event_dispatcher::BasicEvent<SceneEndBaselines>{
public:    
    SceneEndBaselinesEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<SceneEndBaselines >("SceneEndBaselines"){}
    virtual ~SceneEndBaselinesEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_SCENE_END_BASELINES_H
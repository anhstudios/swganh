#ifndef ANH_PACKETS_SCENE_DESTROY_OBJECT_H
#define ANH_PACKETS_SCENE_DESTROY_OBJECT_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct SceneDestroyObject  : public BasePacket
{
    SceneDestroyObject (std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        {}
    uint64_t object_id;
};

class SceneDestroyObjectEvent : public anh::event_dispatcher::BasicEvent<SceneDestroyObject>{
public:    
    SceneDestroyObjectEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<SceneDestroyObject >("SceneDestroyObject"){}
    virtual ~SceneDestroyObjectEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_SCENE_DESTROY_OBJECT_H
#ifndef ANH_PACKETS_CMD_SCENE_READY_H
#define ANH_PACKETS_CMD_SCENE_READY_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ] or Client [ C -> S ]
// http://wiki.swganh.org/index.php/CmdSceneReady
namespace packets {
struct CmdSceneReady : public BasePacket
{
    explicit CmdSceneReady(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, ZONE){}
    CmdSceneReady(std::shared_ptr<anh::network::soe::Session> session_, RoutingDestination dest)
        : BasePacket(session_, dest){}
};

class CmdSceneReadyEvent : public anh::event_dispatcher::BasicEvent<CmdSceneReady>{
public:    
    CmdSceneReadyEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<CmdSceneReady>("CmdSceneReady"){}
    virtual ~CmdSceneReadyEvent() {}
};

} // packets

#endif // ANH_PACKETS_CMD_SCENE_READY_H
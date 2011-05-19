#ifndef ANH_PACKETS_UPDATE_PVP_STATUS_MESSAGE_H
#define ANH_PACKETS_UPDATE_PVP_STATUS_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UpdatePvpStatusMessage  : public BasePacket
{
    UpdatePvpStatusMessage (std::shared_ptr<network::Session> session_  = nullptr, uint32_t pvp_status_ = 0
        , uint32_t faction_= 0, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , pvp_status(pvp_status_)
        , faction(faction_)
        , object_id(object_id_)
        {}
    uint32_t pvp_status;
    uint32_t faction;
    uint64_t object_id;
};

class UpdatePvpStatusMessageEvent : public anh::event_dispatcher::BasicEvent<UpdatePvpStatusMessage>{
public:    
    UpdatePvpStatusMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint32_t pvp_status_ = 0
        , uint32_t faction_= 0, uint64_t object_id_ = 0 
        : anh::event_dispatcher::BasicEvent<UpdatePvpStatusMessage>("UpdatePvpStatusMessage"){}
    virtual ~UpdatePvpStatusMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(pvp_status);
        buffer.write<uint32_t>(faction);
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_UPDATE_PVP_STATUS_MESSAGE_H
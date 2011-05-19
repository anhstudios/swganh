#ifndef ANH_PACKETS_ENTER_STRUCTURE_PLACEMENT_MODE_MESSAGE_H
#define ANH_PACKETS_ENTER_STRUCTURE_PLACEMENT_MODE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct EnterStructurePlacementModeMessage  : public BasePacket
{
    EnterStructurePlacementModeMessage (std::shared_ptr<network::Session> session_  = nullptr, 
        uint64_t deed_id_ = 0, std::string& obj_iff_ = std::string())
        : BasePacket(session_, CLIENT)
        , deed_id(deed_id_)
        , obj_iff(obj_iff_)
        {}
    uint64_t deed_id;
    std::string obj_iff;
};

class EnterStructurePlacementModeMessageEvent : public anh::event_dispatcher::BasicEvent<EnterStructurePlacementModeMessage >{
public:    
    EnterStructurePlacementModeMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, 
        uint64_t deed_id_ = 0, std::string& obj_iff_ = std::string()) 
        : anh::event_dispatcher::BasicEvent<EnterStructurePlacementModeMessage >("EnterStructurePlacementModeMessage"){}
    virtual ~EnterStructurePlacementModeMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(deed_id);
        buffer.write<std::string>(obj_iff);
    }
};

} // packets

#endif // ANH_PACKETS_ENTER_STRUCTURE_PLACEMENT_MODE_MESSAGE_H
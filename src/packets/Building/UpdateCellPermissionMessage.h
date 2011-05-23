#ifndef ANH_PACKETS_UPDATE_CELL_PERMISSION_MESSAGE_H
#define ANH_PACKETS_UPDATE_CELL_PERMISSION_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct UpdateCellPermissionMessage  : public BasePacket
{
    UpdateCellPermissionMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint8_t permission_flag_ = 0
        , uint64_t cell_id_ = 0)
        : BasePacket(session_, CLIENT)
        , permission_flag(permission_flag_)
        , cell_id(cell_id_)
        {}
    uint8_t permission_flag;
    uint64_t cell_id;
};

class UpdateCellPermissionMessageEvent : public anh::event_dispatcher::BasicEvent<UpdateCellPermissionMessage>{
public:    
    UpdateCellPermissionMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint8_t permission_flag_ = 0
        , uint64_t cell_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<UpdateCellPermissionMessage>("UpdateCellPermissionMessage"){}
    virtual ~UpdateCellPermissionMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint8_t>(permission_flag);
        buffer.write<uint64_t>(cell_id);
    }
};

} // packets

#endif // ANH_PACKETS_UPDATE_CELL_PERMISSION_MESSAGE_H
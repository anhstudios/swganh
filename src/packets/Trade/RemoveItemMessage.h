#ifndef ANH_PACKETS_REMOVE_ITEM_MESSAGE_H
#define ANH_PACKETS_REMOVE_ITEM_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct RemoveItemMessage  : public BasePacket
{
    RemoveItemMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
        {}
    uint64_t object_id;
};

class RemoveItemMessageEvent : public anh::event_dispatcher::BasicEvent<RemoveItemMessage >{
public:    
    RemoveItemMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<RemoveItemMessage >("RemoveItemMessage"){}
    virtual ~RemoveItemMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_REMOVE_ITEM_MESSAGE_H
#ifndef ANH_PACKETS_ADD_ITEM_MESSAGE_H
#define ANH_PACKETS_ADD_ITEM_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on server or client
// http://wiki.swganh.org/index.php/AddItemMessage
namespace packets {
struct AddItemMessage : public BasePacket
{
explicit AddItemMessage(std::shared_ptr<anh::network::soe::Session> session_ = nullptr)
        : BasePacket(session_, ZONE){}
    AddItemMessage(std::shared_ptr<anh::network::soe::Session> session_ = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
    {}
    uint64_t object_id;
};

class AddItemMessageEvent : public anh::event_dispatcher::BasicEvent<AddItemMessage>{
public:    
    AddItemMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<AddItemMessage>("AddItemMessage"){}
    virtual ~AddItemMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
    }
    void deserialize(anh::ByteBuffer& buffer) {
        object_id = buffer.read<uint64_t>();
    }
};

} // packets

#endif // ANH_PACKETS_ADD_ITEM_MESSAGE_H
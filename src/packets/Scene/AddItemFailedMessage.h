#ifndef ANH_PACKETS_ADD_ITEM_FAILED_MESSAGE_H
#define ANH_PACKETS_ADD_ITEM_FAILED_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on server
// http://wiki.swganh.org/index.php/AddItemFailedMessage
namespace packets {
struct AddItemFailedMessage : public BasePacket
{
    AddItemFailedMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , object_id(object_id_)
    {}
    uint64_t object_id;
};

class AddItemFailedMessageEvent : public anh::event_dispatcher::BasicEvent<AddItemFailedMessage>{
public:    
    AddItemFailedMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<AddItemFailedMessage>("AddItemFailedMessage"){}
    virtual ~AddItemFailedMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_ADD_ITEM_FAILED_MESSAGE_H
#ifndef ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
#define ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
// http://wiki.swganh.org/index.php/DeleteCharacterReplyMessage
namespace packets {
struct DeleteCharacterReplyMessage : public BasePacket
{
    DeleteCharacterReplyMessage(std::shared_ptr<network::Session> session_  = nullptr, int32_t failure_flag_ = 0)
        : BasePacket(session_, CLIENT)
        , failure_flag(failure_flag_){}
    int32_t failure_flag;
};

class DeleteCharacterReplyMessageEvent : public anh::event_dispatcher::BasicEvent<DeleteCharacterReplyMessage>{
public:    
    DeleteCharacterReplyMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, int32_t failure_flag_ = 0) 
        : anh::event_dispatcher::BasicEvent<DeleteCharacterReplyMessage>("DeleteCharacterReplyMessage"){}
    virtual ~DeleteCharacterReplyMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int32_t>(failure_flag);
    }
};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
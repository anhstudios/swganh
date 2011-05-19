#ifndef ANH_PACKETS_APPEND_COMMENT_RESPONSE_MESSAGE_H
#define ANH_PACKETS_APPEND_COMMENT_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on server
// http://wiki.swganh.org/index.php/AppendCommentResponseMessage
namespace packets {
struct AppendCommentResponseMessage : public BasePacket
{
    AppendCommentResponseMessage(std::shared_ptr<network::Session> session_  = nullptr, int32_t message_ = 0,int32_t ticket_ = 0)
        : BasePacket(session_, CLIENT)
        , message(message_)
        , ticket(ticket_)
    {}
    int32_t message;
    int32_t ticket;
};

class AppendCommentResponseMessageEvent : public anh::event_dispatcher::BasicEvent<AppendCommentResponseMessage>{
public:    
    AppendCommentResponseMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, int32_t message_ = 0,int32_t ticket_ = 0) 
        : anh::event_dispatcher::BasicEvent<AppendCommentResponseMessage>("AppendCommentResponseMessage"){}
    virtual ~AppendCommentResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int32_t>(message);
        buffer.write<int32_t>(ticket);
    }
};

} // packets

#endif // ANH_PACKETS_APPEND_COMMENT_RESPONSE_MESSAGE_H
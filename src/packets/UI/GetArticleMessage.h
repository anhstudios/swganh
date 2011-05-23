#ifndef ANH_PACKETS_GET_ARTICLE_MESSAGE_H
#define ANH_PACKETS_GET_ARTICLE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on client
namespace packets {
struct GetArticleMessage  : public BasePacket
{
    GetArticleMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, ZONE)
        {}
    std::string article_id;
};

class GetArticleMessageEvent : public anh::event_dispatcher::BasicEvent<GetArticleMessage >{
public:    
    GetArticleMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<GetArticleMessage >("GetArticleMessage"){}
    virtual ~GetArticleMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        article_id = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_GET_ARTICLE_MESSAGE_H
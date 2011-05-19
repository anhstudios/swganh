#ifndef ANH_PACKETS_GET_ARTICLE_RESPONSE_MESSAGE_H
#define ANH_PACKETS_GET_ARTICLE_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct GetArticleResponseMessage  : public BasePacket
{
    GetArticleResponseMessage (std::shared_ptr<network::Session> session_  = nullptr, int32_t options_ = 0
        , std::wstring& text_ = std::wstring())
        : BasePacket(session_, CLIENT)
        , options(options_)
        , text(text_)
        {}
    int32_t options;
    std::wstring& text;
};

class GetArticleResponseMessageEvent : public anh::event_dispatcher::BasicEvent<GetArticleResponseMessage >{
public:    
    GetArticleResponseMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, int32_t options_ = 0
        , std::wstring& text_ = std::wstring()) 
        : anh::event_dispatcher::BasicEvent<GetArticleResponseMessage >("GetArticleResponseMessage"){}
    virtual ~GetArticleResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<int32_t>(options);
        buffer.write<std::wstring>(text);
    }
};

} // packets

#endif // ANH_PACKETS_GET_ARTICLE_RESPONSE_MESSAGE_H
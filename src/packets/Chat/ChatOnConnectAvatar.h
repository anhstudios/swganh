#ifndef ANH_PACKETS_CHAT_ON_CONNECT_AVATAR_H
#define ANH_PACKETS_CHAT_ON_CONNECT_AVATAR_H

#include <packets/BasePacket.h>

// Originates on server
// http://wiki.swganh.org/index.php/ChatOnConnectAvatar
namespace packets {
struct ChatOnConnectAvatar  : public BasePacket
{
    explicit ChatOnConnectAvatar (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT){}
};

class ChatOnConnectAvatarEvent : public anh::event_dispatcher::BasicEvent<ChatOnConnectAvatar >{
public:    
    ChatOnConnectAvatarEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr=nullptr) : anh::event_dispatcher::BasicEvent<ChatOnConnectAvatar >("ChatOnConnectAvatar"){}
    virtual ~ChatOnConnectAvatarEvent() {}
};

} // packets

#endif // ANH_PACKETS_CHAT_ON_CONNECT_AVATAR_H
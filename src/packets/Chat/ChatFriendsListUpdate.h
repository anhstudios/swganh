#ifndef ANH_PACKETS_CHAT_FRIENDS_LIST_UPDATE_H
#define ANH_PACKETS_CHAT_FRIENDS_LIST_UPDATE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/ChatFriendsListUpdate
namespace packets {
struct ChatFriendsListUpdate   : public BasePacket
{
    ChatFriendsListUpdate  (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& game_name_ = std::string(""),
        std::string& server_name_ = std::string(""), std::string& friend_name_ = std::string(""), uint8_t online_flag_ = 0)
        : BasePacket(session_, SHARED_SERVICE)
        , game_name(game_name_)
        , server_name(server_name_)
        , friend_name(friend_name_)
        , online_flag(online_flag_)
    {}
    std::string game_name;
    std::string server_name;
    std::string friend_name;
    uint8_t online_flag;
};

class ChatFriendsListUpdateEvent : public anh::event_dispatcher::BasicEvent<ChatFriendsListUpdate >{
public:    
    ChatFriendsListUpdateEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& game_name_ = std::string(""),
        std::string& server_name_ = std::string(""), std::string& friend_name_ = std::string(""), uint8_t online_flag_ = 0) 
        : anh::event_dispatcher::BasicEvent<ChatFriendsListUpdate >("ChatFriendsListUpdate"){}
    virtual ~ChatFriendsListUpdateEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(game_name);
        buffer.write<std::string>(server_name);
        buffer.write<std::string>(friend_name);
        buffer.write<uint8_t>(online_flag);
    }

};

} // packets

#endif // ANH_PACKETS_CHAT_FRIENDS_LIST_UPDATE_H
#ifndef ANH_PACKETS_CHAT_ON_BAN_AVATAR_FROM_ROOM_H
#define ANH_PACKETS_CHAT_ON_BAN_AVATAR_FROM_ROOM_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/ChatOnBanAvatarFromRoom
namespace packets {
struct ChatOnBanAvatarFromRoom   : public BasePacket
{
    ChatOnBanAvatarFromRoom  (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& room_name_ = std::string(""),
        std::string& game_from_name_ = std::string(""), std::string& server_from_name_ = std::string(""),std::string& moderator_name_ = std::string(""),
        std::string& game_to_name_ = std::string(""), std::string& server_to_name_ = std::string(""), std::string& player_name_ = std::string("") , 
        int32_t success_bitmask_ = 0, int32_t req_id_ = 0)
        : BasePacket(session_, SHARED_SERVICE)
        , room_name(room_name_)
        , game_from_name(game_from_name_)
        , server_from_name(server_from_name_)
        , moderator_name(moderator_name_)
        , server_to_name(server_to_name_)
        , player_name(player_name_)
        , success_bitmask(success_bitmask_)
        , req_id(req_id_)
    {}
    std::string room_name;
    std::string game_from_name;
    std::string server_from_name;
    std::string moderator_name;
    std::string game_to_name;
    std::string server_to_name;
    std::string player_name;
    int32_t success_bitmask;
    int32_t req_id;
};

class ChatOnBanAvatarFromRoomEvent : public anh::event_dispatcher::BasicEvent<ChatOnBanAvatarFromRoom >{
public:    
    ChatOnBanAvatarFromRoomEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& room_name_ = std::string(""),
        std::string& game_from_name_ = std::string(""), std::string& server_from_name_ = std::string(""),std::string& moderator_name_ = std::string(""),
        std::string& game_to_name_ = std::string(""), std::string& server_to_name_ = std::string(""), std::string& player_name_ = std::string("") , 
        int32_t success_bitmask_ = 0, int32_t req_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<ChatOnBanAvatarFromRoom >("ChatOnBanAvatarFromRoom"){}
    virtual ~ChatOnBanAvatarFromRoomEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(room_name);
        buffer.write<std::string>(game_from_name);
        buffer.write<std::string>(server_from_name);
        buffer.write<std::string>(moderator_name);
        buffer.write<std::string>(game_to_name);
        buffer.write<std::string>(server_to_name);
        buffer.write<std::string>(player_name);
        buffer.write<int32_t>(success_bitmask);
        buffer.write<int32_t>(req_id);
    }
};

} // packets

#endif // ANH_PACKETS_CHAT_ON_BAN_AVATAR_FROM_ROOM_H
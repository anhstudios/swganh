#ifndef ANH_PACKETS_CHAT_ON_ADD_MODERATOR_TO_ROOM_H
#define ANH_PACKETS_CHAT_ON_ADD_MODERATOR_TO_ROOM_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
// http://wiki.swganh.org/index.php/ChatOnAddModeratorToRoom
namespace packets {
struct ChatOnAddModeratorToRoom   : public BasePacket
{
    explicit ChatOnAddModeratorToRoom (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}
    std::string game_from_name;
    std::string server_from_name;
    std::string player_from_name;
    std::string game_to_name;
    std::string server_to_name;
    std::string moderator_name;
    int32_t error_code;
    std::string room_path_name;
    int32_t req_id;
};

class ChatOnAddModeratorToRoomEvent : public anh::event_dispatcher::BasicEvent<ChatOnAddModeratorToRoom >{
public:    
    ChatOnAddModeratorToRoomEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<ChatOnAddModeratorToRoom >("ChatOnAddModeratorToRoom"){}
    virtual ~ChatOnAddModeratorToRoomEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        game_from_name = buffer.read<std::string>();
        server_from_name = buffer.read<std::string>();
        player_from_name = buffer.read<std::string>();
        game_to_name = buffer.read<std::string>();
        server_to_name = buffer.read<std::string>();
        moderator_name = buffer.read<std::string>();
        error_code = buffer.read<int32_t>();
        room_path_name = buffer.read<std::string>();
        req_id = buffer.read<int32_t>();
    }
};

} // packets

#endif // ANH_PACKETS_CHAT_ON_ADD_MODERATOR_TO_ROOM_H
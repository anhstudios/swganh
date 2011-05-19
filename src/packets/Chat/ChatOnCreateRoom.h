#ifndef ANH_PACKETS_CHAT_ON_CREATE_ROOM_H
#define ANH_PACKETS_CHAT_ON_CREATE_ROOM_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/ChatOnCreateRoom
namespace packets {
struct ChatOnCreateRoom   : public BasePacket
{
    ChatOnCreateRoom  (std::shared_ptr<network::Session> session_  = nullptr, std::string& game_from_name_= std::string("SWG"), std::string& server_from_name_= std::string(""),
        std::string& player_from_name_ = std::string(""), std::string& game_to_name_ = std::string("SWG"), std::string& server_to_name_ = std::string(""), std::string& moderator_name_ = std::string(""), int32_t error_code_ = 0,
        std::string& room_path_name_= std::string(""), int32_t req_id_=0)
        : BasePacket(session_, SHARED_SERVICE)
        , game_from_name(game_from_name_)
        , server_from_name(server_from_name_)
        , player_from_name(player_from_name_)
        , server_to_name(server_to_name_)
        , moderator_name(moderator_name_)
        , error_code(error_code_)
        , room_path_name(room_path_name_)
        , req_id(req_id_)
    {}
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

class ChatOnAddModeratorToRoomEvent : public anh::event_dispatcher::BasicEvent<ChatOnCreateRoom >{
public:    
    ChatOnAddModeratorToRoomEvent(std::shared_ptr<network::Session> session_  = nullptr, std::string& game_from_name_= std::string("SWG"), std::string& server_from_name_= std::string(""),
        std::string& player_from_name_ = std::string(""), std::string& game_to_name_ = std::string("SWG"), std::string& server_to_name_ = std::string(""), std::string& moderator_name_ = std::string(""), int32_t error_code_ = 0,
        std::string& room_path_name_= std::string(""), int32_t req_id_=0) 
        : anh::event_dispatcher::BasicEvent<ChatOnCreateRoom >("ChatOnCreateRoom"){}
    virtual ~ChatOnAddModeratorToRoomEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(game_from_name);
        buffer.write<std::string>(server_from_name);
        buffer.write<std::string>(player_from_name);
        buffer.write<std::string>(game_to_name);
        buffer.write<std::string>(server_to_name);
        buffer.write<std::string>(moderator_name);
        buffer.write<int32_t>(error_code);
        buffer.write<std::string>(room_path_name);
        buffer.write<int32_t>(req_id);
    }
};

} // packets

#endif // ANH_PACKETS_CHAT_ON_CREATE_ROOM_H
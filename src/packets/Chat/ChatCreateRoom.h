#ifndef ANH_PACKETS_CHAT_ROOM_CREATE_MESSAGE_H
#define ANH_PACKETS_CHAT_ROOM_CREATE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
// http://wiki.swganh.org/index.php/ChatCreateRoom
namespace packets {
struct ChatCreateRoom  : public BasePacket
{
    explicit ChatCreateRoom (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, SHARED_SERVICE){}
    int32_t error_code;     int32_t chatroom_id;     int32_t private_flag;    uint8_t moderation_flag;    std::string room_full_address;
    std::string game_from;  std::string server_from;    std::string creator;    std::string game_to;    std::string server_to;
    std::string owner;    std::wstring room_name;    int32_t mod_list_size;    std::string mod_game;    std::string mod_server;
    std::string moderator_name;    int32_t user_list_size;    std::string user_game;    std::string user_server;    std::string user_name;
    uint32_t request_id;
};

class ChatCreateRoomEvent : public anh::event_dispatcher::BasicEvent<ChatCreateRoom>{
public:    
    ChatCreateRoomEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) : anh::event_dispatcher::BasicEvent<ChatCreateRoom>("ChatCreateRoom"){}
    virtual ~ChatCreateRoomEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        error_code = buffer.read<int32_t>();
        chatroom_id = buffer.read<int32_t>();
        private_flag = buffer.read<int32_t>();
        moderation_flag = buffer.read<uint8_t>();
        room_full_address = buffer.read<std::string>();
        game_from = buffer.read<std::string>();
        server_from = buffer.read<std::string>();
        creator = buffer.read<std::string>();
        game_to = buffer.read<std::string>();
        server_to = buffer.read<std::string>();
        owner = buffer.read<std::string>();
        room_name = buffer.read<std::wstring>();
        mod_list_size = buffer.read<int32_t>();
        mod_game = buffer.read<std::string>();
        mod_server = buffer.read<std::string>();
        moderator_name = buffer.read<std::string>();
        user_list_size = buffer.read<int32_t>();
        user_game = buffer.read<std::string>();
        user_server =  buffer.read<std::string>();
        user_name = buffer.read<std::string>();
        request_id = buffer.read<uint32_t>();
    }
};

} // packets

#endif // ANH_PACKETS_CHAT_ROOM_CREATE_MESSAGE_H
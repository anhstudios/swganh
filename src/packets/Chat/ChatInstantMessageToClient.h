#ifndef ANH_PACKETS_CHAT_INSTANT_MESSAGE_TO_CLIENT_H
#define ANH_PACKETS_CHAT_INSTANT_MESSAGE_TO_CLIENT_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/ChatInstantMessageToClient
namespace packets {
struct ChatInstantMessageToClient   : public BasePacket
{
    ChatInstantMessageToClient(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& game_name_ = std::string(""), 
        std::string& server_name_ = std::string(""), std::string& player_name_ = std::string(""), std::wstring& message_ = std::wstring(L""), int32_t unknown_ = 0)
        : BasePacket(session_, SHARED_SERVICE)
        , game_name(game_name_)
        , server_name(server_name_)
        , player_name(player_name_)
        , message(message_)
        , unknown(unknown_)
    {}
    std::string game_name;
    std::string server_name;
    std::string player_name;
    std::wstring message;
    int32_t unknown;
};

class ChatInstantMessageToClientEvent : public anh::event_dispatcher::BasicEvent<ChatInstantMessageToClient >{
public:    
    ChatInstantMessageToClientEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& game_name_ = std::string(""), 
        std::string& server_name_ = std::string(""), std::string& player_name_ = std::string(""), std::wstring& message_ = std::wstring(L""), int32_t unknown_ = 0) 
        : anh::event_dispatcher::BasicEvent<ChatInstantMessageToClient >("ChatInstantMessageToClient"){}
    virtual ~ChatInstantMessageToClientEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(game_name);
        buffer.write<std::string>(server_name);
        buffer.write<std::string>(player_name);
        buffer.write<int32_t>(unknown);
    }
};

} // packets

#endif // ANH_PACKETS_CHAT_INSTANT_MESSAGE_TO_CLIENT_H
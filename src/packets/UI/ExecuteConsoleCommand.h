#ifndef ANH_PACKETS_EXECUTE_CONSOLE_COMMAND_H
#define ANH_PACKETS_EXECUTE_CONSOLE_COMMAND_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
namespace packets {
struct ExecuteConsoleCommand : public BasePacket
{
    ExecuteConsoleCommand(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& game_command_ = std::string())
        : BasePacket(session_, CLIENT)
        , game_command(game_command_)
    {}
    std::string game_command;
};

class ExecuteConsoleCommandEvent : public anh::event_dispatcher::BasicEvent<ExecuteConsoleCommand>{
public:    
    ExecuteConsoleCommandEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& game_command_ = std::string()) 
        : anh::event_dispatcher::BasicEvent<ExecuteConsoleCommand>("ExecuteConsoleCommand"){}
    virtual ~ExecuteConsoleCommandEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        game_command = buffer.read<std::string>();
    }
};

} // packets

#endif // ANH_PACKETS_CMD_SCENE_READY_H
#ifndef ANH_PACKETS_CLIENT_RANDOM_NAME_RESPONSE_H
#define ANH_PACKETS_CLIENT_RANDOM_NAME_RESPONSE_H

#include <packets/BasePacket.h>

// Originates on server
namespace packets {
struct ClientRandomNameResponse : public BasePacket
{
    ClientRandomNameResponse(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& player_race_iff_ = std::string(), std::wstring& random_name_ = std::wstring(),
        std::string& stf_file_ = std::string(), uint32_t string_spacer_ = 0, std::string& approval_string_ = std::string())
        : BasePacket(session_, CLIENT)
        , player_race_iff(player_race_iff_)
        , random_name(random_name_)
        , stf_file(stf_file_)
        , string_spacer(string_spacer_)
        , approval_string(approval_string_)
    {}
    std::string player_race_iff;
    std::wstring random_name;
    std::string stf_file;
    uint32_t string_spacer;
    std::string approval_string;
};

class ClientRandomNameResponseEvent : public anh::event_dispatcher::BasicEvent<ClientRandomNameResponse>{
public:    
    ClientRandomNameResponseEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& player_race_iff_ = std::string(), std::wstring& random_name_ = std::wstring(),
        std::string& stf_file_ = std::string(), uint32_t string_spacer_ = 0, std::string& approval_string_ = std::string()) 
        : anh::event_dispatcher::BasicEvent<ClientRandomNameResponse>("ClientRandomNameResponse"){}
    virtual ~ClientRandomNameResponseEvent() {}
    void serialize(anh::ByteBuffer& buffer) { 
        buffer.write<std::string>(player_race_iff);
        buffer.write<std::wstring>(random_name);
        buffer.write<uint32_t>(string_spacer);
        buffer.write<std::string>(approval_string);
    }
};

} // packets

#endif // ANH_PACKETS_CLIENT_RANDOM_NAME_RESPONSE_H
#ifndef ANH_PACKETS_PLAY_CLIENT_EFFECT_OBJECT_MESSAGE_H
#define ANH_PACKETS_PLAY_CLIENT_EFFECT_OBJECT_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct PlayClientEffectObjectMessage  : public BasePacket
{
    PlayClientEffectObjectMessage (std::shared_ptr<network::Session> session_  = nullptr, std::string& file_ = std::string(""), 
        std::string& aux_string_ = std::string(""),uint64_t object_id_ = 0)
        : BasePacket(session_, CLIENT)
        , file(file_)
        , aux_string(aux_string_)
        , object_id(object_id_)
        {}
    std::string file;
    std::string aux_string;
    uint64_t object_id;
};

class PlayClientEffectObjectMessageEvent : public anh::event_dispatcher::BasicEvent<PlayClientEffectObjectMessage >{
public:    
    PlayClientEffectObjectMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, std::string& file_ = std::string(""), 
        std::string& aux_string_ = std::string(""),uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<PlayClientEffectObjectMessage >("PlayClientEffectObjectMessage"){}
    virtual ~PlayClientEffectObjectMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(file);
        buffer.write<std::string>(aux_string);
        buffer.write<uint64_t>(object_id);
    }
};

} // packets

#endif // ANH_PACKETS_PLAY_CLIENT_EFFECT_OBJECT_MESSAGE_H
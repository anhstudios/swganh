#ifndef ANH_PACKETS_PLAY_MUSIC_MESSAGE_H
#define ANH_PACKETS_PLAY_MUSIC_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct PlayMusicMessage  : public BasePacket
{
    PlayMusicMessage (std::shared_ptr<network::Session> session_  = nullptr, std::string& sound_file_ = std::string(""),
        int32_t unk1_ = 0x00, int32_t unk2_ = 0x01, int32_t unk3_ = 0)
        : BasePacket(session_, CLIENT)
        , sound_file(sound_file_)
        , unk1(unk1_)
        , unk2(unk2)
        , unk3(unk3)
        {}
    std::string sound_file;
    int64_t unk1;
    int32_t unk2;
    uint8_t unk3;
};

class PlayMusicMessageEvent : public anh::event_dispatcher::BasicEvent<PlayMusicMessage >{
public:    
    PlayMusicMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, std::string& sound_file_ = std::string(""),
        int32_t unk1_ = 0x00, int32_t unk2_ = 0x01, int32_t unk3_ = 0) 
        : anh::event_dispatcher::BasicEvent<PlayMusicMessage >("PlayMusicMessage"){}
    virtual ~PlayMusicMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(sound_file);
        buffer.write<int64_t>(0x00);
        buffer.write<int32_t>(0x01);
        buffer.write<uint8_t>(0);
    }
};

} // packets

#endif // ANH_PACKETS_PLAY_MUSIC_MESSAGE_H
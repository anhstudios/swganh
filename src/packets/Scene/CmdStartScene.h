#ifndef ANH_PACKETS_CMD_START_SCENE_H
#define ANH_PACKETS_CMD_START_SCENE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
namespace packets {
struct CmdStartScene : public BasePacket
{
    explicit CmdStartScene(std::shared_ptr<network::Session> session_  = nullptr, uint8_t ignore_layout_files_ = 0
        , uint64_t character_id_ = 0, std::string& terrain_map_ = std::string(), float x_= 0.0f, float y_= 0.0f, float z_ = 0.0f
        , std::string& shared_race_template_ = std::string(), uint64_t galactic_time_ = 0)
    : BasePacket(session_, CLIENT)
    , ignore_layout_files(ignore_layout_files_)
    , character_id(character_id_)
    , terrain_map(terrain_map_)
    , x(x_)
    , y(y_)
    , z(z_)
    , shared_race_template(shared_race_template_)
    , galactic_time(galactic_time_)
    {}
    uint8_t ignore_layout_files;
    uint64_t character_id;
    std::string terrain_map;
    float x;
    float y;
    float z;
    std::string shared_race_template;
    uint64_t galactic_time;
};

class CmdStartSceneEvent : public anh::event_dispatcher::BasicEvent<CmdStartScene>{
public:    
    CmdStartSceneEvent(std::shared_ptr<network::Session> session_  = nullptr, uint8_t ignore_layout_files_ = 0
        , uint64_t character_id_ = 0, std::string& terrain_map_ = std::string(), float x_= 0.0f, float y_= 0.0f, float z_ = 0.0f
        , std::string& shared_race_template_ = std::string(), uint64_t galactic_time_ = 0) 
        : anh::event_dispatcher::BasicEvent<CmdStartScene>("CmdStartScene"){}
    virtual ~CmdStartSceneEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint8_t>(ignore_layout_files);
        buffer.write<uint64_t>(character_id);
        buffer.write<std::string>(terrain_map);
        buffer.write<float>(x);
        buffer.write<float>(y);
        buffer.write<float>(z);
        buffer.write<std::string>(shared_race_template);
        buffer.write<uint64_t>(galactic_time);
    }
};

} // packets

#endif // ANH_PACKETS_CMD_START_SCENE_H
#ifndef ANH_PACKETS_CMD_START_SCENE_H
#define ANH_PACKETS_CMD_START_SCENE_H

#include <anh/byte_buffer.h>

// Originates on Server [ S -> C ]
namespace packets {
struct CmdStartScene
{
    CmdStartScene(uint32_t session_, anh::ByteBuffer& buffer_)
    {
        deserialize(buffer_);
    }
    CmdStartScene(uint8_t ignore_layout_files_, uint64_t character_id_, std::string terrain_map_, float x_, float y_, float z_
        , std::string shared_race_template_, uint64_t galactic_time_)
        : ignore_layout_files(ignore_layout_files_)
        , character_id(character_id_)
        , terrain_map(terrain_map_)
        , x(x_)
        , y(y_)
        , z(z_)
        , shared_race_template(shared_race_template_)
        , galactic_time(galactic_time_)
    {
    }
    uint8_t ignore_layout_files;
    uint64_t character_id;
    std::string terrain_map;
    float x;
    float y;
    float z;
    std::string shared_race_template;
    uint64_t galactic_time;

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<uint8_t>(ignore_layout_files);
        buffer.write<uint64_t>(character_id);
        buffer.write<std::string>(terrain_map);
        buffer.write<float>(x);
        buffer.write<float>(y);
        buffer.write<float>(z);
        buffer.write<std::string>(shared_race_template);
        buffer.write<uint64_t>(galactic_time);
    }
    void deserialize(anh::ByteBuffer& buffer) {
        // read off crc/operand count
        ignore_layout_files = buffer.read<uint8_t>();
        character_id = buffer.read<uint64_t>();
        terrain_map = buffer.read<std::string>();
        x = buffer.read<float>();
        y = buffer.read<float>();
        z = buffer.read<float>();
        shared_race_template = buffer.read<std::string>();
        galactic_time = buffer.read<uint64_t>();
    }

    static uint32_t crc() { return 0x3AE6DFAE; }
    static uint16_t operand_count() { return 8; }
};

} // packets

#endif // ANH_PACKETS_CMD_START_SCENE_H
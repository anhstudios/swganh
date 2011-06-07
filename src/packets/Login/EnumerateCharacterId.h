#ifndef ANH_PACKETS_ENUMERATE_CHARACTER_ID_H
#define ANH_PACKETS_ENUMERATE_CHARACTER_ID_H

#include <anh/byte_buffer.h>
#include <list>
#include <algorithm>

// Originates on Server
// http://wiki.swganh.org/index.php/EnumerateCharacterId
namespace packets {
struct Character
{
    std::wstring name;
    int32_t race_gender_crc;
    uint64_t character_id;
    uint32_t server_id;
    int32_t status;
};
struct EnumerateCharacterId
{
    EnumerateCharacterId(uint32_t character_count_ = 0,
        std::list<Character> characters_ =  std::list<Character>())
        : character_count(character_count_)
        , characters(characters_)
    {}
    explicit EnumerateCharacterId(anh::ByteBuffer buffer)
    {
        deserialize(buffer);
    }
    virtual ~EnumerateCharacterId() {}

    uint32_t character_count;
    std::list<Character> characters;

    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<uint32_t>(characters.size());
        std::for_each(characters.begin(), characters.end(), [&buffer] (Character character){
            buffer.write<std::wstring>(character.name);
            buffer.write<int32_t>(character.race_gender_crc);
            buffer.write<uint64_t>(character.character_id);
            buffer.write<uint32_t>(character.server_id);
            buffer.write<int32_t>(character.status);
        });
    }

    void deserialize(anh::ByteBuffer buffer) {
        character_count = buffer.read<uint32_t>();
        for (uint32_t i = 0; i < character_count; i++)
        {
            Character character;
            character.name = buffer.read<std::wstring>();
            character.race_gender_crc = buffer.read<int32_t>();
            character.character_id = buffer.read<uint64_t>();
            character.server_id = buffer.read<uint32_t>();
            character.status = buffer.read<int32_t>();
            characters.push_back(character);
        }
    }
    static uint16_t operand_count() { return 2; }
    static uint32_t crc() { return 0x65EA4574; }
};


} // packets

#endif // ANH_PACKETS_ENUMERATE_CHARACTER_ID_H
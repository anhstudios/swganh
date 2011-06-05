#ifndef ANH_PACKETS_ENUMERATE_CHARACTER_ID_H
#define ANH_PACKETS_ENUMERATE_CHARACTER_ID_H

#include <packets/BasePacket.h>
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

struct EnumerateCharacterId : public BasePacket
{
    EnumerateCharacterId(std::shared_ptr<network::Session> session_  = nullptr,
        std::list<Character> characters_ =  std::list<Character>())
        : BasePacket(session_, CLIENT)
        , characters(characters_)
    {}
    
    virtual ~EnumerateCharacterId() {}

    std::list<Character> characters;

    void serialize(anh::ByteBuffer& buffer) const {
		buffer.write<uint16_t>(2);
		buffer.write<uint32_t>(0x65EA4574);
        buffer.write<uint32_t>(characters.size());
        std::for_each(characters.begin(), characters.end(), [&buffer] (Character character){
            buffer.write<std::wstring>(character.name);
            buffer.write<uint32_t>(character.race_gender_crc);
            buffer.write<uint64_t>(character.character_id);
            buffer.write<uint32_t>(character.server_id);
            buffer.write<uint32_t>(character.status);
        });
    }

    void deserialize(anh::ByteBuffer buffer) {}
};

typedef anh::event_dispatcher::BasicEvent<EnumerateCharacterId> EnumerateCharacterIdEvent;

} // packets

#endif // ANH_PACKETS_ENUMERATE_CHARACTER_ID_H
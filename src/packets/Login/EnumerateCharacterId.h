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
    std::string name;
    int32_t race_gender_crc;
    uint64_t character_id;
    uint32_t server_id;
    int32_t status;
};
struct EnumerateCharacterId : public BasePacket
{
    EnumerateCharacterId(std::shared_ptr<network::Session> session_  = nullptr, uint32_t character_count_ = 0,
        std::list<Character> characters_ =  std::list<Character>())
        : BasePacket(session_, CLIENT)
        , character_count(character_count_)
        , characters(characters_)
    {}
    uint32_t character_count;
    std::list<Character> characters;
};

class EnumerateCharacterIdEvent : public anh::event_dispatcher::BasicEvent<EnumerateCharacterId>{
public:    
    EnumerateCharacterIdEvent(std::shared_ptr<network::Session> session_  = nullptr, uint32_t character_count_ = 0,
        std::list<Character> characters_ =  std::list<Character>()) 
        : anh::event_dispatcher::BasicEvent<EnumerateCharacterId>("EnumerateCharacterId"){}
    virtual ~EnumerateCharacterIdEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(characters.size());
        std::for_each(characters.begin(), characters.end(), [&buffer] (Character character){
            buffer.write<std::string>(character.name);
            buffer.write<int32_t>(character.race_gender_crc);
            buffer.write<uint64_t>(character.character_id);
            buffer.write<uint32_t>(character.server_id);
            buffer.write<int32_t>(character.status);
        });

    }
};

} // packets

#endif // ANH_PACKETS_ENUMERATE_CHARACTER_ID_H
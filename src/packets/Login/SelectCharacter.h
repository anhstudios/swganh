#ifndef ANH_PACKETS_SELECT_CHARACTER_H
#define ANH_PACKETS_SELECT_CHARACTER_H

#include <packets/BasePacket.h>

// Originates on Client [ C -> S ]
namespace packets {
struct SelectCharacter : public BasePacket
{
    SelectCharacter(std::shared_ptr<network::Session> session_  = nullptr, uint64_t character_id_ = 0 )
        : BasePacket(session_, NONE)
    , character_id(character_id_) {}
    uint64_t character_id;
};

class SelectCharacterEvent : public anh::event_dispatcher::BasicEvent<SelectCharacter>{
public:    
    SelectCharacterEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t character_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<SelectCharacter>("SelectCharacter"){}
    virtual ~SelectCharacterEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        character_id = buffer.read<uint64_t>();
    }
};

} // packets

#endif // ANH_PACKETS_SELECT_CHARACTER_H
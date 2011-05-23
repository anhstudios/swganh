#ifndef ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H
#define ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client
// http://wiki.swganh.org/index.php/DeleteCharacterMessage
namespace packets {
struct DeleteCharacterMessage : public BasePacket
{
    explicit DeleteCharacterMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, NONE){}
    int32_t server_id;
    uint64_t character_id;
};

class DeleteCharacterMessageEvent : public anh::event_dispatcher::BasicEvent<DeleteCharacterMessage>{
public:    
    DeleteCharacterMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<DeleteCharacterMessage>("DeleteCharacterMessage"){}
    virtual ~DeleteCharacterMessageEvent() {}
    void deserialize(anh::ByteBuffer& buffer) {
        server_id = buffer.read<int32_t>();
        character_id = buffer.read<uint64_t>();
    }
};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_MESSAGE_H
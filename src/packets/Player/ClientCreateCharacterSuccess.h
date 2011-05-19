#ifndef ANH_PACKETS_CLIENT_CREATE_CHARACTER_SUCCESS_H
#define ANH_PACKETS_CLIENT_CREATE_CHARACTER_SUCCESS_H

#include <packets/BasePacket.h>

// Originates on server
namespace packets {
struct ClientCreateCharacterSuccess : public BasePacket
{
    ClientCreateCharacterSuccess(std::shared_ptr<network::Session> session_  = nullptr, uint64_t character_id_ = 0)
        : BasePacket(session_, CLIENT)
        , character_id(character_id_)
    {}
    uint64_t character_id;
};

class ClientCreateCharacterSuccessEvent : public anh::event_dispatcher::BasicEvent<ClientCreateCharacterSuccess>{
public:    
    ClientCreateCharacterSuccessEvent(std::shared_ptr<network::Session> session_  = nullptr, uint64_t character_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<ClientCreateCharacterSuccess>("ClientCreateCharacterSuccess"){}
    virtual ~ClientCreateCharacterSuccessEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(character_id);
    }
};

} // packets

#endif // ANH_PACKETS_CLIENT_CREATE_CHARACTER_SUCCESS_H
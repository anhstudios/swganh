#ifndef ANH_PACKETS_VERIFY_PLAYER_RESPONSE_MESSAGE_H
#define ANH_PACKETS_VERIFY_PLAYER_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct VerifyPlayerNameResponseMessage  : public BasePacket
{
    VerifyPlayerNameResponseMessage (std::shared_ptr<network::Session> session_  = nullptr, uint8_t result_ = 0
        , uint32_t null_ = 0)
        : BasePacket(session_, CLIENT)
        , result(result_)
        , null(null_)
        {}
    uint8_t result;
    uint32_t null;
};

class VerifyPlayerNameResponseMessageEvent : public anh::event_dispatcher::BasicEvent<VerifyPlayerNameResponseMessage>{
public:    
    VerifyPlayerNameResponseMessageEvent(std::shared_ptr<network::Session> session_  = nullptr, uint8_t result_ = 0
        , uint32_t null_ = 0 
        : anh::event_dispatcher::BasicEvent<VerifyPlayerNameResponseMessage>("VerifyPlayerNameResponseMessage"){}
    virtual ~VerifyPlayerNameResponseMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint8_t>(result);
        buffer.write<uint32_t>(null);
    }
};

} // packets

#endif // ANH_PACKETS_VERIFY_PLAYER_RESPONSE_MESSAGE_H
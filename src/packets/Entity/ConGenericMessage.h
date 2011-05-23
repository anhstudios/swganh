#ifndef ANH_PACKETS_CON_GENERIC_MESSAGE_H
#define ANH_PACKETS_CON_GENERIC_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
namespace packets {
struct ConGenericMessage : public BasePacket
{
    ConGenericMessage()
        : BasePacket() {}
    explicit ConGenericMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& message_ = std::string("")
        , uint32_t spacer_ = 0 )
    : BasePacket(session_, CLIENT)
    , message(message_)
    , spacer(spacer_)
    {}
    std::string message;
    uint32_t spacer;
};

class ConGenericMessageEvent : public anh::event_dispatcher::BasicEvent<ConGenericMessage>{
public:    
    ConGenericMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& message_ = std::string("")
        , uint32_t spacer_ = 0) : anh::event_dispatcher::BasicEvent<ConGenericMessage>("ConGenericMessage"){}
    virtual ~ConGenericMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(message);
        buffer.write<uint32_t>(spacer);
    }
};

} // packets

#endif // ANH_PACKETS_CON_GENERIC_MESSAGE_H
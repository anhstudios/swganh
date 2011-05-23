#ifndef ANH_PACKETS_ERROR_MESSAGE_H
#define ANH_PACKETS_ERROR_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ]
// http://wiki.swganh.org/index.php/DeleteCharacterReplyMessage
namespace packets {
struct ErrorMessage : public BasePacket
{
    ErrorMessage(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& error_type_ = std::string()
        , std::string& error_message_ = std::string(), uint8_t force_fatal_ = 0)
        : BasePacket(session_, CLIENT) 
        , error_type(error_type_)
        , error_message(error_message_)
        , force_fatal(force_fatal_) {}
    std::string error_type;
    std::string error_message;
    uint8_t force_fatal;
};
class ErrorMessageEvent : public anh::event_dispatcher::BasicEvent<ErrorMessage>{
public:    
    ErrorMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, std::string& error_type_ = std::string()
        , std::string& error_message_ = std::string(), uint8_t force_fatal_ = 0) : anh::event_dispatcher::BasicEvent<ErrorMessage>("ErrorMessage"){}
    virtual ~ErrorMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<std::string>(error_type);
        buffer.write<std::string>(error_message);
        buffer.write<uint8_t>(force_fatal);
    }
};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
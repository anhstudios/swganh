#ifndef ANH_PACKETS_ERROR_MESSAGE_H
#define ANH_PACKETS_ERROR_MESSAGE_H

#include <anh/byte_buffer.h>

// Originates on Server [ S -> C ]
namespace packets {
struct ErrorMessage
{
    ErrorMessage(anh::ByteBuffer& buffer)
    {
        deserialize(buffer);
    }
    ErrorMessage(std::string error_type_ = "", std::string error_message_ = "", char force_fatal_ = 0)
        : error_type(error_type_)
        , error_message(error_message_)
        , force_fatal(force_fatal_) {}
    virtual ~ErrorMessage() {}

    std::string error_type;
    std::string error_message;
    char force_fatal;

    void serialize(anh::ByteBuffer& buffer) const
    {
        buffer.write<uint16_t>(operand_count());
        buffer.write<uint32_t>(crc());
        buffer.write<std::string>(error_type);
        buffer.write<std::string>(error_message);
        buffer.write<char>(force_fatal);
    }
    void deserialize(anh::ByteBuffer buffer)
    {
        error_type = buffer.read<std::string>();
        error_message = buffer.read<std::string>();
        force_fatal = buffer.read<char>();
    }
    static uint16_t operand_count() { return 3; }
    static uint32_t crc() { return 0xB5ABF91A; }
};

} // packets

#endif // ANH_PACKETS_DELETE_CHARACTER_REPLY_MESSAGE_H
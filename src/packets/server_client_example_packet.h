#ifndef ANH_PACKETS_CLIENT_SERVER_EXAMPLE_MESSAGE_H
#define ANH_PACKETS_CLIENT_SERVER_EXAMPLE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server [ S -> C ] or Client [ C -> S ]
namespace packets {
struct ClientServerExampleMessage : public BasePacket
{
    ClientServerExampleMessage(std::shared_ptr<anh::network::soe::Session> session_ )
        : BasePacket(session_, ZONE){}
    ClientServerExampleMessage(std::shared_ptr<anh::network::soe::Session> session_ = nullptr, std::string ASCII_ = std::string()
	, std::wstring UNICODE_ = std::wstring(), uint8_t byte_ = 0, uint16_t positive_SHORT_ = 0, int16_t negative_SHORT_ = 0, uint32_t positive_INT_ = 0, int32_t negative_INT_ = 0, uint64_t positive_LONG_ = 0, int64_t negative_LONG_ = 0, float 	float_var_ = 0.0f)
        : BasePacket(session_, CLIENT)
		, ASCII(ASCII_)
		, UNICODE(UNICODE_)
		, byte(byte_)
		, positive_SHORT(positive_SHORT_)
		, negative_SHORT(negative_SHORT_)
		, positive_INT(positive_INT_)
		, negative_INT(negative_INT_)
		, positive_LONG(positive_LONG_)
		, negative_LONG(negative_LONG_)
		, float_var(float_var_)
        {}
		
	std::string	ASCII;
	std::wstring UNICODE;
	uint8_t byte;
	uint16_t positive_SHORT;
	int16_t negative_SHORT;
    uint32_t positive_INT;
	int32_t negative_INT;
	uint64_t positive_LONG;
    int64_t negative_LONG;
	float float_var;
};

class ClientServerExampleMessageEvent : public anh::event_dispatcher::BasicEvent<ClientServerExampleMessage>{
public:    
    ClientServerExampleMessageEvent(std::shared_ptr<anh::network::soe::Session> session_ = nullptr, std::string ASCII_ = std::string()
	, std::wstring UNICODE_ = std::wstring(), uint8_t byte_ = 0, uint16_t positive_SHORT_ = 0, int16_t negative_SHORT_ = 0, uint32_t positive_INT_ = 0, int32_t negative_INT_ = 0, uint64_t positive_LONG_ = 0, int64_t negative_LONG_ = 0, float 	float_var_ = 0.0f) 
	: anh::event_dispatcher::BasicEvent<ClientServerExampleMessage>("ClientServerExampleMessage"){}
    virtual ~ClientServerExampleMessageEvent() {}
	void serializer(anh::ByteBuffer& buffer) {
		buffer.write<std::string>(ASCII);
		buffer.write<std::wstring>(UNICODE);
		buffer.write<uint8_t>(byte);
		buffer.write<uint16_t>(positive_SHORT);
		buffer.write<int16_t>(negative_SHORT);
		buffer.write<uint32_t>(positive_INT);
		buffer.write<int32_t>(negative_INT);
		buffer.write<uint64_t>(positive_LONG);
		buffer.write<int64_t>(negative_LONG);
		buffer.write<float>(float_var);
	}
	void deserialize(anh::ByteBuffer& buffer) {
		ASCII = buffer.read<std::string>();
		UNICDOE = buffer.read<std::wstring>();
		byte = buffer.read<uint8_t>();
		positive_SHORT = buffer.read<uint16_t>();
		negative_SHORT = buffer.read<int16_t>();
		positive_INT = buffer.read<uint32_t>();
		negative_INT = buffer.read<int32_t>();
		positive_LONG = buffer.read<uint64_t>();
		negative_LONG = buffer.read<int64_t>();
		float_var = buffer.read<float>();
	}
};

} // packets

#endif // ANH_PACKETS_CLIENT_SERVER_EXAMPLE_MESSAGE_H
#ifndef ANH_PACKETS_SERVER_ONLY_EXAMPLE_MESSAGE_H
#define ANH_PACKETS_SERVER_ONLY_EXAMPLE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct ServerOnlyExampleMessage  : public BasePacket
{
    ServerOnlyExampleMessage (std::shared_ptr<anh::network::soe::Session> session_ = nullptr, std::string ASCII_ = std::string(), std::wstring UNICODE_ = std::wstring(), uint8_t byte_ = 0
							 , uint16_t positive_SHORT_ = 0, int16_t negative_SHORT_ = 0, uint32_t positive_INT_ = 0, int32_t negative_INT_ = 0
							 , uint64_t positive_LONG_ = 0, int64_t negative_LONG_ = 0, float float_var_ = 0.0f)
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
	int64_t positive_LONG;
    int64_t negative_LONG;
	float float_var;
};

class ServerOnlyExampleMessageEvent : public anh::event_dispatcher::BasicEvent<ServerOnlyExampleMessage >{
public:    
    ServerOnlyExampleMessageEvent(std::shared_ptr<anh::network::soe::Session> session_ = nullptr, std::string ASCII_ = std::string(),
	std::wstring UNICODE_ = std::wstring(), uint8_t byte_ = 0, uint16_t positive_SHORT_ = 0, int16_t negative_SHORT_ = 0, uint32_t positive_INT_ = 0, int32_t negative_INT_ = 0, uint64_t positive_LONG_ = 0, int64_t negative_LONG_ = 0, float float_var_ = 0.0f) 
	 : anh::event_dispatcher::BasicEvent<ServerOnlyExampleMessage >("ServerOnlyExampleMessage "){}
    virtual ~ServerOnlyExampleMessageEvent() {}
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
};

} // packets

#endif // ANH_PACKETS_SERVER_ONLY_EXAMPLE_MESSAGE_H
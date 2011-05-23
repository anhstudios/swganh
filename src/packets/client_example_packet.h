#ifndef ANH_PACKETS_CLIENT_ONLY_EXAMPLE_MESSAGE_H
#define ANH_PACKETS_CLIENT_ONLY_EXAMPLE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client
namespace packets {
struct ClientOnlyExampleMessage : public BasePacket
{
    ClientOnlyExampleMessage(std::shared_ptr<anh::network::soe::Session> session_ = nullptr)
        : BasePacket(session_, ZONE){}
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

class ClientOnlyExampleMessageEvent : public anh::event_dispatcher::BasicEvent<ClientOnlyExampleMessage>{
public:    
    ClientOnlyExampleMessageEvent(std::shared_ptr<anh::network::soe::Session> session_ = nullptr) 
	: anh::event_dispatcher::BasicEvent<ClientOnlyExampleMessage>("ClientOnlyExampleMessage"){}
    virtual ~ClientOnlyExampleMessageEvent() {}
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

#endif // ANH_PACKETS_CLIENT_ONLY_EXAMPLE_MESSAGE_H
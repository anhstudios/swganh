#ifndef ANH_PACKETS_IS_VENDOR_MESSAGE_H
#define ANH_PACKETS_IS_VENDOR_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Client
namespace packets {
struct IsVendorMessage : public BasePacket
{
    explicit IsVendorMessage(std::shared_ptr<network::Session> session_  = nullptr)
        : BasePacket(session_, ZONE){}
	int64_t terminal_id;
    
};

class IsVendorMessageEvent : public anh::event_dispatcher::BasicEvent<IsVendorMessage>{
public:    
    IsVendorMessageEvent(std::shared_ptr<network::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<IsVendorMessage>("IsVendorMessage"){}
    virtual ~IsVendorMessageEvent() {}
	void deserialize(anh::ByteBuffer& buffer) {
		terminal_id = buffer.read<uint64_t>();
	}
};

} // packets

#endif // ANH_PACKETS_IS_VENDOR_MESSAGE_H
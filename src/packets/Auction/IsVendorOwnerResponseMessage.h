#ifndef ANH_PACKETS_IS_VENDOR_OWNER_RESPONSE_MESSAGE_H
#define ANH_PACKETS_IS_VENDOR_OWNER_RESPONSE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct IsVendorOwnerResponseMessage  : public BasePacket
{
    IsVendorOwnerResponseMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t permission_flag_ = 0, uint32_t error_message_ = 0
        , uint64_t terminal_id_ = 0, std::string& vendor_info_ = std::string(""), uint16_t unknown_ = 0x64)
        : BasePacket(session_, CLIENT)
		, permission_flag(permission_flag_)
        , error_message(error_message_)
        , terminal_id(terminal_id_)
        , vendor_info(vendor_info_)
        , unknown(unknown_)
        {}
	uint32_t permission_flag;
	uint32_t error_message;
	uint64_t terminal_id;
    std::string	vendor_info;
    uint16_t unknown;
};

class IsVendorOwnerResponseMessageEvent : public anh::event_dispatcher::BasicEvent<IsVendorOwnerResponseMessage >{
public:    
    IsVendorOwnerResponseMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t permission_flag_ = 0, uint32_t error_message_ = 0
        , uint64_t terminal_id_ = 0, std::string& vendor_info_ = std::string(""), uint16_t unknown_ = 0x64) 
        : anh::event_dispatcher::BasicEvent<IsVendorOwnerResponseMessage >("IsVendorOwnerResponseMessage"){}
    virtual ~IsVendorOwnerResponseMessageEvent() {}
	void serializer(anh::ByteBuffer& buffer) {
		buffer.write<uint32_t>(permission_flag);
        buffer.write<uint32_t>(error_message);
        buffer.write<uint64_t>(terminal_id);
        buffer.write<std::string>(vendor_info);
        buffer.write<uint16_t>(unknown);
	}
};

} // packets

#endif // ANH_PACKETS_IS_VENDOR_OWNER_RESPONSE_MESSAGE_H
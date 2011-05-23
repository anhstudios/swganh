#ifndef ANH_PACKETS_OPEN_HOLOCRON_TO_PAGE_MESSAGE_H
#define ANH_PACKETS_OPEN_HOLOCRON_TO_PAGE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct OpenHolocronToPageMessage  : public BasePacket
{
    OpenHolocronToPageMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr)
        : BasePacket(session_, CLIENT)
        {}
};

class OpenHolocronToPageMessageEvent : public anh::event_dispatcher::BasicEvent<OpenHolocronToPageMessage >{
public:    
    OpenHolocronToPageMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr) 
        : anh::event_dispatcher::BasicEvent<OpenHolocronToPageMessage >("OpenHolocronToPageMessage"){}
    virtual ~OpenHolocronToPageMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        // doesn't seem to affect anything
        buffer.write<uint16_t>(0x00);
    }
};

} // packets

#endif // ANH_PACKETS_OPEN_HOLOCRON_TO_PAGE_MESSAGE_H
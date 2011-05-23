#ifndef ANH_PACKETS_SUI_FORCE_CLOSE_PAGE_H
#define ANH_PACKETS_SUI_FORCE_CLOSE_PAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct SuiForceClosePage  : public BasePacket
{
    SuiForceClosePage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t window_id_ = 0)
        : BasePacket(session_, CLIENT)
        , window_id(window_id_)
        {}
    uint32_t window_id;
};

class SuiForceClosePageEvent : public anh::event_dispatcher::BasicEvent<SuiForceClosePage>{
public:    
    SuiForceClosePageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint32_t window_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<SuiForceClosePage>("SuiForceClosePage"){}
    virtual ~SuiForceClosePageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(window_id);
    }
};

} // packets

#endif // ANH_PACKETS_SUI_FORCE_CLOSE_PAGE_H
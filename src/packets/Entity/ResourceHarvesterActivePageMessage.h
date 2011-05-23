#ifndef ANH_PACKETS_RESOURCE_HARVESTER_ACTIVATE_PAGE_MESSAGE_H
#define ANH_PACKETS_RESOURCE_HARVESTER_ACTIVATE_PAGE_MESSAGE_H

#include <packets/BasePacket.h>

// Originates on Server
namespace packets {
struct ResourceHarvesterActivatePageMessage  : public BasePacket
{
    ResourceHarvesterActivatePageMessage (std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t harvester_id_ = 0)
        : BasePacket(session_, CLIENT)
        , harvester_id(harvester_id_)
        {}
    uint64_t harvester_id;
};

class RemoveItemMessageEvent : public anh::event_dispatcher::BasicEvent<ResourceHarvesterActivatePageMessage >{
public:    
    RemoveItemMessageEvent(std::shared_ptr<anh::network::soe::Session> session_  = nullptr, uint64_t object_id_ = 0) 
        : anh::event_dispatcher::BasicEvent<ResourceHarvesterActivatePageMessage >("ResourceHarvesterActivatePageMessage"){}
    virtual ~RemoveItemMessageEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint64_t>(harvester_id);
    }
};

} // packets

#endif // ANH_PACKETS_RESOURCE_HARVESTER_ACTIVATE_PAGE_MESSAGE_H
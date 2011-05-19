#ifndef ANH_PACKETS_SUI_EVENT_NOTIFICATION_H
#define ANH_PACKETS_SUI_EVENT_NOTIFICATION_H

#include <packets/BasePacket.h>
#include <algorithm>
#include <list>

// Originates on Server [ S -> C ] or Client [ C -> S ]
namespace packets {
struct SuiEventNotification : public BasePacket
{
    SuiEventNotification(std::shared_ptr<network::Session> session_  = nullptr, uint32_t window_id_ = 0
        , uint32_t action_ = 0, std::list<std::wstring> arguments_ = std::list<std::wstring>())
        : BasePacket(session_, ZONE)
    , window_id(window_id_)
    , action(action_)
    , list_size1(0)
    , list_size2(0)
    , arguments(arguments_)
    {}
    SuiEventNotification(std::shared_ptr<network::Session> session_, RoutingDestination dest)
        : BasePacket(session_, dest){}
    uint32_t window_id;
    uint32_t action;
    uint32_t list_size1;
    uint32_t list_size2;
    std::list<std::wstring> arguments;
};

class SuiEventNotificationEvent : public anh::event_dispatcher::BasicEvent<SuiEventNotification>{
public:    
    SuiEventNotificationEvent(std::shared_ptr<network::Session> session_  = nullptr, uint32_t window_id_ = 0
        , uint32_t action_ = 0, std::list<std::wstring> arguments_ = std::list<std::wstring>()) 
        : anh::event_dispatcher::BasicEvent<SuiEventNotification>("SuiEventNotification"){}
    virtual ~SuiEventNotificationEvent() {}
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint32_t>(window_id);
        buffer.write<uint32_t>(action);
        buffer.write<uint32_t>(arguments.size());
        buffer.write<uint32_t>(list_size2);
        std::for_each(arguments.begin(), arguments.end(), [&buffer] (std::wstring arg) {
            buffer.write<std::wstring>(arg);
        });
    }
    void deserialize(anh::ByteBuffer& buffer) { 
        window_id = buffer.read<uint32_t>();
        action = buffer.read<uint32_t>();
        list_size1 = buffer.read<uint32_t>();
        list_size2 = buffer.read<uint32_t>();
        for (int i = 0; i< list_size1; ++i) {
            arguments.push_back(buffer.read<std::wstring>());
        }
    }
};

} // packets

#endif // ANH_PACKETS_SUI_EVENT_NOTIFICATION_H
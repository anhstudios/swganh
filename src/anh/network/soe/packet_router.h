#ifndef ANH_NETWORK_SOE_PACKET_ROUTER_H_
#define ANH_NETWORK_SOE_PACKET_ROUTER_H_

#include <exception>
#include <functional>
#include <memory>

#include "anh/event_dispatcher/basic_event.h"

namespace anh {
namespace network {
namespace soe {

/**
 * Utility class that routes an incoming message to the appropriate client handler.
 */
template<typename ClientMap>
class PacketRouter {
typedef typename ClientMap::mapped_type ClientType;    
    
public:
    PacketRouter(ClientMap& client_map)
        : clients_(client_map) {}
        
    template<typename MessageType> 
    bool RoutePacket(
        std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event,
        std::function<void (ClientType, const MessageType&)> handler)
    {
        // @TODO: Make a safe_pointer_cast that does a dynamic cast and check in
        // debug and a static in release.
        auto remote_event = std::static_pointer_cast<anh::event_dispatcher::BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
        auto it = clients_.find(remote_event->session()->remote_endpoint());
        
        if (it == clients_.end()) {
            throw std::runtime_error("Received remote message for an invalid client");
        }
    
        ClientType client = it->second;
        
        try {
            MessageType message;
            message.deserialize(*remote_event->message());        
            
            handler(client, message);
        } catch(const std::exception& e) {
            DLOG(ERROR) << "Error handling remote message\nclient: " << remote_event->session()->remote_endpoint() << "\nerror: \n" << e.what();
            return false;
        }

        return true;  
    }
    
private:    
    ClientMap& clients_;
};

}}}  // namespace anh::network::soe

#endif  // ANH_NETWORK_SOE_PACKET_ROUTER_H_

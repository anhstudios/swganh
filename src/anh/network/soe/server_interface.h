#ifndef ANH_NETWORK_SERVER_INTERFACE_H_
#define ANH_NETWORK_SERVER_INTERFACE_H_

#include <memory>

#ifdef SendMessage
#undef SendMessage
#endif

namespace anh {
class ByteBuffer;
}  // namespace anh

namespace anh {
namespace event_dispatcher {
class EventDispatcherInterface;
}}  // namespace anh::event_dispatcher

namespace anh {
namespace network {
namespace soe {

class Session;
class SessionManager;
class Socket;

class ServerInterface {
public:
    virtual ~ServerInterface() {}
    
    virtual void Start(uint16_t port) = 0;
    
    virtual void Shutdown(void) = 0;

    virtual void SendMessage(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> outgoing_packet) = 0;

    virtual std::shared_ptr<anh::ByteBuffer> AllocateBuffer() = 0;

    virtual void HandleMessage(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> message) = 0;
    
    virtual std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher() = 0;
    
    virtual SessionManager& session_manager() = 0;
    
    virtual std::shared_ptr<Socket> socket() = 0;

    virtual uint32_t max_receive_size() = 0;
};

}}} // namespace anh::network::soe

#endif  // ANH_NETWORK_SERVER_INTERFACE_H_
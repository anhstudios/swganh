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

    virtual void SendTo(const boost::asio::ip::udp::endpoint& endpoint, anh::ByteBuffer buffer) = 0;

    virtual std::shared_ptr<anh::ByteBuffer> AllocateBuffer() = 0;
    
    virtual void HandleMessage(
        const std::shared_ptr<Session>& connection, 
        anh::ByteBuffer message) = 0;
    
    virtual bool RemoveSession(std::shared_ptr<Session> session) = 0;
    
    virtual std::shared_ptr<Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;
    
    virtual std::shared_ptr<Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;
    
    virtual boost::asio::ip::udp::socket* socket() = 0;

    virtual uint32_t max_receive_size() = 0;
};

}}} // namespace anh::network::soe

#endif  // ANH_NETWORK_SERVER_INTERFACE_H_
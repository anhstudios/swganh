// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>

#ifdef SendMessage
#undef SendMessage
#endif

namespace swganh {
class ByteBuffer;
}  // namespace swganh

namespace swganh {
namespace event_dispatcher {
class EventDispatcherInterface;
}}  // namespace swganh::event_dispatcher

namespace swganh {
namespace network {
namespace soe {

class Session;
class SessionManager;
class Socket;

class ServerInterface {
public:
    virtual ~ServerInterface() {}
    
    virtual void Startup(uint16_t port) = 0;
    
    virtual void Shutdown(void) = 0;

    virtual void SendTo(const boost::asio::ip::udp::endpoint& endpoint, swganh::ByteBuffer buffer) = 0;

    virtual void HandleMessage(
        const std::shared_ptr<Session>& connection, 
        swganh::ByteBuffer message) = 0;
    
    virtual bool RemoveSession(std::shared_ptr<Session> session) = 0;
    
    virtual std::shared_ptr<Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;
    
    virtual std::shared_ptr<Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;
    
    virtual boost::asio::ip::udp::socket* socket() = 0;

    virtual uint32_t max_receive_size() = 0;
};

}}} // namespace swganh::network::soe
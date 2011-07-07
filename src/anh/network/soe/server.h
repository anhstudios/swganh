/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef ANH_NETWORK_SOE_SERVER_H_
#define ANH_NETWORK_SOE_SERVER_H_

#include <cstdint>
#include <list>
#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <tbb/pipeline.h>

#include "anh/network/soe/server_interface.h"
#include "anh/network/soe/session_manager.h"

#ifdef SendMessage
#undef SendMessage
#endif

namespace anh {

class ByteBuffer;

namespace event_dispatcher {
class EventDispatcherInterface;
}

namespace network {
namespace soe {

// FORWARD DECLARATION
class Packet;
class Session;
class Socket;

typedef std::function<void (std::shared_ptr<Session>, std::shared_ptr<anh::ByteBuffer>)> MessageHandler;

/**
 * @brief An SOE Protocol Service.
 *
 * 
 */
class Server : public std::enable_shared_from_this<Server>, public ServerInterface {
public:
    explicit Server(MessageHandler message_handler);
    ~Server(void);

    /**
     * @brief Starts the SOE Frontend Service.
     * 
     * @parama port The port to listen for messages on.
     */
    void Start(uint16_t port);

    /**
     * @brief Performs any work the pipelines have waiting and receives and sends
     * any waiting messages. (Blocking)
     */
    void Update(void);
    
    /**
     * @brief
     */
    void Shutdown(void);
    
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher();
    void event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);

    void SendMessage(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> message);
    
    void HandleMessage(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> message);
    
    SessionManager& session_manager();

    std::shared_ptr<Socket> socket();
    
    std::shared_ptr<ByteBuffer> AllocateBuffer();
    
private:
    Server();

    /**
     * @brief Called when the socket receives a message.
     */
    void OnSocketRecv_(boost::asio::ip::udp::endpoint remote_endpoint, std::shared_ptr<anh::ByteBuffer> message);

    std::shared_ptr<Socket>		socket_;
    boost::asio::io_service		io_service_;

    SessionManager				session_manager_;
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
    uint32_t					crc_seed_;
    
    tbb::filter_t<void, void>   incoming_filter_;
    tbb::filter_t<void, void>   outgoing_filter_;

    std::list<std::shared_ptr<Packet>> incoming_messages_;
    std::list<std::shared_ptr<Packet>> outgoing_messages_;

    MessageHandler              message_handler_;
};

}}} // namespace anh::network::soe

#endif // ANH_NETWORK_SOE_SERVER_H_

/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef ANH_NETWORK_SOE_SERVICE_H_
#define ANH_NETWORK_SOE_SERVICE_H_

#include <cstdint>
#include <list>
#include <map>
#include <memory>

#include <boost/asio.hpp>
#include <tbb/pipeline.h>

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
class IncomingPacket;
class IncomingSessionlessPacket;
class OutgoingPacket;
class Session;
class Socket;

class ServiceInterface {
public:
    virtual ~ServiceInterface() {}
    
    virtual void Start(uint16_t port) = 0;

    virtual void Update(void) = 0;
    
    virtual void Shutdown(void) = 0;

    virtual void SendMessage(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> outgoing_packet) = 0;
        
    virtual std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher() = 0;

    virtual SessionManager& session_manager() = 0;

    virtual std::shared_ptr<Socket> socket() = 0;
};

/**
 * @brief An SOE Protocol Service.
 *
 * 
 */
class Service : public std::enable_shared_from_this<Service>, public ServiceInterface {
public:
    Service(void);
    ~Service(void);

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

    void SendMessage(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> outgoing_packet);
    
    SessionManager& session_manager();

    std::shared_ptr<Socket> socket();

private:
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
    tbb::filter_t<void, void>   sessionless_filter_;

    std::list<std::shared_ptr<IncomingSessionlessPacket>> sessionless_messages_;
    std::list<std::shared_ptr<IncomingPacket>> incoming_messages_;
    std::list<std::shared_ptr<OutgoingPacket>> outgoing_messages_;
};

} // namespace soe
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_SOE_SERVER_H_
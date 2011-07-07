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

#ifndef ANH_NETWORK_SOE_MOCK_SERVER_H_
#define ANH_NETWORK_SOE_MOCK_SERVER_H_

#include <gmock/gmock.h>
#include "anh/network/soe/server.h"
#include "anh/network/soe/session_manager.h"

namespace anh{

class ByteBuffer;

namespace event_dispatcher {
class EventDispatcherInterface;
}
    
namespace network {
namespace soe {
    
class Session;
class Socket;
  
class MockServer : public ServerInterface {
public:
    MOCK_METHOD1(Start, void(uint16_t port));
    MOCK_METHOD0(Update, void());
    MOCK_METHOD0(Shutdown, void());
    MOCK_METHOD2(SendMessage, void(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> message));
    MOCK_METHOD2(HandleMessage, void(std::shared_ptr<Session> session, std::shared_ptr<anh::ByteBuffer> message));
    MOCK_METHOD0(event_dispatcher, std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface>());
    MOCK_METHOD0(session_manager, SessionManager&());
    MOCK_METHOD0(socket, std::shared_ptr<Socket>());
    MOCK_METHOD0(AllocateBuffer, std::shared_ptr<anh::ByteBuffer>());
};
    
}}}  // namespace anh::network::soe

#endif  // ANH_NETWORK_SOE_MOCK_SERVER_H_
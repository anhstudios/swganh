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

namespace anh{

class ByteBuffer;
 
namespace network {
namespace soe {
    
class Session;
  
class MockServer : public ServerInterface {
public:
    MOCK_METHOD1(Start, void(uint16_t port));
    MOCK_METHOD0(Shutdown, void());
    MOCK_METHOD2(SendTo, void(const boost::asio::ip::udp::endpoint& endpoint, anh::ByteBuffer buffer));
    MOCK_METHOD0(AllocateBuffer, std::shared_ptr<anh::ByteBuffer>());
    MOCK_METHOD2(HandleMessage, void(const std::shared_ptr<Session>& connection, anh::ByteBuffer message));    

    MOCK_METHOD1(RemoveSession, bool(std::shared_ptr<Session> session));
    MOCK_METHOD1(CreateSession, std::shared_ptr<Session>(const boost::asio::ip::udp::endpoint& endpoint));
    MOCK_METHOD1(GetSession, std::shared_ptr<Session>(const boost::asio::ip::udp::endpoint& endpoint));

    MOCK_METHOD0(socket, boost::asio::ip::udp::socket*());
    MOCK_METHOD0(max_receive_size, uint32_t());
};
    
}}}  // namespace anh::network::soe

#endif  // ANH_NETWORK_SOE_MOCK_SERVER_H_

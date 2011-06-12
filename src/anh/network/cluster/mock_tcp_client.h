/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gmock/gmock.h>
#include <anh/network/cluster/tcp_client.h>

namespace anh {
namespace network {
namespace cluster {
class MockTCPClient : public tcp_client
{
public:
    MOCK_METHOD1(Send, void(anh::ByteBuffer& buffer));
    MOCK_METHOD0(bytes_recv, const uint64_t());
    MOCK_METHOD0(bytes_sent, const uint64_t());
};
}
}
}
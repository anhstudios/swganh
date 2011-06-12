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
#include <anh/network/cluster/cluster_service_interface.h>

namespace anh {
namespace network {
namespace cluster {
class MockClusterService : public ClusterServiceInterface
{
public:
    MOCK_METHOD0(Start, void());
    MOCK_METHOD1(Start, void(std::shared_ptr<tcp_host>));
    MOCK_METHOD0(Update, void());
    MOCK_METHOD0(Shutdown, void());
    MOCK_METHOD1(Connect, void(std::shared_ptr<anh::server_directory::Process>));
    MOCK_METHOD2(Connect, void(std::shared_ptr<tcp_client>, std::shared_ptr<anh::server_directory::Process>));
    MOCK_METHOD1(Disconnect, void (std::shared_ptr<anh::server_directory::Process> process));
    MOCK_METHOD3(sendMessage, void(const std::string& name, std::shared_ptr<anh::event_dispatcher::EventInterface> event_out, DestinationType dest));
    MOCK_METHOD4(sendMessage, void(const std::string& host, uint16_t port, anh::ByteBuffer& buffer, DestinationType dest));
    MOCK_METHOD1(isConnected, bool(const std::string& name));
    MOCK_METHOD1(isConnected, bool(std::shared_ptr<anh::server_directory::Process> process));
    MOCK_METHOD2(getConnection, std::shared_ptr<tcp_client>(const std::string& host, uint16_t port));
    MOCK_METHOD1(getConnection, std::shared_ptr<tcp_client>(const std::string& name));
    MOCK_METHOD0(host, std::shared_ptr<tcp_host>());

};
}
}
}
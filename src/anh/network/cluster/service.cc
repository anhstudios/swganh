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
#include <iostream>
#include <anh/byte_buffer.h>

#include <boost/bind.hpp>
#include <anh/network/cluster/service.h>

#include <glog/logging.h>

using namespace anh::server_directory;
using namespace std;

namespace anh {
namespace network {
namespace cluster {

Service::Service(boost::asio::io_service& io_service, shared_ptr<ServerDirectoryInterface> directory)
    : directory_(directory)
    , io_service_(io_service)
    , resolver_(io_service)
{
    //proc_list_ = directory_->getProcessSnapshot(directory_->cluster());
}

Service::~Service(void)
{	
    Shutdown();
}
void Service::Connect(const std::string& host, const std::string& port)
{
    try {
        auto client = std::make_shared<tcp_client>(io_service_, host, port);
        tcp_client_map_.insert(ClientPair(std::string(host+":"+port), client));
    }
    catch (exception e)
    {
        std::cout << "Exception in Service::Connect " << e.what() << std::endl;
    }
}
void Service::SendMessage(const std::string& host_and_port, anh::ByteBuffer& buffer)
{
    auto iter = tcp_client_map_.find(host_and_port);
    if (iter != tcp_client_map_.end())
    {
        std::string host = host_and_port.substr(host_and_port.front(), host_and_port.find(":"));
        std::string port = host_and_port.substr(host_and_port.find(":"), host_and_port.back());
        Connect(host, port);
    }
    
}
void Service::Start(uint16_t port)
{
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_service_, tcp::endpoint(tcp::v4(), port ));
    tcp_host_ = std::make_shared<tcp_host>(io_service_);

    acceptor_->async_accept(tcp_host_->socket(), boost::bind(&Service::handle_accept_, this, 
       tcp_host_ , boost::asio::placeholders::error));

    // open up a connection for each cluster
    //std::for_each(proc_list_.begin(), proc_list_.end(), [&] (anh::server_directory::Process process) {
    //    auto conn = std::make_shared<TCPConnection>(io_service_);
    //    // create endpoint
    //    auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4(), process.tcp_port());
    //    // open connection to endpoint
    //    conn->socket().connect(endpoint);
    //    // insert into a map for later use...
    //    socket_map_.insert(SocketPair(endpoint, conn->socket()));
    //});
}

void Service::Update(void)
{
    io_service_.poll();
}

void Service::Shutdown(void)
{
    io_service_.reset();
    resolver_.cancel();
    acceptor_->close();
}

void Service::handle_accept_(std::shared_ptr<tcp_host> host, const boost::system::error_code& error)
{
    if (!error)
    {
        std::cout << "handle accept" << std::endl;
        host->Start();
        host = std::make_shared<tcp_host>(io_service_);
        acceptor_->async_accept(host->socket(),
            std::bind(&Service::handle_accept_, this, host, 
            std::placeholders::_1));
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
    }
}

} // namespace soe
} // namespace network
} // namespace anh
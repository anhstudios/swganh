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

#include <anh/network/cluster/service.h>
#include <anh/network/cluster/socket.h>

#ifdef ERROR
#undef ERROR
#endif

#include <glog/logging.h>

using namespace anh::server_directory;
using namespace std;

namespace anh {
namespace network {
namespace cluster {

Service::Service(shared_ptr<ServerDirectoryInterface> directory)
    : directory_(directory)
{
    //proc_list_ = directory_->getProcessSnapshot(directory_->cluster());
}

Service::~Service(void)
{	
    Shutdown();
}

void Service::Start(uint16_t port)
{
    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_service_, tcp::endpoint(tcp::v4(), port ));
    auto conn = std::make_shared<TCPConnection>
        (io_service_, std::bind(&Service::OnNewConnection_, std::placeholders::_1, std::placeholders::_2));
    acceptor_->async_accept(conn->socket(), std::bind(&Service::OnNewConnection_, this, 
        std::placeholders::_1));

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
    //outgoing_pipeline_.run(1000);
    //incoming_pipeline_.run(1000);
}

void Service::Shutdown(void)
{
    outgoing_pipeline_.clear();
    incoming_pipeline_.clear();
    socket_.reset();
}

void Service::OnNewConnection_(const boost::system::error_code& error)
{
    auto conn = std::make_shared<TCPConnection>
        (io_service_, std::bind(&Service::OnNewConnection_, std::placeholders::_1, std::placeholders::_2));
    if (! error)
    {
        conn->Start();
        acceptor_->async_accept(conn->socket(), std::bind(&Service::OnNewConnection_, shared_from_this(), 
            std::placeholders::_1,std::placeholders::_2));

        // insert into map  ??
        //connection_map_.insert(ConnectionPair(1,2));
    }
}

} // namespace soe
} // namespace network
} // namespace anh
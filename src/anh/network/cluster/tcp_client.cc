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

#include <anh/network/cluster/tcp_client.h>

#include <boost/bind.hpp>
#include <iostream>

namespace anh {
namespace network {
namespace cluster {

tcp_client::tcp_client(boost::asio::io_service& service, const std::string& host, uint16_t port)
    : socket_(service)
    , resolver_(service)
    , bytes_recv_(0)
    , bytes_sent_(0)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    socket_.async_connect(endpoint,
            boost::bind(&tcp_client::handle_connect_, this,
                boost::asio::placeholders::error, endpoint) /*++endpoint_iterator*/);	

}

void tcp_client::handle_connect_(const boost::system::error_code& error, boost::asio::ip::tcp::endpoint endpoint)
{
    if(!error)
    {
        std::cout << "Connection To Endpoint " << endpoint.address() << " Successful..." << std::endl;
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
    }
}
tcp_client::~tcp_client(void)
{
    resolver_.cancel();
    socket_.close();
}

void tcp_client::Send(ByteBuffer& buffer)
{
    std::cout << "buffer sent size : " << buffer.size() << std::endl;
    socket_.async_write_some(boost::asio::buffer(buffer.data(), buffer.size()), 		 
        [this, buffer](const boost::system::error_code& error, std::size_t bytes_transferred)
    {
        bytes_sent_ += bytes_transferred;
    });
}


} // namespace soe
} // namespace network
} // namespace anh
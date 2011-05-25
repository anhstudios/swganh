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

tcp_client::tcp_client(boost::asio::io_service& service, const std::string& host, const std::string& port)
	: socket_(service)
    , resolver_(service)
	, bytes_recv_(0)
	, bytes_sent_(0)
{
    tcp::resolver resolver(service);
    tcp::resolver_query query(tcp::v4(), host, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    resolver_.async_resolve(query,[&] (const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator) {
        if (!error)
        {
        std::cout << "Handle Resolve" << std::endl;
        // attempt a connection to the first endpoint in the list
        tcp::endpoint endpoint = *endpoint_iterator;
        //auto socket = std::make_shared<tcp::socket>(socket_.io_service());
        socket_.async_connect(endpoint,
            boost::bind(&tcp_client::handle_connect_, this,
                boost::asio::placeholders::error, endpoint_iterator /*++endpoint_iterator*/));
        }
        else
        {
            std::cout << "error in resolve: " << error.message() << std::endl;
        }
    });
	
}

void tcp_client::handle_connect_(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
{
    if(!error)
    {
        std::cout << "Connection Successful, sending request..." << std::endl;
        tcp::endpoint endpoint = *endpoint_iterator;
        anh::ByteBuffer buffer((const unsigned char)"Hello");
        Send(endpoint, buffer);
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
       std::cout<< "Connection failed, try next endpoint" << std::endl;
       socket_.close();
       tcp::endpoint endpoint = *endpoint_iterator;
       socket_.async_connect(endpoint,
           boost::bind(&tcp_client::handle_connect_, this,
           boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
    }
}
tcp_client::~tcp_client(void)
{
    socket_.close();
}

void tcp_client::Send(boost::asio::ip::tcp::endpoint& endpoint, ByteBuffer& buffer)
{
	
	socket_.async_send(boost::asio::buffer(buffer.data(), buffer.size()), 		 
		[this, buffer](const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		bytes_sent_ += bytes_transferred;
	});
}


} // namespace soe
} // namespace network
} // namespace anh
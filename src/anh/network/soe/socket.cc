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

#include <anh/network/soe/socket.h>

#include <boost/bind.hpp>

namespace anh {
namespace network {
namespace soe {

Socket::Socket(boost::asio::io_service& service, uint16_t port, NetworkCallback callback)
	: socket_(service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	, callback_(callback)
	, bytes_recv_(0)
	, bytes_sent_(0)
{
	StartRead();
}

Socket::~Socket()
{

}

void Socket::Send(const boost::asio::ip::udp::endpoint& endpoint, ByteBuffer buffer) {
	socket_.async_send_to(boost::asio::buffer(buffer.data(), buffer.size()), 
		endpoint, 
		[this, buffer](const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		bytes_sent_ += bytes_transferred;
	});
}

void Socket::StartRead() {
	socket_.async_receive_from(boost::asio::buffer(recv_buffer_), current_remote_endpoint_,
		[&] (const boost::system::error_code& error, std::size_t bytes_transferred) {
			if(bytes_transferred > 2 || !error || error == boost::asio::error::message_size)
			{
				bytes_recv_ += bytes_transferred;
				callback_(current_remote_endpoint_, std::make_shared<ByteBuffer>((const unsigned char*)recv_buffer_.data(), bytes_transferred));
			}

			StartRead();
	});
}

} // namespace soe
} // namespace network
} // namespace anh
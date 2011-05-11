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

#ifndef ANH_NETWORK_SOE_SOCKET_H_
#define ANH_NETWORK_SOE_SOCKET_H_

#include <anh/byte_buffer.h>

#include <boost/asio.hpp>

#include <vector>

namespace anh {
namespace network {
namespace soe {

typedef std::function<void(boost::asio::ip::udp::endpoint&, anh::ByteBuffer&)> NetworkCallback;

/**
 * @brief A basic UDP Callback Socket.
 *
 * This class instantiates a udp socket and converts incoming messages to ByteBuffers. The function
 * will trigger a callback whenever data is recieved on the line, returning the remote endpoint and
 * a ByteBuffer containing the raw packet material ( which is then to be handled by the appropriate manager ).
 *
 */
class Socket
{
public:
	/**
	 * @brief Default Constructor.
	 *
	 * @param service The boost::asio::io_service that owns this socket.
	 * @param port The port to listen on for packet.
	 * @param callback Function called after the message is received.
	 */
	Socket(boost::asio::io_service& service, uint16_t port, NetworkCallback callback);

	/**
	 * @brief Default Deconstructor.
	 */
	~Socket(void);

	/**
	 * @returns The amount of bytes received on the socket.
	 */
	const uint64_t& bytes_recv() { return bytes_recv_; }

	/**
	 * @returns The amount of bytes sent on the socket.
	 */
	const uint64_t& bytes_sent() { return bytes_sent_; }

private:
	void StartSocketRecv_(void);

	uint64_t	bytes_recv_;
	uint64_t	bytes_sent_;
	NetworkCallback callback_;

	boost::asio::ip::udp::socket		socket_;
	boost::asio::ip::udp::endpoint		current_remote_endpoint_;
	std::vector<uint8_t>				recv_buffer_;
};

} // namespace soe
} // namespace network
} // namespace anh

#endif
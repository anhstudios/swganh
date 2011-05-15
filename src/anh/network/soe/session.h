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

#ifndef ANH_NETWORK_SOE_SESSION_H_
#define ANH_NETWORK_SOE_SESSION_H_

#include <anh/network/soe/socket.h>

#include <queue>
#include <boost/asio.hpp>

namespace anh {

// FORWARD DECLARATIONS
class ByteBuffer;
namespace event_dispatcher { class EventInterface; }

namespace network {
namespace soe {

/**
 * @brief An estabilished connection between a SOE Client and a SOE Service.
 */
class Session : public std::enable_shared_from_this<Session>
{
public:
	/**
	 * @brief Adds itself to the Session Manager.
	 */
	Session(boost::asio::ip::udp::endpoint& remote_endpoint, std::shared_ptr<Socket> socket);
	~Session(void);

	void SendMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> message);
	void SendMessage(std::shared_ptr<anh::ByteBuffer> message);
	void SendMessage(char* buffer, uint16_t len);

	/**
	 * Clears each message pump.
	 */
	void Update(void);

	/**
	 * Closes the Session.
	 */
	void Disconnect(void);

	bool connected() { return connected_; }

	uint32_t connection_id() { return connection_id_; }
	void set_connection_id(uint32_t connection_id) { connection_id_ = connection_id; }

	uint32_t recv_buffer_size() { return recv_buffer_size_; }
	void set_recv_buffer_size(uint32_t size) { recv_buffer_size_ = size; }

	uint32_t crc_len() { return crc_len_; }
	void set_crc_len(uint32_t crc_len) { crc_len_ = crc_len; }

	boost::asio::ip::udp::endpoint& remote_endpoint() { return remote_endpoint_; }

private:
	boost::asio::ip::udp::endpoint		remote_endpoint_;
	std::shared_ptr<Socket>				socket_;

	std::queue<std::shared_ptr<anh::ByteBuffer>>			fragmented_messages_;
	std::queue<std::shared_ptr<anh::ByteBuffer>>			outgoing_messages_;
	std::queue<std::shared_ptr<anh::ByteBuffer>>			incoming_messages_;

	bool								connected_;

	// SOE Session Variables
	uint32_t							connection_id_;
	uint32_t							recv_buffer_size_;
	uint32_t							crc_len_;
};

} // namespace soe
} // namespace network
} // namespace anh

#endif
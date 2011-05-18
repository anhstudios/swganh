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

#ifndef ANH_NETWORK_SOE_SERVICE_H_
#define ANH_NETWORK_SOE_SERVICE_H_

#include <anh/byte_buffer.h>
#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/socket.h>

// Filters
#include <anh/network/soe/crc_filter.h>
#include <anh/network/soe/decompression_filter.h>
#include <anh/network/soe/decryption_filter.h>
#include <anh/network/soe/recv_packet_filter.h>
#include <anh/network/soe/session_request_filter.h>
#include <anh/network/soe/soe_protocol_filter.h>

#include <map>
#include <list>

#include <boost/asio.hpp>

#include <tbb/pipeline.h>

namespace anh {
namespace network {
namespace soe {

// FORWARD DECLARATION
class IncomingPacket;
class IncomingSessionlessPacket;
class OutgoingPacket;

/**
 * @brief Represent an SOE Service, which takes incoming SOE Protocol packets on a specific port and filters them down to events and vise versa.
 */
class Service : public std::enable_shared_from_this<Service>
{
public:
	Service(void);
	~Service(void);

	/**
	 * @brief Starts the SOE Frontend Service.
	 * 
	 * @parama port The port to listen for messages on.
	 */
	void Start(uint16_t port);

	/**
	 * @brief Performs any work the pipelines have waiting and receives and sends
	 * any waiting messages. (Blocking)
	 */
	void Update(void);
	
	/**
	 * @brief
	 */
	void Shutdown(void);

	// Friended Filters. Filters are considered extensions of the
	// Service class, as the operations performed in them are specific
	// to this Service type (SOE).
	friend class CrcFilter;
	friend class DecompressionFilter;
	friend class DecryptionFilter;
	friend class RecvPacketFilter;
	friend class SessionRequestFilter;
	friend class SoeProtocolFilter;

	// We friend the Session class to gain access to the Socket
	// and packet processing queues/pipelines without exposing
	// such volitle tools to the global space.
	friend class Session;

private:
	/**
	 * @brief Called when the socket receives a message.
	 *
	 * This function will also attempt to find the session in the SessionManager,
	 * if it fails to find a Session it will check if the incoming message is a
	 * Session Request in which case, it will create a new Session.
	 */
	void OnSocketRecv_(boost::asio::ip::udp::endpoint& remote_endpoint, std::shared_ptr<anh::ByteBuffer> message);

	std::shared_ptr<Socket>		socket_;
	boost::asio::io_service		io_service_;

	SessionManager				session_manager_;

	// Pipelines
	tbb::pipeline				sessionless_incoming_pipeline_;
	tbb::pipeline				incoming_pipeline_;
	tbb::pipeline				outgoing_pipeline_;
	tbb::pipeline				outgoing_data_message_pipeline_;
	tbb::pipeline				incoming_data_message_pipeline_;

	std::list<IncomingSessionlessPacket*>		sessionless_messages_;
	std::list<IncomingPacket*>					incoming_messages_;
	std::list<OutgoingPacket*>					outgoing_messages_;

	// Filters
	CrcFilter					crc_filter_;
	DecompressionFilter			decompression_filter_;
	DecryptionFilter			decryption_filter_;
	RecvPacketFilter			recv_packet_filter_;
	SessionRequestFilter		session_request_filter_;
	SoeProtocolFilter			soe_protocol_filter_;

};

} // namespace soe
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_SOE_SERVER_H_
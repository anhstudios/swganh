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
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/socket.h>

// Filters
#include <anh/network/soe/compression_filter.h>
#include <anh/network/soe/crc_in_filter.h>
#include <anh/network/soe/crc_out_filter.h>
#include <anh/network/soe/decompression_filter.h>
#include <anh/network/soe/decryption_filter.h>
#include <anh/network/soe/encryption_filter.h>
#include <anh/network/soe/recv_packet_filter.h>
#include <anh/network/soe/outgoing_start_filter.h>
#include <anh/network/soe/send_packet_filter.h>
#include <anh/network/soe/session_request_filter.h>
#include <anh/network/soe/soe_protocol_filter.h>

#include <map>
#include <list>

#include <boost/asio.hpp>

#include <tbb/pipeline.h>
#include <tbb/task_group.h>

namespace anh {
namespace network {
namespace soe {

// FORWARD DECLARATION
class IncomingPacket;
class IncomingSessionlessPacket;
class OutgoingPacket;

/**
 * @brief An SOE Protocol Service.
 *
 * 
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
	friend class CompressionFilter;
	friend class CrcInFilter;
	friend class CrcOutFilter;
	friend class DecompressionFilter;
	friend class DecryptionFilter;
	friend class EncryptionFilter;
	friend class OutgoingStartFilter;
	friend class RecvPacketFilter;
	friend class SendPacketFilter;
	friend class SessionRequestFilter;
	friend class SoeProtocolFilter;

	// We friend the Session class to gain access to the Socket
	// and packet processing queues/pipelines without exposing
	// such volitle tools to the global space.
	friend class Session;

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher();
    void event_dispatcher(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);

private:
	/**
	 * @brief Called when the socket receives a message.
	 */
	void OnSocketRecv_(boost::asio::ip::udp::endpoint remote_endpoint, std::shared_ptr<anh::ByteBuffer> message);

	std::shared_ptr<Socket>		socket_;
	boost::asio::io_service		io_service_;

	SessionManager				session_manager_;
    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher_;
	uint32_t					crc_seed_;

	// Pipelines
	tbb::pipeline				sessionless_incoming_pipeline_;
	tbb::pipeline				incoming_pipeline_;
	tbb::pipeline				outgoing_pipeline_;

    tbb::filter_t<void, void>   incoming_filter_;
    tbb::filter_t<void, void>   outgoing_filter_;
    tbb::filter_t<void, void>   sessionless_filter_;

	std::list<IncomingSessionlessPacket*>		sessionless_messages_;
	std::list<std::shared_ptr<IncomingPacket>>	incoming_messages_;
	std::list<OutgoingPacket*>					outgoing_messages_;

	// Outgoing Packet Pipeline Filters
	//
	OutgoingStartFilter			outgoing_start_filter_;
	CompressionFilter			compression_filter_;
	EncryptionFilter			encryption_filter_;
	CrcOutFilter				crc_out_filter_;
	SendPacketFilter			send_packet_filter_;
	
	// Sessionless Packet Pipeline Filters
	//
	SessionRequestFilter		session_request_filter_;

	// Incoming Packet Pipeline Filters
	//
	RecvPacketFilter			recv_packet_filter_;
	CrcInFilter					crc_in_filter_;
	DecryptionFilter			decryption_filter_;
	DecompressionFilter			decompression_filter_;
	SoeProtocolFilter			soe_protocol_filter_;
};

} // namespace soe
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_SOE_SERVER_H_
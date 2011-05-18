/*7262626262676
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

#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/protocol_opcodes.h>
#include <anh/network/soe/outgoing_packet.h>
#include <anh/utilities.h>
#include <anh/event_dispatcher/event_interface.h>

#ifdef ERROR
#undef ERROR
#endif

#include <glog/logging.h>

namespace anh {
namespace network {
namespace soe {

Session::Session(boost::asio::ip::udp::endpoint& remote_endpoint, Service* service)
	: std::enable_shared_from_this<Session>()
	, remote_endpoint_(remote_endpoint)
	, connected_(false)
	, service_(service)
	, server_net_stats_(0, 0, 0, 0, 0, 0)
{
}

Session::~Session(void)
{
	Close();
}

void Session::Update(void)
{
}

void Session::SendMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> message)
{
	auto buffer = std::make_shared<ByteBuffer>();
	message->serialize(*buffer);
}

void Session::Close(void)
{
	if(connected_)
	{
		connected_ = false;
		service_->session_manager_.RemoveSession(shared_from_this());
	}

	LOG(WARNING) << "Session Closed.";
}

void Session::HandleSoeMessage(anh::ByteBuffer& message)
{
	switch(message.peek<uint16_t>(true))
	{
	case SESSION_REQUEST: { handleSessionRequest_(SessionRequest(message)); break; }
	case MULTI_PACKET: { handleMultiPacket_(MultiPacket(message)); break; }
	case DISCONNECT: { handleDisconnect_(Disconnect(message)); break; }
	case PING: { handlePing_(Ping(message)); break; }
	case NET_STATS_CLIENT: { handleNetStatsClient_(NetStatsClient(message)); break; }
	case CHILD_DATA_A: { handleChildDataA_(ChildDataA(message)); break; }
	case DATA_FRAG_A: { handleDataFragA_(DataFragA(message)); break; }
	case ACK_A: { handleAckA_(AckA(message)); break; }
	case FATAL_ERROR: { break; }
	default:
		LOG(WARNING) << "Unhandled SOE Opcode" << message.peek<uint16_t>(true);
	}
}

void Session::handleSessionRequest_(SessionRequest& packet)
{
	LOG(WARNING) << "Processing Session Request...";
	connection_id_ = packet.connection_id;
	crc_length_ = packet.crc_length;
	recv_buffer_size_ = packet.client_udp_buffer_size;

	SessionResponse session_response(connection_id_, service_->crc_seed_);
	anh::ByteBuffer session_response_buffer;
	session_response.serialize(session_response_buffer);

	// Directly put this on the wire, it requires no outgoing processing.
	service_->socket_->Send(remote_endpoint_, session_response_buffer);

	service_->session_manager_.AddSession(shared_from_this());
}

void Session::handleMultiPacket_(MultiPacket& packet)
{
	LOG(WARNING) << "Processing Multi-Packet Begin";
	std::for_each(packet.packets.begin(), packet.packets.end(), [=](anh::ByteBuffer& buffer) {
		HandleSoeMessage(buffer);
	});
	LOG(WARNING) << "Processing Multi-Packet End";
}

void Session::handleDisconnect_(Disconnect& packet)
{
	LOG(WARNING) << "Closing Session...";
	Close();
}

void Session::handlePing_(Ping& packet)
{
	LOG(WARNING) << "Processing Ping.";
}

void Session::handleNetStatsClient_(NetStatsClient& packet)
{
	LOG(WARNING) << "Processing NetStatsClient";

	server_net_stats_.client_tick_count = packet.client_tick_count;

	// Serialize.
	//std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();
	//server_net_stats_.serialize(*buffer);

	// Queue for outgoing.
	//service_->outgoing_messages_.push_back(new OutgoingPacket(shared_from_this(), buffer));
}

void Session::handleChildDataA_(ChildDataA& packet)
{
	LOG(WARNING) << "Handling Child Data A [" << std::hex << packet.data.peekAt<uint32_t>(4) << "] Sequence: [" << packet.sequence << "]";
	AckA ack(packet.sequence);

	std::shared_ptr<anh::ByteBuffer> ack_buffer = std::make_shared<anh::ByteBuffer>();
	ack.serialize(*ack_buffer);

	service_->outgoing_messages_.push_back(new OutgoingPacket(shared_from_this(), ack_buffer));
}

void Session::handleDataFragA_(DataFragA& packet)
{
	LOG(WARNING) << "Handling Data Frag A.";
}

void Session::handleAckA_(AckA& packet)
{
	LOG(WARNING) << "Handling Ack A.";
}

} // namespace anh
} // namespace network
} // namespace soe
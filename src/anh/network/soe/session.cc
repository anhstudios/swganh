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
	, last_acknowledged_sequence_(0)
	, current_client_sequence_(0)
	, next_client_sequence_(0)
	, server_sequence_(0)
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
}

void Session::Close(void)
{
	if(connected_)
	{
		connected_ = false;
		service_->session_manager_.RemoveSession(shared_from_this());
	}

	Disconnect disconnect(connection_id_);
	std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();

	disconnect.serialize(*buffer);
	SendSoePacket(buffer);

	LOG(WARNING) << "Session [" << connection_id_ << "] Closed.";
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
		LOG(INFO) << "Unhandled SOE Opcode" << message.peek<uint16_t>(true);
	}
}

void Session::handleSessionRequest_(SessionRequest& packet)
{
	connection_id_ = packet.connection_id;
	crc_length_ = packet.crc_length;
	recv_buffer_size_ = packet.client_udp_buffer_size;

	SessionResponse session_response(connection_id_, service_->crc_seed_);
	anh::ByteBuffer session_response_buffer;
	session_response.serialize(session_response_buffer);

	// Directly put this on the wire, it requires no outgoing processing.
	service_->socket_->Send(remote_endpoint_, session_response_buffer);

	service_->session_manager_.AddSession(shared_from_this());

	LOG(WARNING) << "Created Session [" << connection_id_ << "] @ " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port();
}

void Session::handleMultiPacket_(MultiPacket& packet)
{
	DLOG(WARNING) << "Handling DISCONNECT";
	std::for_each(packet.packets.begin(), packet.packets.end(), [=](anh::ByteBuffer& buffer) {
		HandleSoeMessage(buffer);
	});
}

void Session::handleDisconnect_(Disconnect& packet)
{
	DLOG(WARNING) << "Handling DISCONNECT";
	Close();
}

void Session::handlePing_(Ping& packet)
{
	DLOG(WARNING) << "Handling PING";
	Ping pong;
	
	std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();
	pong.serialize(*buffer);

	SendSoePacket(buffer);
}

void Session::handleNetStatsClient_(NetStatsClient& packet)
{
	DLOG(WARNING) << "Handling NET_STATS_CLIENT";
	server_net_stats_.client_tick_count = packet.client_tick_count;

	std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();
	server_net_stats_.serialize(*buffer);
	SendSoePacket(buffer);
}

void Session::handleChildDataA_(ChildDataA& packet)
{	
	DLOG(WARNING) << "Handling CHILD_DATA_A";
	if(!SequenceIsValid_(packet.sequence))
		return;

	AcknowledgeSequence_(packet.sequence);

}

void Session::handleDataFragA_(DataFragA& packet)
{
	DLOG(WARNING) << "Handling DATA_FRAG_A";
	if(!SequenceIsValid_(packet.sequence))
		return;

	AcknowledgeSequence_(packet.sequence);

	
}

void Session::handleAckA_(AckA& packet)
{
	DLOG(WARNING) << "Handling ACK_A";
	SequencedMessageMapIterator begin = sent_messages_.find(last_acknowledged_sequence_);
	SequencedMessageMapIterator end = sent_messages_.find(packet.sequence);
	sent_messages_.erase(begin, end);
}

void Session::SendSoePacket(std::shared_ptr<anh::ByteBuffer> message)
{
	service_->outgoing_messages_.push_back(new OutgoingPacket(shared_from_this(), message));
}

bool Session::SequenceIsValid_(const uint16_t& sequence)
{
	if(next_client_sequence_ == sequence)
	{
		return true;
	}
	else
	{
		// Tell the client we have received an Out of Order sequence.
		OutOfOrderA	out_of_order(current_client_sequence_ + 1);
		std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();
		out_of_order.serialize(*buffer);
		SendSoePacket(buffer);

		return false;
	}
}

void Session::AcknowledgeSequence_(const uint16_t& sequence)
{
	AckA ack(sequence);
	std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();
	ack.serialize(*buffer);
	SendSoePacket(buffer);

	next_client_sequence_ = sequence + 1;
	current_client_sequence_ = sequence;
}

} // namespace anh
} // namespace network
} // namespace soe
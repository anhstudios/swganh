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
#include <anh/network/network_events.h>
#include <anh/utilities.h>
#include <anh/event_dispatcher/event_interface.h>

#include <packets/Login/LoginClientId.h>
#include <packets/Login/LoginClientToken.h>
#include <packets/Login/LoginEnumCluster.h>
#include <packets/Login/EnumerateCharacterId.h>
#include <packets/Login/LoginClusterStatus.h>

#include <iostream>

#ifdef ERROR
#undef ERROR
#endif

#include <glog/logging.h>

using anh::event_dispatcher::make_shared_event;

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
	, outgoing_data_message_(0)
	, incoming_fragmented_total_len_(0)
	, incoming_fragmented_curr_len_(0)
{
}

Session::~Session(void)
{
	Close();

	LOG(WARNING) << "Session [" << connection_id_ << "] Closed.";
}

void Session::Update(void)
{
	// If we have waiting messages that need to be sent.
	if(!outgoing_data_queue_[active_outgoing_queue_].empty()) {	
        int previous_active_queue = active_outgoing_queue_;
        active_outgoing_queue_.fetch_and_store((active_outgoing_queue_ + 1) % NUM_QUEUES);

        auto process_queue = std::move(outgoing_data_queue_[previous_active_queue]);

        ByteBuffer data_message;
        if (process_queue.size() > 1) {
			data_message.write<uint16_t>(anh::bigToHost<uint16_t>(0x19));
			std::for_each(process_queue.begin(), process_queue.end(), [=, &data_message](anh::ByteBuffer& item){
				data_message.write<uint8_t>(item.size());
				data_message.append(item);
			});
        } else {
            data_message.append(process_queue.front());
        }



        if (data_message.size() >= recv_buffer_size_) {
            uint32_t bytes_sent = 0;
            uint32_t message_size = data_message.size();

            // Create a temporary buffer that prepends a size value to the data.
            ByteBuffer tmp;
            tmp.write<uint32_t>(message_size);
            tmp.append(std::move(data_message));

            while (bytes_sent < message_size) {
                uint16_t server_sequence = ++server_sequence_;

                uint32_t bytes_remaining = message_size - bytes_sent;

                uint32_t chunk_size = (bytes_remaining > recv_buffer_size_) ? recv_buffer_size_ : bytes_remaining;

                ByteBuffer fragmented_data_chunk(tmp.data(), tmp.read_position() + bytes_sent + chunk_size);
                bytes_sent =+ chunk_size;
                tmp.read_position(bytes_sent);

                DataFragA outgoing_message(server_sequence, std::move(fragmented_data_chunk));
                
                auto message = std::make_shared<ByteBuffer>();
                outgoing_message.serialize(*message);
		        SendSoePacket(message);
                
                sent_messages_.insert(SequencedMessageMap::value_type(server_sequence, message));
            }
        } else {
            uint16_t server_sequence = ++server_sequence_;

            ChildDataA outgoing_message(server_sequence);
            outgoing_message.messages = std::move(process_queue);

            auto message = std::make_shared<ByteBuffer>();
		    SendSoePacket(message);

            sent_messages_.insert(SequencedMessageMap::value_type(server_sequence, message));
        }
	}
}

void Session::SendMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> message)
{ 
	ByteBuffer message_buffer;
	message->serialize(message_buffer);

    outgoing_data_queue_[active_outgoing_queue_].push_back(std::move(message_buffer));
}

void Session::Close(void)
{
	if(connected_)
	{
		connected_ = false;
		service_->session_manager_.RemoveSession(shared_from_this());

		Disconnect disconnect(connection_id_);
		std::shared_ptr<anh::ByteBuffer> buffer = std::make_shared<anh::ByteBuffer>();

		disconnect.serialize(*buffer);
		SendSoePacket(buffer);
	}
}

void Session::HandleSoeMessage(anh::ByteBuffer& message)
{
	switch(message.peek<uint16_t>(true))
	{
	case SESSION_REQUEST:					{ handleSessionRequest_(SessionRequest(message)); break; }
	case MULTI_PACKET:						{ handleMultiPacket_(MultiPacket(message)); break; }
	case DISCONNECT:						{ handleDisconnect_(Disconnect(message)); break; }
	case PING:								{ handlePing_(Ping(message)); break; }
	case NET_STATS_CLIENT:					{ handleNetStatsClient_(NetStatsClient(message)); break; }
	case CHILD_DATA_A:						{ handleChildDataA_(ChildDataA(message)); break; }
	case DATA_FRAG_A:						{ handleDataFragA_(DataFragA(message)); break; }
	case ACK_A:								{ handleAckA_(AckA(message)); break; }
	case OUT_OF_ORDER_A:					{ handleOutOfOrderA_(OutOfOrderA(message)); break; }
	case FATAL_ERROR:						{ Close(); break; }
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

	// Add our Session to the SessionManager and flag ourselves as connected.
	service_->session_manager_.AddSession(shared_from_this());
	connected_ = true;

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

    std::for_each(packet.messages.begin(), packet.messages.end(), [this] (anh::ByteBuffer& message) {
        uint16_t priority = message.read<uint16_t>();
        uint32_t message_type = message.read<uint32_t>();

        this->service_->event_dispatcher()->triggerAsync(make_shared_event(
            message_type,
            anh::network::RemoteMessage(this->connection_id(), message)
        ));
    });
}

void Session::handleDataFragA_(DataFragA& packet)
{
	DLOG(WARNING) << "Handling DATA_FRAG_A";
	if(!SequenceIsValid_(packet.sequence))
		return;
	
	AcknowledgeSequence_(packet.sequence);

	// Continuing a frag
	if(incoming_fragmented_total_len_ > 0)
	{
		incoming_fragmented_curr_len_ += packet.data.size();
		incoming_fragmented_messages_.push_back(packet.data);

		if(incoming_fragmented_total_len_ == incoming_fragmented_curr_len_)
		{
			// Send to translator.
			incoming_fragmented_total_len_ = 0;
			incoming_fragmented_curr_len_ = 0;

            ByteBuffer full_packet;
            std::for_each(
                incoming_fragmented_messages_.cbegin(), 
                incoming_fragmented_messages_.cend(), 
                [&full_packet] (const ByteBuffer& buffer) {                
                    full_packet.append(buffer);
                }
            );

            handleChildDataA_(ChildDataA(std::move(full_packet)));

            incoming_fragmented_messages_.clear();
		}
	}
	// Starting a new frag
	else
	{
		incoming_fragmented_total_len_ = packet.data.read<uint16_t>();
		incoming_fragmented_curr_len_ += packet.data.size();
		incoming_fragmented_messages_.push_back(anh::ByteBuffer(packet.data.data()+2, packet.data.size()-2));
	}
	
}

void Session::handleAckA_(AckA& packet)
{
	DLOG(WARNING) << "Handling ACK_A";
	SequencedMessageMapIterator begin = sent_messages_.find(last_acknowledged_sequence_);
	SequencedMessageMapIterator end = sent_messages_.find(packet.sequence);

	if(begin == end)
	{
		sent_messages_.erase(begin);
		return;
	}
	else
	{
		sent_messages_.erase(begin, end);
		return;
	}
}

void Session::handleOutOfOrderA_(OutOfOrderA& packet)
{
	DLOG(WARNING) << "Handle OUT_OF_ORDER_A";

	SequencedMessageMapIterator begin = sent_messages_.find(last_acknowledged_sequence_);
	SequencedMessageMapIterator end = sent_messages_.end();

	std::for_each(begin, end, [=](SequencedMessageMap::value_type& item) {
		SendSoePacket(item.second);
	});

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
		OutOfOrderA	out_of_order(sequence);
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
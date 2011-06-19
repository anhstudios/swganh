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

#include <algorithm>

#include <boost/pool/pool_alloc.hpp>

#include <anh/network/soe/session.h>
#include <anh/network/soe/session_manager.h>
#include <anh/network/soe/service.h>
#include <anh/network/soe/protocol_opcodes.h>
#include <anh/network/soe/outgoing_packet.h>
#include "anh/network/soe/packet_utilities.h"
#include <anh/network/network_events.h>
#include <anh/utilities.h>
#include <anh/event_dispatcher/event_interface.h>

#include <packets/Login/LoginClientId.h>
#include <packets/Login/LoginClientToken.h>
#include <packets/Login/LoginEnumCluster.h>
#include <packets/Login/EnumerateCharacterId.h>
#include <packets/Login/LoginClusterStatus.h>

#include <iostream>

#include <glog/logging.h>

using namespace anh;
using namespace anh::event_dispatcher;
using namespace anh::network;
using namespace anh::network::soe;
using namespace std;

Session::Session(boost::asio::ip::udp::endpoint remote_endpoint, anh::network::SocketInterface<boost::asio::ip::udp>* socket)
    : std::enable_shared_from_this<Session>()
    , remote_endpoint_(remote_endpoint)
    , connected_(false)
    , service_(nullptr)
    , socket_(socket)
    , server_net_stats_(0, 0, 0, 0, 0, 0)
    , last_acknowledged_sequence_(0)
    , current_client_sequence_(0)
    , next_client_sequence_(0)
    , server_sequence_()
    , outgoing_data_message_(0)
    , incoming_fragmented_total_len_(0)
    , incoming_fragmented_curr_len_(0)
{
    server_sequence_ = 0;
}

Session::~Session(void)
{
    Close();

    LOG(WARNING) << "Session [" << connection_id_ << "] Closed.";
}

uint16_t Session::server_sequence() const {
    return server_sequence_;
}

void Session::receive_buffer_size(uint32_t receive_buffer_size) {
    receive_buffer_size_ = receive_buffer_size;
}

void Session::crc_length(uint32_t crc_length) {
    crc_length_ = crc_length;
}

vector<shared_ptr<ByteBuffer>> Session::GetUnacknowledgedMessages() const {
    vector<shared_ptr<ByteBuffer>> unacknowledged_messages;

    for_each(
        sent_messages_.begin(), 
        sent_messages_.end(), 
        [&unacknowledged_messages] (const SequencedMessageMap::value_type& i) 
    {
        unacknowledged_messages.push_back(i.second);
    });

    return unacknowledged_messages;
}

void Session::Update() {
    // Exit as quickly as possible if there is no work currently.
    if (outgoing_data_messages_.empty()) {
        return;
    }

    // Build up a list of data messages to process
    list<shared_ptr<ByteBuffer>> process_list(
        outgoing_data_messages_.unsafe_begin(), 
        outgoing_data_messages_.unsafe_end());

    // Pack the message list into a single data channel payload and send it.
    SendMessage(PackDataChannelMessages(process_list));
}

void Session::SendMessage(ByteBuffer data_channel_payload) {
    // If the data channel message is too big
    uint32_t max_data_channel_size = receive_buffer_size_ - crc_length_ - 5;

    if (data_channel_payload.size() > max_data_channel_size) {
        list<ByteBuffer> fragmented_message = SplitDataChannelMessage(
            data_channel_payload, 
            max_data_channel_size);

        for_each(fragmented_message.begin(), fragmented_message.end(), [this] (ByteBuffer& fragment) {
            // Get the next sequence number
            uint16_t message_sequence = ++server_sequence_;

            // Allocate a new packet
            auto data_channel_message = AllocateBuffer_(BuildFragmentedDataChannelHeader(message_sequence));
            data_channel_message->append(fragment);
            
            // Send it over the wire
            socket_->Send(remote_endpoint_, *data_channel_message);
            
            // Store it for resending later if necessary
            sent_messages_.insert(make_pair(message_sequence, data_channel_message));
        });
    } else {        
        // Get the next sequence number
        uint16_t message_sequence = ++server_sequence_;

        // Allocate a new packet
        auto data_channel_message = AllocateBuffer_(BuildDataChannelHeader(message_sequence));
        data_channel_message->append(data_channel_payload);
        
        // Send it over the wire
        socket_->Send(remote_endpoint_, *data_channel_message);
        
        // Store it for resending later if necessary
        sent_messages_.insert(make_pair(message_sequence, data_channel_message));
    }
}

void Session::SendMessage(std::shared_ptr<anh::event_dispatcher::EventInterface> message) { 
    auto message_buffer = AllocateBuffer_();
    message->serialize(*message_buffer);

    outgoing_data_messages_.push(message_buffer);
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
    case CHILD_DATA_A:						{ handleChildDataA_(ChildDataA(message)); break; }
    case MULTI_PACKET:						{ handleMultiPacket_(MultiPacket(message)); break; }
    case DATA_FRAG_A:						{ handleDataFragA_(DataFragA(message)); break; }
    case ACK_A:								{ handleAckA_(AckA(message)); break; }
    case PING:								{ handlePing_(Ping(message)); break; }
    case NET_STATS_CLIENT:					{ handleNetStatsClient_(NetStatsClient(message)); break; }
    case OUT_OF_ORDER_A:					{ handleOutOfOrderA_(OutOfOrderA(message)); break; }
    case DISCONNECT:						{ handleDisconnect_(Disconnect(message)); break; }
    case SESSION_REQUEST:					{ handleSessionRequest_(SessionRequest(message)); break; }
    case FATAL_ERROR:						{ Close(); break; }
    default:
        LOG(INFO) << "Unhandled SOE Opcode" << message.peek<uint16_t>(true);
    }
}

shared_ptr<ByteBuffer> Session::AllocateBuffer_() const {    
    auto allocated_buffer = allocate_shared<ByteBuffer, boost::pool_allocator<ByteBuffer>>(
        boost::pool_allocator<ByteBuffer>());

    return allocated_buffer;
}

shared_ptr<ByteBuffer> Session::AllocateBuffer_(ByteBuffer buffer) const {    
    auto allocated_buffer = allocate_shared<ByteBuffer, boost::pool_allocator<ByteBuffer>>(
        boost::pool_allocator<ByteBuffer>(), std::move(buffer));

    return allocated_buffer;
}

void Session::handleSessionRequest_(SessionRequest& packet)
{
    connection_id_ = packet.connection_id;
    crc_length_ = packet.crc_length;
    receive_buffer_size_ = packet.client_udp_buffer_size;

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
    DLOG(WARNING) << "Handling MULTIPACKET";
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

    std::for_each(packet.messages.begin(), packet.messages.end(), [this] (shared_ptr<ByteBuffer> message) {
        uint16_t priority = message->read<uint16_t>();
        uint32_t message_type = message->read<uint32_t>();

        this->service_->event_dispatcher()->triggerAsync(make_shared_event(
            message_type,
            anh::network::RemoteMessage(this->connection_id(), *message)
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

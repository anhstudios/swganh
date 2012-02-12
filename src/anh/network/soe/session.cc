/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "anh/network/soe/session.h"

#include <algorithm>

#include <glog/logging.h>

#include "anh/network/soe/socket.h"
#include "anh/network/soe/session_manager.h"
#include "anh/network/soe/server_interface.h"

#include "anh/network/soe/protocol_opcodes.h"
#include "anh/network/soe/packet_utilities.h"

using namespace anh;
using namespace anh::event_dispatcher;
using namespace anh::network;
using namespace anh::network::soe;
using namespace std;

Session::Session(boost::asio::ip::udp::endpoint remote_endpoint, ServerInterface* server)
    : std::enable_shared_from_this<Session>()
    , remote_endpoint_(remote_endpoint)
    , connected_(false)
    , server_(server)
    , crc_seed_(0xDEADBABE)
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

uint32_t Session::receive_buffer_size() const {
    return receive_buffer_size_;
}

void Session::receive_buffer_size(uint32_t receive_buffer_size) {
    receive_buffer_size_ = receive_buffer_size;
}

uint32_t Session::crc_length() const {
    return crc_length_;
}

void Session::crc_length(uint32_t crc_length) {
    crc_length_ = crc_length;
}

void Session::crc_seed(uint32_t crc_seed) {
    crc_seed_ = crc_seed;
}

uint32_t Session::crc_seed() const {
    return crc_seed_;
}

void Session::datachannel_handler(DatachannelHandler handler) {
    datachannel_handler_ = make_shared<DatachannelHandler>(handler);
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
    if (!connected_ || outgoing_data_messages_.empty()) {
        return;
    }

    // Build up a list of data messages to process
    uint32_t message_count = outgoing_data_messages_.unsafe_size();
    list<shared_ptr<ByteBuffer>> process_list;
    shared_ptr<ByteBuffer> tmp;

    for (uint32_t i = 0; i < message_count; ++i) {
        if (outgoing_data_messages_.try_pop(tmp)) {
            process_list.push_back(tmp);
        }
    }

    // Pack the message list into a single data channel payload and send it.
    ByteBuffer data_channel_payload = PackDataChannelMessages(process_list);
    
    // Split up the message if it's too big
    // @note: in determining the max size 3 is the size of the soe header + the compression flag.
    uint32_t max_data_channel_size = receive_buffer_size_ - crc_length_ - 3;

    if (data_channel_payload.size() > max_data_channel_size) {
        list<ByteBuffer> fragmented_message = SplitDataChannelMessage(
            data_channel_payload, 
            max_data_channel_size);

        for_each(fragmented_message.begin(), fragmented_message.end(), [this] (ByteBuffer& fragment) {
            SendSequencedMessage_(&BuildFragmentedDataChannelHeader, move(fragment));
        });
    } else {        
        SendSequencedMessage_(&BuildDataChannelHeader, move(data_channel_payload));
    }
}

void Session::SendMessage(ByteBuffer message) {
    auto message_buffer = server_->AllocateBuffer();
    message_buffer->swap(message);
    
    outgoing_data_messages_.push(message_buffer);
}

void Session::Close(void)
{
    if(connected_)
    {
        connected_ = false;
        server_->RemoveSession(shared_from_this());

        Disconnect disconnect(connection_id_);
        auto buffer = server_->AllocateBuffer();

        disconnect.serialize(*buffer);
        SendSoePacket_(buffer);
    }
}

void Session::HandleMessage(anh::ByteBuffer& message)
{
    switch(message.peek<uint16_t>(true))
    {
        case CHILD_DATA_A:	   { ChildDataA child_data_a(message); handleChildDataA_(child_data_a); break; }
        case MULTI_PACKET:	   { MultiPacket multi_packet(message); handleMultiPacket_(multi_packet); break; }
        case DATA_FRAG_A:	   { DataFragA data_frag_a(message); handleDataFragA_(data_frag_a); break; }
        case ACK_A:			   { AckA ack_a(message); handleAckA_(ack_a); break; }
        case PING:			   { Ping ping(message); handlePing_(ping); break; }
        case NET_STATS_CLIENT: { NetStatsClient net_stats_client(message); handleNetStatsClient_(net_stats_client); break; }
        case OUT_OF_ORDER_A:   { OutOfOrderA out_of_order_a(message); handleOutOfOrderA_(out_of_order_a); break; }
        case DISCONNECT:	   { Disconnect disconnect(message); handleDisconnect_(disconnect); break; }
        case SESSION_REQUEST:  { SessionRequest session_request(message); handleSessionRequest_(session_request); break; }
        case FATAL_ERROR:      { Close(); break; }
        default:
            if (message.peek<uint8_t>() != 0)
            {
                auto packet = make_shared<Packet>(shared_from_this(), make_shared<ByteBuffer>(message));
                this->server_->HandleMessage(packet);
            }
            else 
            {
                LOG(WARNING) << "Unhandled SOE Opcode: " << std::hex << message.peek<uint16_t>(true)
                    << "\n\n" << message;
            }
    }
}

void Session::SendSequencedMessage_(HeaderBuilder header_builder, ByteBuffer message) {
    // Get the next sequence number
    uint16_t message_sequence = server_sequence_++;

    // Allocate a new packet
    auto data_channel_message = server_->AllocateBuffer();
    data_channel_message->append(header_builder(message_sequence));
    data_channel_message->append(message);
    
    // Send it over the wire
    SendSoePacket_(data_channel_message);
    
    // Store it for resending later if necessary
    sent_messages_.push_back(make_pair(message_sequence, data_channel_message));
    
    // If the data channel message is too big
    uint32_t max_data_channel_size = receive_buffer_size_ - crc_length_ - 5;
}

void Session::handleSessionRequest_(SessionRequest& packet)
{
    connection_id_ = packet.connection_id;
    crc_length_ = packet.crc_length;
    receive_buffer_size_ = packet.client_udp_buffer_size;

    SessionResponse session_response(connection_id_, crc_seed_);
    session_response.server_udp_buffer_size = server_->max_receive_size();

    anh::ByteBuffer session_response_buffer;
    session_response.serialize(session_response_buffer);

    // Directly put this on the wire, it requires no outgoing processing.
    server_->socket()->Send(remote_endpoint_, session_response_buffer);

    // Add our Session to the SessionManager and flag ourselves as connected.
    if (server_->AddSession(shared_from_this())) {
        connected_ = true;
        LOG(INFO) << "Created Session [" << connection_id_ << "] @ " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port();
    }
}

void Session::handleMultiPacket_(MultiPacket& packet)
{
    VLOG(3) << "Handling MULTIPACKET";
    std::for_each(packet.packets.begin(), packet.packets.end(), [=](anh::ByteBuffer& buffer) {
        HandleMessage(buffer);
    });
}

void Session::handleDisconnect_(Disconnect& packet)
{
    VLOG(3) << "Handling DISCONNECT";
    Close();
}

void Session::handlePing_(Ping& packet)
{
    VLOG(3) << "Handling PING";
    Ping pong;
    
    auto buffer = server_->AllocateBuffer();
    pong.serialize(*buffer);

    SendSoePacket_(buffer);
}

void Session::handleNetStatsClient_(NetStatsClient& packet)
{
    VLOG(3) << "Handling NET_STATS_CLIENT";
    server_net_stats_.client_tick_count = packet.client_tick_count;

    auto buffer = server_->AllocateBuffer();
    server_net_stats_.serialize(*buffer);
    SendSoePacket_(buffer);
}

void Session::handleChildDataA_(ChildDataA& packet)
{	
    VLOG(3) << "Handling CHILD_DATA_A";
    if(!SequenceIsValid_(packet.sequence)) {
        DLOG(WARNING) << "Invalid sequence: " << packet.sequence << "; Current sequence " << this->next_client_sequence_;
        return;
    }

    AcknowledgeSequence_(packet.sequence);

    std::for_each(packet.messages.begin(), packet.messages.end(), [this] (shared_ptr<ByteBuffer> message) {
        try {
            auto packet = make_shared<Packet>(this->shared_from_this(), message);

            this->server_->HandleMessage(packet);
        } catch(...) {

        }
    });
}

void Session::handleDataFragA_(DataFragA& packet)
{
    VLOG(3) << "Handling DATA_FRAG_A";
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

            ChildDataA child_data_a(full_packet);
            handleChildDataA_(child_data_a);

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
    VLOG(3) << "Handle ACK_A";

    auto it = find_if(
        sent_messages_.begin(), 
        sent_messages_.end(), 
        [this, &packet] (const SequencedMessageMap::value_type& message) 
    {
        return (message.first == packet.sequence) ? true : false;
    });

    sent_messages_.erase(sent_messages_.begin(), it);

    last_acknowledged_sequence_ = packet.sequence;
}

void Session::handleOutOfOrderA_(OutOfOrderA& packet)
{
    VLOG(3) << "Handle OUT_OF_ORDER_A";
    
    std::for_each(sent_messages_.begin(), sent_messages_.end(), [=](SequencedMessageMap::value_type& item) {
        SendSoePacket_(item.second);
    });
}

void Session::SendSoePacket_(std::shared_ptr<anh::ByteBuffer> message)
{
    server_->SendMessage(shared_from_this(), message);
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
        auto buffer = server_->AllocateBuffer();
        out_of_order.serialize(*buffer);
        SendSoePacket_(buffer);

        return false;
    }
}

void Session::AcknowledgeSequence_(const uint16_t& sequence)
{
    AckA ack(sequence);
    auto buffer = server_->AllocateBuffer();
    ack.serialize(*buffer);
    SendSoePacket_(buffer);

    next_client_sequence_ = sequence + 1;
    current_client_sequence_ = sequence;
}

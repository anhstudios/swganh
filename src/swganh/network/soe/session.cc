// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/network/soe/session.h"

#include <algorithm>

#include "swganh/logger.h"

#include "swganh/network/soe/server_interface.h"

#include "swganh/network/soe/packet_utilities.h"

using namespace swganh;
using namespace swganh::event_dispatcher;
using namespace swganh::network;
using namespace swganh::network::soe;
using namespace std;

Session::Session(ServerInterface* server, boost::asio::io_service& cpu_pool, boost::asio::ip::udp::endpoint remote_endpoint)
    : std::enable_shared_from_this<Session>()
    , remote_endpoint_(remote_endpoint)
    , server_(server)
    , strand_(cpu_pool)
    , connected_(false)
    , crc_seed_(0xDEADBABE)
    , last_acknowledged_sequence_(0)
    , next_client_sequence_(0)
    , current_client_sequence_(0)
    , server_sequence_()
    , server_net_stats_(0, 0, 0, 0, 0, 0)
    , incoming_fragmented_total_len_(0)
    , decompression_filter_(server_->max_receive_size())
    , security_filter_(server_->max_receive_size())
{
    server_sequence_ = 0;
}

Session::~Session(void)
{
    Close();
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

void Session::SendSoePacket_(swganh::ByteBuffer& message)
{
	LOG_NET << "Server -> Client: \n" << message;

    compression_filter_(this, &message);
    encryption_filter_(this, &message);
    crc_output_filter_(this, &message);
	
    // Store it for resending later if necessary
    server_->SendTo(remote_endpoint(), message);
}

void Session::SendFragmentedPacket_(ByteBuffer message, boost::optional<SequencedCallbacks> callbacks)
{
    auto fragments = SplitDataChannelMessage(message, max_data_size());
    
    auto& current_fragment = fragments.front();
    while(fragments.size() > 1) {
        SendSequencedMessage_(&BuildFragmentedDataChannelHeader, std::move(current_fragment));
        fragments.pop_front();
    }

    if(fragments.size() > 0) {
        SendSequencedMessage_(&BuildFragmentedDataChannelHeader, std::move(current_fragment), std::move(callbacks));
    }
}

void Session::SendSequencedMessage_(HeaderBuilder header_builder, ByteBuffer message, boost::optional<SequencedCallbacks> callbacks) {
    // Get the next sequence number
    uint16_t message_sequence = server_sequence_++;

    // Allocate a new packet
    ByteBuffer data_channel_message;
    data_channel_message.append(header_builder(message_sequence));
    data_channel_message.append(move(message));

    // Send it over the wire
    SendSoePacket_(data_channel_message);
    
    unacknowledged_messages_.emplace_back(std::make_tuple(message_sequence, std::move(message), std::move(callbacks)));
}

void Session::SendTo(ByteBuffer message, boost::optional<SequencedCallback> callback)
{    
    uint32_t max_data_channel_size = receive_buffer_size_ - crc_length_ - 3;

    boost::optional<SequencedCallbacks> callbacks;
    if (callback) {
        callbacks = SequencedCallbacks();
        (*callbacks).emplace_back(std::move(*callback));
    }

    if (message.size() > max_data_channel_size) {
        SendFragmentedPacket_(message, std::move(callbacks));
    } else {
        SendSequencedMessage_(&BuildDataChannelHeader, move(message), std::move(callbacks));
    }
}

void Session::Close(void)
{
    if(connected_)
    {
        connected_ = false;

        Disconnect disconnect(connection_id_);
        SendSoePacket_(serialize(disconnect));

        server_->RemoveSession(shared_from_this());

        OnClose();
    }
}

void Session::HandleMessage(swganh::ByteBuffer message)
{
    switch(swganh::bigToHost(message.peek<uint16_t>()))
	{
		case CHILD_DATA_A:	   { handleChildDataA_(ChildDataA(message)); break; }
		case MULTI_PACKET:	   { handleMultiPacket_(MultiPacket(message)); break; }
		case DATA_FRAG_A:	   { handleDataFragA_(DataFragA(message)); break; }
		case ACK_A:			   { handleAckA_(AckA(message)); break; }
		case PING:			   { handlePing_(Ping(message)); break; }
		case NET_STATS_CLIENT: { handleNetStatsClient_(NetStatsClient(message)); break; }
		case OUT_OF_ORDER_A:   { handleOutOfOrderA_(OutOfOrderA(message)); break; }
		case DISCONNECT:	   { handleDisconnect_(Disconnect(message)); break; }
		case SESSION_REQUEST:  { handleSessionRequest_(SessionRequest(message)); break; }
		case FATAL_ERROR:      { Close(); break; }
		default:
			{
				LOG(warning) << "Unhandled message: \n" << message;
			}
	}
}

void Session::HandleProtocolMessage(swganh::ByteBuffer message)
{
    strand_.post(bind(&Session::HandleProtocolMessageInternal, shared_from_this(), move(message)));
}

void Session::HandleProtocolMessageInternal(swganh::ByteBuffer message)
{
    try {
        security_filter_(this, &message);

        if(connected())
        {
            crc_input_filter_(this, &message);
            decryption_filter_(this, &message);
            decompression_filter_(this, &message);
        }
        
        if(message.peek<uint8_t>() != 0) {
            server_->HandleMessage(shared_from_this(), std::move(message));
        } else {
            HandleMessage(move(message));
        }
    } catch(const std::exception& e) {
        LOG(warning) << "Error handling protocol message\n\n" << e.what();
    }
}

void Session::handleSessionRequest_(SessionRequest packet)
{
    connection_id_ = packet.connection_id;
    crc_length_ = packet.crc_length;
    receive_buffer_size_ = packet.client_udp_buffer_size;

    SessionResponse session_response(connection_id_, crc_seed_);
    session_response.server_udp_buffer_size = server_->max_receive_size();

    ByteBuffer buffer;
    session_response.serialize(buffer);

    // Directly put this on the wire, it requires no outgoing processing.
    server_->SendTo(remote_endpoint_, move(buffer));

    connected_ = true;
    LOG(info) << "Created Session [" << connection_id_ << "] @ " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port();
}

void Session::handleMultiPacket_(MultiPacket packet)
{
    for(auto& message : packet.packets) {
        HandleMessage(std::move(message));
    }
}

void Session::handleDisconnect_(Disconnect packet)
{
    Close();
}

void Session::handlePing_(Ping packet)
{
    Ping pong;
    SendSoePacket_(serialize(pong));
}

void Session::handleNetStatsClient_(NetStatsClient packet)
{
    server_net_stats_.client_tick_count = packet.client_tick_count;

    SendSoePacket_(serialize(server_net_stats_));
}

void Session::handleChildDataA_(ChildDataA packet)
{
    if(!AcknowledgeSequence_(packet.sequence)) return;

    for(auto& message : packet.messages) {
        server_->HandleMessage(shared_from_this(), std::move(message));
    }
}

void Session::handleDataFragA_(DataFragA packet)
{
    if(!AcknowledgeSequence_(packet.sequence)) return;
    
    // Continuing a frag
    if(incoming_fragmented_total_len_ > 0) {
        incoming_frag_.append(packet.data);

        if(incoming_fragmented_total_len_ == incoming_frag_.size()) {
            incoming_fragmented_total_len_ = 0;
            server_->HandleMessage(shared_from_this(), std::move(incoming_frag_));
        }
    }
    // Starting a new frag
    else
    {
        incoming_fragmented_total_len_ = packet.data.read<uint16_t>();
        incoming_frag_.append(packet.data);
    }

}

void Session::handleAckA_(AckA packet)
{
    auto& front = unacknowledged_messages_.front();
    while(std::get<0>(front) <= packet.sequence) {
        if (auto& callbacks = std::get<2>(front)) {
            auto current_sequence = std::get<0>(front);
            for(auto& func : *callbacks) {
			    func(current_sequence);
            }
        }

        unacknowledged_messages_.pop_front();
    }

    last_acknowledged_sequence_ = packet.sequence;
}

void Session::handleOutOfOrderA_(OutOfOrderA packet)
{    
    for(const auto& message : unacknowledged_messages_) {
        server_->SendTo(remote_endpoint(), std::get<1>(message));

        if(std::get<0>(message) == packet.sequence) break;
    }
}

bool Session::AcknowledgeSequence_(const uint16_t& sequence)
{
    if(next_client_sequence_ == sequence)
    {
        AckA ack(sequence);
        SendSoePacket_(serialize(ack));

        next_client_sequence_ = sequence + 1;
        current_client_sequence_ = sequence;

        return true;
    }

    if(sequence > (next_client_sequence_ + 50) && next_client_sequence_ > 50) {
		Close();
	} else {
		// Tell the client we have received an Out of Order sequence.
		OutOfOrderA	out_of_order(next_client_sequence_ - 1);
		SendSoePacket_(serialize(out_of_order));        
    }
	
    DLOG(warning) << "Invalid sequence: [" << sequence << "] Current sequence [" << next_client_sequence_ << "]";
	return false;
}

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "session.h"

#include <algorithm>

#include "swganh/logger.h"

#include "server_interface.h"

using namespace swganh;
using namespace swganh::event_dispatcher;
using namespace swganh::network;
using namespace std;

Session::Session(ServerInterface* server, boost::asio::io_service& cpu_pool, boost::asio::ip::udp::endpoint remote_endpoint)
    : std::enable_shared_from_this<Session>()
    , remote_endpoint_(remote_endpoint)
    , server_(server)
    , strand_(cpu_pool)
    , connected_(false)
    , crc_seed_(0xDEADBABE)
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

swganh::ByteBuffer Session::SendSoePacket_(swganh::ByteBuffer message)
{
//	LOG_NET << "S->C: " << server_->listen_endpoint() << " -> " << remote_endpoint() << "\n" << message;
    
    compression_filter_(this, &message);
    encryption_filter_(this, &message);
    crc_output_filter_(this, &message);
	
    // Store it for resending later if necessary
    server_->SendTo(remote_endpoint(), message);
    
    return message;
}

void Session::SendFragmentedPacket_(ByteBuffer message, SequencedCallbacks callbacks)
{
    uint32_t message_size = message.size();  
    uint32_t remaining_bytes = message_size;
    uint32_t max_size = max_data_size();
    uint32_t chunk_size = 0;

    if (message_size < max_size) {
        throw std::invalid_argument("Message must be bigger than max size to fragment");
    }

    while (remaining_bytes != 0) {        
        ByteBuffer fragment;
        chunk_size = (remaining_bytes > max_size) ? max_size : remaining_bytes;

        if (remaining_bytes == message_size) {
            chunk_size -= sizeof(uint32_t);
            fragment.write(hostToBig(message_size));
        }
        
        fragment.write(message.data() + (message_size - remaining_bytes), chunk_size);
        remaining_bytes -= chunk_size;

        if (remaining_bytes == 0) {
            SendSequencedMessage_(0x0D, std::move(fragment), std::move(callbacks));
        } else {
            SendSequencedMessage_(0x0D, std::move(fragment));
        }
    }
}

void Session::SendSequencedMessage_(uint16_t header, ByteBuffer message, SequencedCallbacks callbacks)
{
    auto shared_message = std::make_shared<ByteBuffer>(std::move(message));
    auto shared_callbacks = std::make_shared<SequencedCallbacks>(std::move(callbacks));
    strand_.post([this, header, shared_message, shared_callbacks] () {
        // Get the next sequence number
        uint16_t message_sequence = server_sequence_++;

        // Allocate a new packet
        ByteBuffer data_channel_message;
        data_channel_message.write(hostToBig(header));
        data_channel_message.write(hostToBig(message_sequence));
        data_channel_message.append(move(*shared_message));

        // Send it over the wire
        SendSoePacket_(data_channel_message);
        unacknowledged_messages_.emplace_back(std::make_tuple(message_sequence, std::move(data_channel_message), std::move(*shared_callbacks)));
    });
}

void Session::SendTo(ByteBuffer message, boost::optional<SequencedCallback> callback)
{
    if(!connected()) {
        LOG(warning) << "Attempting to send to a non-connected client";
        return;
    }

    SequencedCallbacks callbacks;
    if (callback) {
        callbacks.emplace_back(std::move(*callback));
    }

    SendTo(std::move(message), std::move(callbacks));
}

void Session::SendTo(ByteBuffer message, SequencedCallbacks callbacks)
{
    if(!connected()) {
        LOG(warning) << "Attempting to send to a non-connected client";
        return;
    }

    if (message.size() > max_data_size()) {
        SendFragmentedPacket_(std::move(message), std::move(callbacks));
    } else {
        SendSequencedMessage_(0x09, std::move(message), std::move(callbacks));
    }
}

void Session::Close(void)
{
    if(connected_)
    {
        connected_ = false;

        Disconnect disconnect(connection_id_);
        SendSoePacket_(Serialize(disconnect));

        server_->RemoveSession(shared_from_this());

        OnClose();
    }
}

void Session::HandleMessage(ByteBuffer message)
{
    switch(bigToHost(message.read<uint16_t>()))
	{
		case CHILD_DATA_A:	   { handleChildDataA_(Deserialize<ChildDataA>(message)); break; }
		case MULTI_PACKET:	   { handleMultiPacket_(Deserialize<MultiPacket>(message)); break; }
		case DATA_FRAG_A:	   { handleDataFragA_(Deserialize<DataFragA>(message)); break; }
		case ACK_A:			   { handleAckA_(Deserialize<AckA>(message)); break; }
		case PING:			   { handlePing_(Deserialize<Ping>(message)); break; }
		case NET_STATS_CLIENT: { handleNetStatsClient_(Deserialize<NetStatsClient>(message)); break; }
		case OUT_OF_ORDER_A:   { handleOutOfOrderA_(Deserialize<OutOfOrderA>(message)); break; }
		case DISCONNECT:	   { handleDisconnect_(Deserialize<Disconnect>(message)); break; }
		case SESSION_REQUEST:  { handleSessionRequest_(Deserialize<SessionRequest>(message)); break; }
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
	    
//        LOG_NET << "C->S: " << remote_endpoint() << " -> " << server_->listen_endpoint() << "\n" << message;
        
        if(message.peek<uint8_t>() != 0) {
            server_->HandleMessage(shared_from_this(), std::move(message));
        } else {
            HandleMessage(move(message));
        }
    } catch(const std::exception& e) {
        LOG(warning) << "Error handling protocol message: " << e.what();
    }
}

void Session::handleSessionRequest_(SessionRequest packet)
{
    connection_id_ = packet.connection_id;
    crc_length_ = packet.crc_length;
    receive_buffer_size_ = packet.client_udp_buffer_size;

    SessionResponse session_response(connection_id_, crc_seed_);
    session_response.server_udp_buffer_size = server_->max_receive_size();

    // Directly put this on the wire, it requires no outgoing processing.
    server_->SendTo(remote_endpoint_, Serialize(session_response));

    connected_ = true;
    LOG(info) << "Created Session [" << connection_id_ << "] @ " << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port();
}

void Session::handleMultiPacket_(MultiPacket packet)
{
    for(auto& message : packet.packets) {
        if(message.peek<uint8_t>() != 0) {
            server_->HandleMessage(shared_from_this(), std::move(message));
        } else {
            HandleMessage(std::move(message));
        }
    }
}

void Session::handleDisconnect_(Disconnect packet)
{
    Close();
}

void Session::handlePing_(Ping packet)
{
    Ping pong;
    SendSoePacket_(Serialize(pong));
}

void Session::handleNetStatsClient_(NetStatsClient packet)
{
    server_net_stats_.client_tick_count = packet.client_tick_count;

    SendSoePacket_(Serialize(server_net_stats_));
}

void Session::handleChildDataA_(ChildDataA packet)
{
    if(!AcknowledgeSequence_(packet.sequence)) return;

    for(auto& message : packet.messages) {
        auto shared_message = std::make_shared<ByteBuffer>(std::move(message));
        strand_.post([this, shared_message] () {
            server_->HandleMessage(shared_from_this(), std::move(*shared_message));
        });
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
    auto end = unacknowledged_messages_.end();
    auto it = unacknowledged_messages_.begin();
    while(it != end) {
        auto current_sequence = std::get<0>(*it);
        if(current_sequence > packet.sequence) break;
        for(auto& func : std::get<2>(*it)) {
            func(current_sequence);
        }

        unacknowledged_messages_.erase(it++);
    }
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
        SendSoePacket_(Serialize(ack));
        
        current_client_sequence_ = sequence;
        next_client_sequence_ = current_client_sequence_ + 1;

        return true;
    }

    if(sequence > (next_client_sequence_ + 50) && next_client_sequence_ > 50) {
		Close();
	} else {
		// Tell the client we have received an Out of Order sequence.
		OutOfOrderA	out_of_order(current_client_sequence_);
		SendSoePacket_(Serialize(out_of_order));        
    }
	
    DLOG(warning) << "Invalid sequence: [" << sequence << "] Current sequence [" << next_client_sequence_ << "]";
	return false;
}

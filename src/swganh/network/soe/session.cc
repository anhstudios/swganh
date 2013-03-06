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
    , incoming_fragmented_curr_len_(0)
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

vector<ByteBuffer> Session::GetUnacknowledgedMessages() const {
    vector<ByteBuffer> unacknowledged_messages;

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
    uint32_t message_count = outgoing_data_messages_.unsafe_size();
    list<ByteBuffer> process_list;
	SequencedCallbacks callbacks;
    OutgoingMessage tmp;

    for (uint32_t i = 0; i < message_count; ++i) {
        if (outgoing_data_messages_.try_pop(tmp)) {
			process_list.push_back(tmp.first);
			if(tmp.second.is_initialized())
				callbacks.push_back(tmp.second.get());
        }
    }

    // Pack the message list into a single data channel payload and send it.
    ByteBuffer data_channel_payload = PackDataChannelMessages(std::move(process_list));

    // Split up the message if it's too big
    // \note: in determining the max size 3 is the size of the soe header + the compression flag.
    uint32_t max_data_channel_size = receive_buffer_size_ - crc_length_ - 3;

    if (data_channel_payload.size() > max_data_channel_size) {
        list<ByteBuffer> fragmented_message = SplitDataChannelMessage(
            data_channel_payload,
            max_data_channel_size);

		uint16_t frag_list_size = fragmented_message.size();
        for_each(fragmented_message.begin(), fragmented_message.end(), [this, &callbacks, &frag_list_size] (ByteBuffer& fragment) {
 
			if(frag_list_size > 1) {
				SendSequencedMessage_(&BuildFragmentedDataChannelHeader, move(fragment), SequencedCallbacks());
			}
			else
			{
				SendSequencedMessage_(&BuildFragmentedDataChannelHeader, move(fragment), callbacks);
			}
			frag_list_size--;
        });
    } else {
        SendSequencedMessage_(&BuildDataChannelHeader, move(data_channel_payload), callbacks);
    }
}

void Session::SendTo(ByteBuffer message, boost::optional<SequencedCallback> callback)
{
    outgoing_data_messages_.push(OutgoingMessage(move(message), callback));
}

void Session::Close(void)
{
    if(connected_)
    {
        connected_ = false;

        Disconnect disconnect(connection_id_);
        ByteBuffer buffer;

        disconnect.serialize(buffer);
        SendSoePacket_(move(buffer));

        auto this_session = shared_from_this();
        server_->RemoveSession(this_session);

        OnClose();
    }
}

ServerInterface* Session::server()
{
    return server_;
}

void Session::HandleMessage(swganh::ByteBuffer message)
{
    strand_.post(bind(&Session::HandleMessageInternal, shared_from_this(), move(message)));
}

void Session::HandleMessageInternal(swganh::ByteBuffer message)
{
	// Sanity Check
	if (message.size() > 0)
	{
		uint16_t soe_opcode = 0;
		try {

			soe_opcode = message.peek<uint16_t>(true);
			switch(soe_opcode)
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
					if (message.peek<uint8_t>() != 0)
					{
						server_->HandleMessage(shared_from_this(), move(message));
					}
					else
					{
						DLOG(warning) << "Unhandled SOE Opcode: "
							<< std::hex << soe_opcode << "\n\n" << message;
					}
			}
		} catch(const std::exception& e) {
			LOG(warning) << "Error handling protocol message "
				<< std::hex << soe_opcode << "\n\n" << e.what();
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

        if (connected())
        {
            crc_input_filter_(this, &message);
            decryption_filter_(this, &message);
            decompression_filter_(this, &message);
        }

        HandleMessage(move(message));
    } catch(const std::exception& e) {
        LOG(warning) << "Error handling protocol message\n\n" << e.what();
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

	if(callbacks.get().size() > 0)
		QueueSequencedCallback(message_sequence, callbacks.get());

    // Store it for resending later if necessary
    sent_messages_.push_back(make_pair(message_sequence, move(data_channel_message)));
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
    std::for_each(
        begin(packet.packets),
        end(packet.packets),
        [=](swganh::ByteBuffer& buffer)
    {
        HandleMessage(move(buffer));
    });
}

void Session::handleDisconnect_(Disconnect packet)
{
    Close();
}

void Session::handlePing_(Ping packet)
{
    Ping pong;

    ByteBuffer buffer;
    pong.serialize(buffer);
    SendSoePacket_(std::move(buffer));
}

void Session::handleNetStatsClient_(NetStatsClient packet)
{
    server_net_stats_.client_tick_count = packet.client_tick_count;

    ByteBuffer buffer;
    server_net_stats_.serialize(buffer);
    SendSoePacket_(std::move(buffer));
}

void Session::handleChildDataA_(ChildDataA packet)
{
    if(!SequenceIsValid_(packet.sequence)) {
        LOG(warning) << "Invalid sequence: " << packet.sequence << "; Current sequence " << this->next_client_sequence_;
        return;
    }

    AcknowledgeSequence_(packet.sequence);

    std::for_each(
        begin(packet.messages),
        end(packet.messages),
        [this] (ByteBuffer& message)
    {
        server_->HandleMessage(this->shared_from_this(), std::move(message));
    });
}

void Session::handleDataFragA_(DataFragA packet)
{
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
        incoming_fragmented_messages_.push_back(swganh::ByteBuffer(packet.data.data()+2, packet.data.size()-2));
    }

}

void Session::handleAckA_(AckA packet)
{
    auto it = find_if(
        sent_messages_.begin(),
        sent_messages_.end(),
        [this, &packet] (const SequencedMessageMap::value_type& message)
    {
        return (message.first == packet.sequence) ? true : false;
    });

    sent_messages_.erase(sent_messages_.begin(), it);

    last_acknowledged_sequence_ = packet.sequence;
	DequeueSequencedCallback(packet.sequence);
}

void Session::handleOutOfOrderA_(OutOfOrderA packet)
{
    std::for_each(
        begin(sent_messages_),
        end(sent_messages_),
        [=](const SequencedMessageMap::value_type& item)
    {
        SendSoePacket_(item.second);
    });
}

void Session::SendSoePacket_(swganh::ByteBuffer message)
{
    strand_.post(bind(&Session::SendSoePacketInternal, shared_from_this(), std::move(message)));
}

void Session::SendSoePacketInternal(swganh::ByteBuffer message)
{
	LOG_NET << "Server -> Client: \n" << message;

    compression_filter_(this, &message);
    encryption_filter_(this, &message);
    crc_output_filter_(this, &message);
	
    server_->SendTo(remote_endpoint(), move(message));

	
}

bool Session::SequenceIsValid_(const uint16_t& sequence)
{
    if(next_client_sequence_ == sequence)
    {
        return true;
    }
	// are we more than 50 packets out of sequence?
	else if(sequence > (next_client_sequence_ + 50) )
	{
		Close();
	}
    else if (next_client_sequence_ < sequence)
    {
		// Tell the client we have received an Out of Order sequence.
		OutOfOrderA	out_of_order(sequence);
		ByteBuffer buffer;
		out_of_order.serialize(buffer);
		encryption_filter_(this, &buffer);
		SendSoePacket_(move(buffer));        
    }
	
	return false;
}

void Session::AcknowledgeSequence_(const uint16_t& sequence)
{
    AckA ack(sequence);
    ByteBuffer buffer;
    ack.serialize(buffer);
    SendSoePacket_(move(buffer));

    next_client_sequence_ = sequence + 1;
    current_client_sequence_ = sequence;
}

void Session::QueueSequencedCallback(uint16_t sequence, SequencedCallbacks callbacks)
{
	auto iter = acknowledgement_callbacks_.find(sequence);
	if(iter == acknowledgement_callbacks_.end())
	{
		acknowledgement_callbacks_.insert(std::pair<uint16_t, SequencedCallbacks>(sequence, callbacks));
	}
	else
		LOG(error) << "Sequence callback is slotted but has not been dequeued.";
}

void Session::DequeueSequencedCallback(uint16_t sequence)
{
	for(std::map<uint16_t, SequencedCallbacks>::iterator iter = acknowledgement_callbacks_.begin(); iter != acknowledgement_callbacks_.end(); iter++)
	{
		if((*iter).first > sequence)
			break;

		for(auto& func : (*iter).second)
		{
			func((*iter).first);
		}
	}
	
	auto iter = acknowledgement_callbacks_.find(sequence);
	if(iter != acknowledgement_callbacks_.end()) {
		acknowledgement_callbacks_.erase(acknowledgement_callbacks_.begin(), iter);
		acknowledgement_callbacks_.erase(iter);
	}
}

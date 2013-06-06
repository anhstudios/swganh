// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <algorithm>
#include <list>
#include <memory>

#include "swganh/byte_buffer.h"
#include "swganh/utilities.h"

namespace swganh {
namespace network {

enum ProtocolOpcodes
{
	SESSION_REQUEST = 1,
	SESSION_RESPONSE,
	MULTI_PACKET,
	NOT_USED,
	DISCONNECT,
	PING,
	NET_STATS_CLIENT,
	NET_STATS_SERVER,
	CHILD_DATA_A,
	CHILD_DATA_B,
	CHILD_DATA_C,
	CHILD_DATA_D,
	DATA_FRAG_A,
	DATA_FRAG_B,
	DATA_FRAG_C,
	DATA_FRAG_D,
	OUT_OF_ORDER_A,
	OUT_OF_ORDER_B,
	OUT_OF_ORDER_C,
	OUT_OF_ORDER_D,
	ACK_A,
	ACK_B,
	ACK_C,
	ACK_D,
	UNKNOWN_A,
	UNKNOWN_B,
	UNKNOWN_C,
	UNKNOWN_D,
	FATAL_ERROR,
	FATAL_ERROR_RESPONSE
};

struct SessionRequest
{
    uint32_t crc_length;
    uint32_t connection_id;
    uint32_t client_udp_buffer_size;

    SessionRequest() {}

    SessionRequest(uint32_t crc_length_, uint32_t connection_id_, uint32_t client_udp_buffer_size_)
        : crc_length(crc_length_)
        , connection_id(connection_id_)
        , client_udp_buffer_size(client_udp_buffer_size_) {}

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(SESSION_REQUEST));
        buffer.write(hostToBig(crc_length));
        buffer.write(hostToBig(connection_id));
        buffer.write(hostToBig(client_udp_buffer_size));
    }

    void Deserialize(swganh::ByteBuffer& buffer) {
        crc_length = bigToHost(buffer.read<uint32_t>());
        connection_id = bigToHost(buffer.read<uint32_t>());
        client_udp_buffer_size = bigToHost(buffer.read<uint32_t>());
    }
};

struct SessionResponse
{
    uint32_t connection_id;
    uint32_t crc_seed;
    uint8_t	crc_length;
    uint8_t	encryption_type;
    uint8_t seed_length;
    uint32_t server_udp_buffer_size;

    SessionResponse() {}

    SessionResponse(uint32_t connection_id_, uint32_t crc_seed_, uint8_t encryption_type_ = 1, uint32_t server_udp_buffer_size = 496)
        : connection_id(connection_id_)
        , crc_seed(crc_seed_)
        , crc_length(2)
        , encryption_type(encryption_type_)
        , seed_length(4)
        , server_udp_buffer_size(server_udp_buffer_size)
    {}

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(SESSION_RESPONSE));
        buffer.write(hostToBig(connection_id));
        buffer.write(hostToBig(crc_seed));
        buffer.write(crc_length);
        buffer.write(encryption_type);
        buffer.write(uint8_t(4));
        buffer.write(hostToBig(server_udp_buffer_size));
    }

    void Deserialize(ByteBuffer& buffer) {
        connection_id = bigToHost(buffer.read<uint32_t>());
        crc_seed = bigToHost(buffer.read<uint32_t>());
        crc_length = buffer.read<uint8_t>();
        encryption_type = buffer.read<uint8_t>();
        seed_length = buffer.read<uint8_t>();
        server_udp_buffer_size = bigToHost(buffer.read<uint32_t>());
    }
};

struct MultiPacket
{
    std::list<ByteBuffer> packets;

    MultiPacket() {}

    explicit MultiPacket(std::list<ByteBuffer> packets_)
        : packets(packets_) {}
    
    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(MULTI_PACKET));
        std::for_each(packets.begin(), packets.end(), [=, &buffer](ByteBuffer& other_buffer) {
            buffer.write<uint8_t>(other_buffer.size());
            buffer.append(other_buffer);
        });
    }

    void Deserialize(ByteBuffer& buffer) {
        while(buffer.read_position() < buffer.size()) {
            uint16_t next_chunk_size = buffer.read<uint8_t>();
            // Varify that we have enough bytes left to copy.
            if(buffer.size() - buffer.read_position() >= next_chunk_size) {
                packets.emplace_back(buffer.data() + buffer.read_position(), next_chunk_size);
                buffer.read_position(buffer.read_position() + next_chunk_size);            
            } else {
                break;
            }
        }
    }
};

struct Disconnect
{
    uint32_t connection_id;
    uint16_t reason_id;

    Disconnect() {}

    explicit Disconnect(uint32_t connection_id_, uint16_t reason_id_ = 6)
        : connection_id(connection_id_)
        , reason_id(reason_id_) { }

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(DISCONNECT));
        buffer.write(hostToBig(connection_id));
        buffer.write(hostToBig(reason_id));
    }

    void Deserialize(ByteBuffer& buffer) {
        connection_id = bigToHost(buffer.read<uint32_t>());
        reason_id = bigToHost(buffer.read<uint16_t>());
    }
};

struct Ping
{
    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(PING));
    }

    void Deserialize(ByteBuffer& buffer) {}
};

struct NetStatsClient
{
    uint16_t client_tick_count;
    uint32_t last_update;
    uint32_t average_update;
    uint32_t shortest_update;
    uint32_t longest_update;
    uint64_t packets_sent;
    uint64_t packets_received;

    NetStatsClient()
        : client_tick_count(0)
        , last_update(0)
        , average_update(0)
        , shortest_update(0)
        , longest_update(0)
        , packets_sent(0)
        , packets_received(0) {}

    NetStatsClient(uint16_t client_tick_count_, uint32_t last_update_, uint32_t average_update_,
        uint32_t shortest_update_, uint32_t longest_update_, uint64_t packets_sent_, uint64_t packets_received_)
        : client_tick_count(client_tick_count_)
        , last_update(last_update_)
        , average_update(average_update_)
        , shortest_update(shortest_update_)
        , longest_update(longest_update_)
        , packets_sent(packets_sent_)
        , packets_received(packets_received_) { }

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(NET_STATS_CLIENT));
        buffer.write(hostToBig(client_tick_count));
        buffer.write(hostToBig(last_update));
        buffer.write(hostToBig(average_update));
        buffer.write(hostToBig(shortest_update));
        buffer.write(hostToBig(longest_update));
        buffer.write(hostToBig(packets_sent));
        buffer.write(hostToBig(packets_received));
    }

    void Deserialize(ByteBuffer& buffer) {
        client_tick_count = bigToHost(buffer.read<uint16_t>());
        last_update = bigToHost(buffer.read<uint32_t>());
        average_update = bigToHost(buffer.read<uint32_t>());
        shortest_update = bigToHost(buffer.read<uint32_t>());
        longest_update = bigToHost(buffer.read<uint32_t>());
        packets_sent = bigToHost(buffer.read<uint64_t>());
        packets_received = bigToHost(buffer.read<uint64_t>());
    }
};

struct NetStatsServer
{
    uint16_t client_tick_count;
    uint32_t server_tick_count;
    uint64_t client_packets_sent;
    uint64_t client_packets_received;
    uint64_t server_packets_sent;
    uint64_t server_packets_received;

    NetStatsServer()
        : client_tick_count(0)
        , server_tick_count(0)
        , client_packets_sent(0)
        , client_packets_received(0)
        , server_packets_sent(0)
        , server_packets_received(0) {}

    NetStatsServer(uint16_t client_tick_count_, uint32_t server_tick_count_, uint64_t client_packets_sent_, uint64_t client_packets_receieved_, uint64_t server_packets_sent_,uint64_t server_packets_received_)
        : client_tick_count(client_tick_count_)
        , server_tick_count(server_tick_count_)
        , client_packets_sent(client_packets_sent_)
        , client_packets_received(client_packets_receieved_)
        , server_packets_sent(server_packets_sent_)
        , server_packets_received(server_packets_received_) { }

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(NET_STATS_SERVER));
        buffer.write(hostToBig(client_tick_count));
        buffer.write(hostToBig(server_tick_count));
        buffer.write(hostToBig(client_packets_sent));
        buffer.write(hostToBig(client_packets_received));
        buffer.write(hostToBig(server_packets_sent));
        buffer.write(hostToBig(server_packets_received));
    }

    void Deserialize(ByteBuffer& buffer) {
        client_tick_count = bigToHost(buffer.read<uint16_t>());
        server_tick_count = bigToHost(buffer.read<uint32_t>());
        client_packets_sent = bigToHost(buffer.read<uint64_t>());
        client_packets_received = bigToHost(buffer.read<uint64_t>());
        server_packets_sent = bigToHost(buffer.read<uint64_t>());
        server_packets_received = bigToHost(buffer.read<uint64_t>());
    }
};

struct ChildDataA
{
    uint16_t sequence;
    uint16_t priority;
    std::list<swganh::ByteBuffer> messages;

    ChildDataA() {}

    explicit ChildDataA(uint16_t sequence_)
        : sequence(sequence_) { }

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(CHILD_DATA_A));
        buffer.write(hostToBig(sequence));
        
        // If there is only one message then no need to pack, just move the message
        // into the output buffer.
        if (messages.size() == 1) {        
            buffer.append(messages.front());
        }
        else
        {
            // Otherwise we need to prepend a header to the whole thing (0x0019)
            buffer.write<uint16_t>(hostToBig<uint16_t>(0x19));
            
            // Then loop through each message and append it to the output buffer with a size prefix.
            for(const auto& message : messages)
            {
                // For messages with a size greater than 254 bytes an 8 byte int is not large enough to
                // hold the size value, in this case we need a little endian uint16_t size.
                if (message.size() >= 255) {
                    buffer.write<uint8_t>(0xFF);
                    buffer.write<uint16_t>(hostToBig<uint16_t>(message.size()));
                } else {
                    buffer.write<uint8_t>(message.size());
                }

                buffer.append(message);
            }
        }
    }

    void Deserialize(ByteBuffer& buffer) {
        sequence = bigToHost(buffer.read<uint16_t>());
        priority = bigToHost(buffer.read<uint16_t>());
        
        // Check for multi-data packet.
        if(priority == 0x0019) {
            while(buffer.read_position() < buffer.size()) {
                uint8_t next_chunk_size = buffer.read<uint8_t>();

                // Varify that we have enough bytes left to copy.
                if((buffer.size()) - buffer.read_position() >= next_chunk_size) {
                    messages.emplace_back(buffer.data() + buffer.read_position(), next_chunk_size);
                    buffer.read_position(buffer.read_position() + next_chunk_size);
                } else {
                    break;
                }
            }
        } else {
            messages.emplace_back(buffer.data() + 4, buffer.size() - 4);
        }
    }
};

struct DataFragA
{
    uint16_t sequence;
    ByteBuffer data;

    DataFragA() {}

    DataFragA(uint16_t sequence_, ByteBuffer& buf_)
        : sequence(sequence_)
        , data(buf_) { }

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(DATA_FRAG_A));
        buffer.write(hostToBig(sequence));
        buffer.append(data);
    }

    void Deserialize(swganh::ByteBuffer& buffer) {
        sequence = bigToHost(buffer.read<uint16_t>());
        data = ByteBuffer(buffer.data(), buffer.size()-7);
    }
};

struct OutOfOrderA
{
    uint16_t sequence;

    OutOfOrderA() {}

    explicit OutOfOrderA(uint16_t sequence_)
        : sequence(sequence_) { }

    void Serialize(ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(OUT_OF_ORDER_A));
        buffer.write(hostToBig(sequence));
    }

    void Deserialize(ByteBuffer& buffer) {
        sequence = bigToHost(buffer.read<uint16_t>());
    }
};

struct AckA
{
    uint16_t sequence;

    AckA() {}

    explicit AckA(uint16_t sequence_)
        : sequence(sequence_) {}

    void Serialize(swganh::ByteBuffer& buffer) {
        buffer.write(hostToBig<uint16_t>(ACK_A));
        buffer.write(hostToBig(sequence));
    }

    void Deserialize(ByteBuffer& buffer) {
        sequence = bigToHost(buffer.read<uint16_t>());
    }
};

struct FatalError
{
    uint16_t	soe_opcode;
};

struct FatalErrorResponse
{
    uint16_t	soe_opcode;
};

}} // namespace swganh::network

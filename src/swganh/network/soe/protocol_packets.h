// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <algorithm>
#include <list>
#include <memory>

#include <swganh/byte_buffer.h>
#include <swganh/utilities.h>
#include <swganh/network/soe/protocol_opcodes.h>
#include "swganh/network/soe/packet_utilities.h"

namespace swganh {
namespace network {
namespace soe {
    
struct SessionRequest
{
    SessionRequest(uint32_t crc_length_, uint32_t connection_id_, uint32_t client_udp_buffer_size_)
        : soe_opcode(SESSION_REQUEST)
        , crc_length(crc_length_)
        , connection_id(connection_id_)
        , client_udp_buffer_size(client_udp_buffer_size_) { }

    SessionRequest(swganh::ByteBuffer& buffer) {
        deserialize(buffer);
    }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(crc_length));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(connection_id));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(client_udp_buffer_size));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        crc_length = buffer.read<uint32_t>(true);
        connection_id = buffer.read<uint32_t>(true);
        client_udp_buffer_size = buffer.read<uint32_t>(true);
    }

    uint16_t	soe_opcode;
    uint32_t	crc_length;
    uint32_t	connection_id;
    uint32_t	client_udp_buffer_size;
};

struct SessionResponse
{
    SessionResponse(uint32_t connection_id_, uint32_t crc_seed_, uint8_t encryption_type_ = 1, uint32_t server_udp_buffer_size = 496)
        : soe_opcode(SESSION_RESPONSE)
        , connection_id(connection_id_)
        , crc_seed(crc_seed_)
        , crc_length(2)
        , encryption_type(encryption_type_)
        , seed_length(4)
        , server_udp_buffer_size(server_udp_buffer_size)
    {
    }

    SessionResponse(ByteBuffer& buffer) {
        deserialize(buffer);
    }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::hostToBig<uint16_t>(soe_opcode));
        buffer.write<uint32_t>(swganh::hostToBig<uint32_t>(connection_id));
        buffer.write<uint32_t>(swganh::hostToBig<uint32_t>(crc_seed));
        buffer.write<uint8_t>(crc_length);
        buffer.write<uint8_t>(encryption_type);
        buffer.write<uint8_t>(4);
        buffer.write<uint32_t>(swganh::hostToBig<uint32_t>(server_udp_buffer_size));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        connection_id = buffer.read<uint32_t>();
        crc_seed = buffer.read<uint32_t>(true);
        crc_length = buffer.read<uint8_t>();
        encryption_type = buffer.read<uint8_t>();
        seed_length = buffer.read<uint8_t>();
        server_udp_buffer_size = buffer.read<uint32_t>(true);
    }

    uint16_t	soe_opcode;
    uint32_t	connection_id;
    uint32_t	crc_seed;
    uint8_t		crc_length;
    uint8_t		encryption_type;
    uint8_t		seed_length;
    uint32_t	server_udp_buffer_size;
};

struct MultiPacket
{
    MultiPacket(std::list<ByteBuffer> packets_)
        : soe_opcode(MULTI_PACKET)
        , packets(packets_) {}

    MultiPacket(swganh::ByteBuffer& buffer) {
        deserialize(buffer); 
    }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        std::for_each(packets.begin(), packets.end(), [=, &buffer](ByteBuffer& other_buffer) {
            buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(other_buffer.size()));
            buffer.append(other_buffer);
        });
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        while(buffer.read_position() < buffer.size()) {
            uint16_t next_chunk_size = buffer.read<uint8_t>();
            // Varify that we have enough bytes left to copy.
            if(buffer.size() - buffer.read_position() >= next_chunk_size) {    
                swganh::ByteBuffer chunk(buffer.data() + buffer.read_position(), next_chunk_size);
                packets.push_back(chunk);
                buffer.read_position(buffer.read_position() + next_chunk_size);            
            } else {
                break;
            }
        }
    }

    uint16_t	soe_opcode;
    std::list<ByteBuffer>	packets;
};

struct Disconnect
{
    Disconnect(uint32_t connection_id_, uint16_t reason_id_ = 6)
        : soe_opcode(DISCONNECT)
        , connection_id(connection_id_)
        , reason_id(reason_id_) { }

    Disconnect(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(connection_id));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(reason_id));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        connection_id = buffer.read<uint32_t>(true);
        reason_id = buffer.read<uint16_t>(true);
    }

    uint16_t	soe_opcode;
    uint32_t	connection_id;
    uint16_t	reason_id;
};

struct Ping
{
    Ping() 
        : soe_opcode(PING)
        { }

    Ping(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::hostToBig<uint16_t>(soe_opcode));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
    }

    uint16_t	soe_opcode;
};

struct NetStatsClient
{
    NetStatsClient(uint16_t client_tick_count_, uint32_t last_update_, uint32_t average_update_,
        uint32_t shortest_update_, uint32_t longest_update_, uint64_t packets_sent_, uint64_t packets_received_)
        : soe_opcode(NET_STATS_CLIENT)
        , client_tick_count(client_tick_count_)
        , last_update(last_update_)
        , average_update(average_update_)
        , shortest_update(shortest_update_)
        , longest_update(longest_update_)
        , packets_sent(packets_sent_)
        , packets_received(packets_received_) { }

    NetStatsClient(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(client_tick_count));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(last_update));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(average_update));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(shortest_update));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(longest_update));
        buffer.write<uint64_t>(swganh::bigToHost<uint64_t>(packets_sent));
        buffer.write<uint64_t>(swganh::bigToHost<uint64_t>(packets_received));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        client_tick_count = buffer.read<uint16_t>(true);
        last_update = buffer.read<uint32_t>(true);
        average_update = buffer.read<uint32_t>(true);
        shortest_update = buffer.read<uint32_t>(true);
        longest_update = buffer.read<uint32_t>(true);
        packets_sent = buffer.read<uint64_t>(true);
        packets_received = buffer.read<uint64_t>(true);
    }

    uint16_t	soe_opcode;
    uint16_t	client_tick_count;
    uint32_t	last_update;
    uint32_t	average_update;
    uint32_t	shortest_update;
    uint32_t	longest_update;
    uint64_t	packets_sent;
    uint64_t	packets_received;
};

struct NetStatsServer
{
    NetStatsServer(uint16_t client_tick_count_, uint32_t server_tick_count_, uint64_t client_packets_sent_, uint64_t client_packets_receieved_, uint64_t server_packets_sent_,uint64_t server_packets_received_)
        : soe_opcode(NET_STATS_SERVER)
        , client_tick_count(client_tick_count_)
        , server_tick_count(server_tick_count_)
        , client_packets_sent(client_packets_sent_)
        , client_packets_received(client_packets_receieved_)
        , server_packets_sent(server_packets_sent_)
        , server_packets_received(server_packets_received_) { }

    NetStatsServer(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(client_tick_count));
        buffer.write<uint32_t>(swganh::bigToHost<uint32_t>(server_tick_count));
        buffer.write<uint64_t>(swganh::bigToHost<uint64_t>(client_packets_sent));
        buffer.write<uint64_t>(swganh::bigToHost<uint64_t>(client_packets_received));
        buffer.write<uint64_t>(swganh::bigToHost<uint64_t>(server_packets_sent));
        buffer.write<uint64_t>(swganh::bigToHost<uint64_t>(server_packets_received));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        client_tick_count = buffer.read<uint16_t>(true);
        server_tick_count = buffer.read<uint32_t>(true);
        client_packets_sent = buffer.read<uint64_t>(true);
        client_packets_received = buffer.read<uint64_t>(true);
        server_packets_sent = buffer.read<uint64_t>(true);
        server_packets_received = buffer.read<uint64_t>(true);
    }

    uint16_t	soe_opcode;
    uint16_t	client_tick_count;
    uint32_t	server_tick_count;
    uint64_t	client_packets_sent;
    uint64_t	client_packets_received;
    uint64_t	server_packets_sent;
    uint64_t	server_packets_received;
};

struct ChildDataA
{
    ChildDataA(uint16_t sequence_)
        : soe_opcode(CHILD_DATA_A)
        , sequence(sequence_) { }

    ChildDataA(swganh::ByteBuffer& buffer) { deserialize(buffer); }
    
    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(sequence));

        // Serialize as a Multi-Data
        buffer.append(PackDataChannelMessages(messages));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        sequence = buffer.read<uint16_t>(true);
        priority = buffer.read<uint16_t>();
        
        // Check for multi-data packet.
        if(swganh::bigToHost<uint16_t>(priority) == 0x0019) {
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
            messages.emplace_back((const unsigned char*)buffer.data() + 4, buffer.size() - 4);
        }
    }

    uint16_t							soe_opcode;
    uint16_t							sequence;
    uint16_t							priority;
    std::list<swganh::ByteBuffer>			messages;
};

struct DataFragA
{
    DataFragA(uint16_t sequence_, ByteBuffer& buf_)
        : soe_opcode(DATA_FRAG_A)
        , sequence(sequence_)
        , data(buf_) { }

    DataFragA(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(sequence));
        buffer.append(data);
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        sequence = buffer.read<uint16_t>(true);
        data = ByteBuffer(buffer.data(), buffer.size()-7);
    }

    uint16_t	soe_opcode;
    uint16_t	sequence;
    ByteBuffer	data;
};

struct OutOfOrderA
{
    OutOfOrderA(uint16_t sequence_)
        : soe_opcode(OUT_OF_ORDER_A)
        , sequence(sequence_) { }

    OutOfOrderA(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(sequence));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        sequence = buffer.read<uint16_t>(true);
    }

    uint16_t	soe_opcode;
    uint16_t	sequence;
};

struct AckA
{
    AckA(uint16_t sequence_)
        : soe_opcode(ACK_A)
        , sequence(sequence_) {}

    AckA(swganh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(swganh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(swganh::bigToHost<uint16_t>(sequence));
    }

    void deserialize(swganh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        sequence = buffer.read<uint16_t>(true);
    }

    uint16_t	soe_opcode;
    uint16_t	sequence;
};

struct FatalError
{
    uint16_t	soe_opcode;
};

struct FatalErrorResponse
{
    uint16_t	soe_opcode;
};

} // namespace soe
} // namespace network
} // namespace swganh

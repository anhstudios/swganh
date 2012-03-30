/*
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

#ifndef ANH_NETWORK_SOE_PROTOCOL_PACKETS_H_
#define ANH_NETWORK_SOE_PROTOCOL_PACKETS_H_

#include <algorithm>
#include <list>
#include <memory>

#include <anh/byte_buffer.h>
#include <anh/utilities.h>
#include <anh/network/soe/protocol_opcodes.h>
#include "anh/network/soe/packet_utilities.h"

namespace anh {
namespace network {
namespace soe {
    
struct SessionRequest
{
    SessionRequest(uint32_t crc_length_, uint32_t connection_id_, uint32_t client_udp_buffer_size_)
        : soe_opcode(SESSION_REQUEST)
        , crc_length(crc_length_)
        , connection_id(connection_id_)
        , client_udp_buffer_size(client_udp_buffer_size_) { }

    SessionRequest(anh::ByteBuffer& buffer) {
        deserialize(buffer);
    }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(crc_length));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(connection_id));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(client_udp_buffer_size));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::hostToBig<uint16_t>(soe_opcode));
        buffer.write<uint32_t>(anh::hostToBig<uint32_t>(connection_id));
        buffer.write<uint32_t>(anh::hostToBig<uint32_t>(crc_seed));
        buffer.write<uint8_t>(crc_length);
        buffer.write<uint8_t>(encryption_type);
        buffer.write<uint8_t>(4);
        buffer.write<uint32_t>(anh::hostToBig<uint32_t>(server_udp_buffer_size));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    MultiPacket(anh::ByteBuffer& buffer) {
        deserialize(buffer); 
    }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        std::for_each(packets.begin(), packets.end(), [=, &buffer](ByteBuffer& other_buffer) {
            buffer.write<uint16_t>(anh::bigToHost<uint16_t>(other_buffer.size()));
            buffer.append(other_buffer);
        });
    }

    void deserialize(anh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        while(buffer.read_position() < buffer.size()) {
            uint16_t next_chunk_size = buffer.read<uint8_t>();
            // Varify that we have enough bytes left to copy.
            if(buffer.size() - buffer.read_position() >= next_chunk_size) {    
                anh::ByteBuffer chunk(buffer.data() + buffer.read_position(), next_chunk_size);
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

    Disconnect(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(connection_id));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(reason_id));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    Ping(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::hostToBig<uint16_t>(soe_opcode));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    NetStatsClient(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(client_tick_count));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(last_update));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(average_update));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(shortest_update));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(longest_update));
        buffer.write<uint64_t>(anh::bigToHost<uint64_t>(packets_sent));
        buffer.write<uint64_t>(anh::bigToHost<uint64_t>(packets_received));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    NetStatsServer(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(client_tick_count));
        buffer.write<uint32_t>(anh::bigToHost<uint32_t>(server_tick_count));
        buffer.write<uint64_t>(anh::bigToHost<uint64_t>(client_packets_sent));
        buffer.write<uint64_t>(anh::bigToHost<uint64_t>(client_packets_received));
        buffer.write<uint64_t>(anh::bigToHost<uint64_t>(server_packets_sent));
        buffer.write<uint64_t>(anh::bigToHost<uint64_t>(server_packets_received));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    ChildDataA(anh::ByteBuffer& buffer) { deserialize(buffer); }
    
    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(sequence));

        // Serialize as a Multi-Data
        buffer.append(PackDataChannelMessages(messages));
    }

    void deserialize(anh::ByteBuffer& buffer) {
        soe_opcode = buffer.read<uint16_t>(true);
        sequence = buffer.read<uint16_t>(true);
        priority = buffer.read<uint16_t>();
        
        // Check for multi-data packet.
        if(anh::bigToHost<uint16_t>(priority) == 0x0019) {
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
    std::list<anh::ByteBuffer>			messages;
};

struct DataFragA
{
    DataFragA(uint16_t sequence_, ByteBuffer& buf_)
        : soe_opcode(DATA_FRAG_A)
        , sequence(sequence_)
        , data(buf_) { }

    DataFragA(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(sequence));
        buffer.append(data);
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    OutOfOrderA(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(sequence));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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

    AckA(anh::ByteBuffer& buffer) { deserialize(buffer); }

    void serialize(anh::ByteBuffer& buffer) {
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(soe_opcode));
        buffer.write<uint16_t>(anh::bigToHost<uint16_t>(sequence));
    }

    void deserialize(anh::ByteBuffer& buffer) {
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
} // namespace anh

#endif // ANH_NETWORK_SOE_PROTOCOL_PACKETS_H_
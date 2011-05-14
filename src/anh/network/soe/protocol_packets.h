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

#include <anh/network/soe/protocol_opcodes.h>
#include <stdint.h>

namespace anh {
namespace network {
namespace soe {

struct SessionRequest
{
	uint32_t	opcode;
	uint32_t	crc_length;
	uint32_t	connection_id;
	uint32_t	client_udp_buffer_size;
};

struct SessionResponse
{
	uint32_t	connection_id;
	uint32_t	crc_seed;
	uint8_t		crc_length;
	uint8_t		encryption_type;
	uint8_t		seed_length;
	uint32_t	server_udp_buffer_size;
};

struct MultiPacket
{
};

struct Disconnect
{
	uint32_t	connection_id;
	uint16_t	reason_id;
};

struct Ping
{
};

struct NetStatsClient
{
	uint16_t	client_tick_count;
	uint32_t	last_update;
	uint32_t	average_update;
	uint32_t	shortest_update;
	uint32_t	longest_update;
	uint64_t	packets_sent;
	uint64_t	packets_recieved;
};

struct NetStatsServer
{
	uint16_t	client_tick_count;
	uint32_t	server_tick_count;
	uint64_t	client_packets_sent;
	uint64_t	client_packets_recieved;
	uint64_t	server_packets_sent;
	uint64_t	server_packets_recieved;
};

struct ChildDataA
{
	uint16_t	sequence;
	char*		data;
};

struct DataFragA
{
	uint16_t	sequence;
	uint16_t	frag_size;
	char*		data;
};

struct OutOfOrderA
{
	uint16_t	sequence;
};

struct AckA
{
	uint16_t	sequence;
};

struct FatalError
{
};

struct FatalErrorResponse
{
};

struct Footer
{
	uint8_t	compressed;
	uint8_t	crc_byte_one;
	uint8_t crc_byte_two;
};

} // namespace soe
} // namespace network
} // namespace anh

#endif // ANH_NETWORK_SOE_PROTOCOL_PACKETS_H_
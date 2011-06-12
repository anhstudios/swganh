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

#ifndef ANH_NETWORK_SOE_PROTOCOL_OPCODES_H_
#define ANH_NETWORK_SOE_PROTOCOL_OPCODES_H_

namespace anh {
namespace network {
namespace soe {

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

} // namespace soe
} // namespace network
} // namespace anh

#endif // ANH_NETWORK_SOE_PROTOCOL_OPCODES_H_ 
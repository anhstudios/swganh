// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SOE_PROTOCOL_OPCODES_H_
#define ANH_NETWORK_SOE_PROTOCOL_OPCODES_H_

namespace swganh {
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
} // namespace swganh

#endif // ANH_NETWORK_SOE_PROTOCOL_OPCODES_H_ 
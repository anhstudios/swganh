// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONNECT_PLAYER_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_CONNECT_PLAYER_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ConnectPlayerResponseMessage : public BaseSwgMessage<ConnectPlayerResponseMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x6137556F; }

    	uint32_t unknown1; // always 5
    	uint32_t unknown2;

    	ConnectPlayerResponseMessage()
    		: unknown1(5)
    	{}

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(unknown1);
    		buffer.write(unknown2);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		unknown1 = buffer.read<uint32_t>();
    		unknown2 = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CONNECT_PLAYER_RESPONSE_MESSAGE_H_

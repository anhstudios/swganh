// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_ENTER_ROOM_BY_ID_H_
#define SWGANH_MESSAGES_CHAT_ENTER_ROOM_BY_ID_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatEnterRoomById : public BaseSwgMessage<ChatEnterRoomById>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xBC6BDDF2; }

    	uint32_t attempts_counter;
    	uint32_t channel_id;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(attempts_counter);
    		buffer.write(channel_id);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		attempts_counter = buffer.read<uint32_t>();
    		channel_id = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ENTER_ROOM_BY_ID_H_

// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_DESTROY_ROOM_H_
#define SWGANH_MESSAGES_CHAT_DESTROY_ROOM_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatDestroyRoom : public BaseSwgMessage<ChatDestroyRoom>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x94B2A77; }

    	uint32_t channel_id;
    	uint32_t attempts_counter;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(channel_id);
    		buffer.write(attempts_counter);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		channel_id = buffer.read<uint32_t>();
    		attempts_counter = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_DESTROY_ROOM_H_

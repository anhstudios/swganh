// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_CREATE_ROOM_H_
#define SWGANH_MESSAGES_CHAT_CREATE_ROOM_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatCreateRoom : public BaseSwgMessage<ChatCreateRoom>
    {
    	static uint16_t Opcount() { return 7; }
    	static uint32_t Opcode() { return 0x35366BED; }

    	uint8_t public_flag; // 0 = private, 1 = public
    	uint8_t moderation_flag; // 0 = unmoderated, 1 = moderated
    	uint16_t unknown;
    	std::string channel_path; // path to the channel, e.g. "swg/<server>/tatooine/<channel_name>"
    	std::string channel_name;
    	uint32_t attempts_counter;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(public_flag);
    		buffer.write(moderation_flag);
    		buffer.write(unknown);
    		buffer.write(channel_path);
    		buffer.write(channel_name);
    		buffer.write(attempts_counter);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		public_flag = buffer.read<uint8_t>();
    		moderation_flag = buffer.read<uint8_t>();
    		unknown = buffer.read<uint16_t>();
    		channel_path = buffer.read<std::string>();
    		channel_name = buffer.read<std::string>();
    		attempts_counter = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_CREATE_ROOM_H_
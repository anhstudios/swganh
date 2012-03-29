// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_SEND_TO_ROOM_H_
#define SWGANH_MESSAGES_CHAT_SEND_TO_ROOM_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatSendToRoom : public BaseSwgMessage<ChatSendToRoom>
    {
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0x20E4DBE3; }

    	std::string sender_character_name;
    	std::wstring message;
    	uint32_t spacer;
    	uint32_t channel_id;
    	uint32_t attempts_counter;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(sender_character_name);
    		buffer.write(message);
    		buffer.write(spacer);
    		buffer.write(channel_id);
    		buffer.write(attempts_counter);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		sender_character_name = buffer.read<std::string>();
    		message = buffer.read<std::wstring>();
    		spacer = buffer.read<uint32_t>();
    		channel_id = buffer.read<uint32_t>();
    		attempts_counter = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_SEND_TO_ROOM_H_
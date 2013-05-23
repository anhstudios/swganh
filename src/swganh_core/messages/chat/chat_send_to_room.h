// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "swganh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatSendToRoom : public BaseSwgMessage
    {
    	uint16_t Opcount() const { return 5; }
    	uint32_t Opcode() const { return 0x20E4DBE3; }

    	std::string sender_character_name;
    	std::wstring message;
    	uint32_t spacer;
    	uint32_t channel_id;
    	uint32_t attempts_counter;
    	
    	void OnSerialize(swganh::ByteBuffer& buffer) const
    	{
    		buffer.write(sender_character_name);
    		buffer.write(message);
    		buffer.write(spacer);
    		buffer.write(channel_id);
    		buffer.write(attempts_counter);
    	}

    	void OnDeserialize(swganh::ByteBuffer& buffer)
    	{
    		sender_character_name = buffer.read<std::string>();
    		message = buffer.read<std::wstring>();
    		spacer = buffer.read<uint32_t>();
    		channel_id = buffer.read<uint32_t>();
    		attempts_counter = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatInstantMessageToClient : public BaseSwgMessage<ChatInstantMessageToClient>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0x3C565CED; }

    	std::string game_name; // arbitrary: "SWG"
    	std::string server_name; // galaxy name
    	std::string sender_character_name; // sender (the recipient receives this packet)
    	std::wstring message;

    	ChatInstantMessageToClient()
    		: game_name("SWG")
    	{}

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(sender_character_name);
    		buffer.write(message);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		sender_character_name = buffer.read<std::string>();
    		message = buffer.read<std::wstring>();
    	}
    };

}} // namespace swganh::messages

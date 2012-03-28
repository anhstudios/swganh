// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_INSTANT_MESSAGE_TO_CHARACTER_H_
#define SWGANH_MESSAGES_CHAT_INSTANT_MESSAGE_TO_CHARACTER_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatInstantMessageToCharacter : public BaseSwgMessage<ChatInstantMessageToCharacter>
    {
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0x84BB21F7; }

    	std::string game_name; // arbitrary: "SWG"
    	std::string server_name; // galaxy name
    	std::string recipient_character_name; // recipient (the server will send ChatInstantMessageToClient to the recipient)
    	std::wstring message;
    	uint32_t unknown;
    	uint32_t sequence_number;

    	ChatInstantMessageToCharacter()
    		: game_name("SWG")
    	{}

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(recipient_character_name);
    		buffer.write(message);
    		buffer.write(unknown);
    		buffer.write(sequence_number);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		recipient_character_name = buffer.read<std::string>();
    		message = buffer.read<std::wstring>();
    		unknown = buffer.read<uint32_t>();
    		sequence_number = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_INSTANT_MESSAGE_TO_CHARACTER_H_

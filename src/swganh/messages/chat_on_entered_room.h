// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_ON_ENTERED_ROOM_H_
#define SWGANH_MESSAGES_CHAT_ON_ENTERED_ROOM_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatOnEnteredRoom : public BaseSwgMessage<ChatOnEnteredRoom>
    {
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0xE69BDC0A; }

    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string character_name;
    	uint32_t success_bitmask;
    	uint32_t channel_id;
    	uint32_t unknown;

    	ChatOnEnteredRoom()
    		: game_name("SWG")
    	{}

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(character_name);
    		buffer.write(success_bitmask);
    		buffer.write(channel_id);
    		buffer.write(unknown);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		character_name = buffer.read<std::string>();
    		success_bitmask = buffer.read<uint32_t>();
    		channel_id = buffer.read<uint32_t>();
    		unknown = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ON_ENTERED_ROOM_H_

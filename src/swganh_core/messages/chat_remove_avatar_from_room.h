// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "swganh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatRemoveAvatarFromRoom : public BaseSwgMessage
    {
    	uint16_t Opcount() const { return 3; }
    	uint32_t Opcode() const { return 0x493E3FFA; }

    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string character_name;
    	std::string channel_path; // path to the channel, e.g. "swg/server/tatooine/<channel_name>" (presumably exclude channel_name)

    	ChatRemoveAvatarFromRoom()
    		: game_name("SWG")
    	{}

    	void OnSerialize(swganh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(character_name);
    		buffer.write(channel_path);
    	}

    	void OnDeserialize(swganh::ByteBuffer& buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		character_name = buffer.read<std::string>();
    		channel_path = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

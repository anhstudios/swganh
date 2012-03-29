// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_REMOVE_AVATAR_FROM_ROOM_H_
#define SWGANH_MESSAGES_CHAT_REMOVE_AVATAR_FROM_ROOM_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatRemoveAvatarFromRoom : public BaseSwgMessage<ChatRemoveAvatarFromRoom>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x493E3FFA; }

    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string character_name;
    	std::string channel_path; // path to the channel, e.g. "swg/server/tatooine/<channel_name>" (presumably exclude channel_name)

    	ChatRemoveAvatarFromRoom()
    		: game_name("SWG")
    	{}

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(character_name);
    		buffer.write(channel_path);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		character_name = buffer.read<std::string>();
    		channel_path = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_REMOVE_AVATAR_FROM_ROOM_H_

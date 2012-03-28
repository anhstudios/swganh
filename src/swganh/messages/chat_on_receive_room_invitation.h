// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_ON_RECEIVE_ROOM_INVITATION_H_
#define SWGANH_MESSAGES_CHAT_ON_RECEIVE_ROOM_INVITATION_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatOnReceiveRoomInvitation : public BaseSwgMessage<ChatOnReceiveRoomInvitation>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xC17EB06D; }

    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string system_string; // arbitrary: "system"
    	std::string channel_name;

    	ChatOnReceiveRoomInvitation()
    		: game_name("SWG")
    	{}

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(system_string);
    		buffer.write(channel_name);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		system_string = buffer.read<std::string>();
    		channel_name = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ON_RECEIVE_ROOM_INVITATION_H_

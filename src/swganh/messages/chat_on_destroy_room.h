// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_ON_DESTROY_ROOM_H_
#define SWGANH_MESSAGES_CHAT_ON_DESTROY_ROOM_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatOnDestroyRoom : public BaseSwgMessage<ChatOnDestroyRoom>
    {
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0xE8EC5877; }
    
    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string system_string; // arbitrary: "system"
    	uint32_t error;
    	uint32_t channel_id;
    	uint32_t request_id;
    
    	ChatOnDestroyRoom()
    		: game_name("SWG")
    	{}
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(system_string);
    		buffer.write(error);
    		buffer.write(channel_id);
    		buffer.write(request_id);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		system_string = buffer.read<std::string>();
    		error = buffer.read<uint32_t>();
    		channel_id = buffer.read<uint32_t>();
    		request_id = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ON_DESTROY_ROOM_H_
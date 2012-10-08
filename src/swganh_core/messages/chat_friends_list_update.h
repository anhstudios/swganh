// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "swganh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatFriendsListUpdate : public BaseSwgMessage
    {
    	uint16_t Opcount() const { return 4; }
    	uint32_t Opcode() const { return 0x6CD2FCD8; }
    
    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string friend_name;
    	uint8_t online_status_flag; // 0 = offline, 1 = online
    
    	ChatFriendsListUpdate()
    		: game_name("SWG")
    	{}
    
    	void OnSerialize(swganh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(friend_name);
    		buffer.write(online_status_flag);
    	}
    
    	void OnDeserialize(swganh::ByteBuffer& buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		friend_name = buffer.read<std::string>();
    		online_status_flag = buffer.read<uint8_t>();
    	}
    };

}} // namespace swganh::messages

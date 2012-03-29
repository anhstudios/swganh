// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CHAT_FRIENDS_LIST_UPDATE_H_
#define SWGANH_MESSAGES_CHAT_FRIENDS_LIST_UPDATE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatFriendsListUpdate : public BaseSwgMessage<ChatFriendsListUpdate>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0x6CD2FCD8; }
    
    	std::string game_name; // default: SWG
    	std::string server_name; // galaxy name
    	std::string friend_name;
    	uint8_t online_status_flag; // 0 = offline, 1 = online
    
    	ChatFriendsListUpdate()
    		: game_name("SWG")
    	{}
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(game_name);
    		buffer.write(server_name);
    		buffer.write(friend_name);
    		buffer.write(online_status_flag);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		game_name = buffer.read<std::string>();
    		server_name = buffer.read<std::string>();
    		friend_name = buffer.read<std::string>();
    		online_status_flag = buffer.read<uint8_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_FRIENDS_LIST_UPDATE_H_

/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_MESSAGES_CHAT_ON_CREATE_ROOM_H_
#define SWGANH_MESSAGES_CHAT_ON_CREATE_ROOM_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ChannelModerator {
	std::string game_name;
	std::string server_name; // galaxy name
	std::string moderator_name;

	ChannelModerator()
		: game_name("SWG")
	{}
};

struct ChannelUser {
	std::string game_name;
	std::string server_name; // galaxy name
	std::string user_name;

	ChannelUser()
		: game_name("SWG")
	{}
};

struct ChatOnCreateRoom : public swganh::messages::BaseSwgMessage<ChatOnCreateRoom> {
	static uint16_t opcount() { return 4; }
	static uint32_t opcode() { return 0x35D7CC9F; }

	uint32_t error; // 0 = success, 6 = cannot create because name invalid, 18 = no message
	uint32_t channel_id;
	uint32_t private_flag; // 0 = public, 1 = private
	uint8_t moderation_flag; // 0 = unmoderated, 1 = moderated
	std::string channel_path; // path to the channel, e.g. "swg/<server>/tatooine/<channel_name>"
	std::string game_name; // arbitrary: "SWG"
	std::string server_name; // galaxy name
	std::string channel_owner_name; // name of character who owns the channel
	std::string channel_creator_name; // name of character who created the channel
	std::wstring channel_name;
	std::list<ChannelModerator> channel_moderators;
	std::list<ChannelUser> channel_users;
	uint32_t request_id;

	ChatOnCreateRoom()
		: game_name("SWG")
	{}

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(error);
		buffer.write(channel_id);
		buffer.write(private_flag);
		buffer.write(moderation_flag);
		buffer.write(channel_path);
		buffer.write(game_name);
		buffer.write(server_name);
		buffer.write(channel_owner_name);
		buffer.write(game_name);
		buffer.write(server_name);
		buffer.write(channel_creator_name);
		buffer.write(channel_name);
		buffer.write<uint32_t>(channel_moderators.size());
		std::for_each(channel_moderators.begin(), channel_moderators.end(), [&buffer] (ChannelModerator moderator) {
			buffer.write(moderator.game_name);
			buffer.write(moderator.server_name);
			buffer.write(moderator.moderator_name);
		});
		buffer.write<uint32_t>(channel_users.size());
		std::for_each(channel_users.begin(), channel_users.end(), [&buffer] (ChannelUser user) {
			buffer.write(user.game_name);
			buffer.write(user.server_name);
			buffer.write(user.user_name);
		});
		buffer.write(request_id);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
        error = buffer.read<uint32_t>();
		channel_id = buffer.read<uint32_t>();
		private_flag = buffer.read<uint32_t>();
		moderation_flag = buffer.read<uint8_t>();
		channel_path = buffer.read<std::string>();
		game_name = buffer.read<std::string>();
		server_name = buffer.read<std::string>();
		channel_owner_name = buffer.read<std::string>();
		game_name = buffer.read<std::string>();
		server_name = buffer.read<std::string>();
		channel_creator_name = buffer.read<std::string>();
		channel_name = buffer.read<std::wstring>();
		uint32_t channel_moderators_count = buffer.read<uint32_t>();
		for(uint32_t i = 0; i < channel_moderators_count; i++) {
			ChannelModerator moderator;
			moderator.game_name = buffer.read<std::string>();
			moderator.server_name = buffer.read<std::string>();
			moderator.moderator_name = buffer.read<std::string>();
			channel_moderators.push_back(moderator);
		}
		uint32_t channel_users_count = buffer.read<uint32_t>();
		for(uint32_t i = 0; i < channel_users_count; i++) {
			ChannelUser user;
			user.game_name = buffer.read<std::string>();
			user.server_name = buffer.read<std::string>();
			user.user_name = buffer.read<std::string>();
			channel_users.push_back(user);
		}
		request_id = buffer.read<uint32_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ON_CREATE_ROOM_H_
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

#ifndef SWGANH_MESSAGES_CHAT_ON_RECEIVE_ROOM_INVITATION_H_
#define SWGANH_MESSAGES_CHAT_ON_RECEIVE_ROOM_INVITATION_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ChatOnReceiveRoomInvitation : public swganh::messages::BaseSwgMessage<ChatOnReceiveRoomInvitation> {
	static uint16_t opcount() { return 3; }
	static uint32_t opcode() { return 0xC17EB06D; }

	std::string game_name; // default: SWG
	std::string server_name; // galaxy name
	std::string system_string; // arbitrary: "system"
	std::string channel_name;

	ChatOnReceiveRoomInvitation()
		: game_name("SWG")
	{}

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(game_name);
		buffer.write(server_name);
		buffer.write(system_string);
		buffer.write(channel_name);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		game_name = buffer.read<std::string>();
		server_name = buffer.read<std::string>();
		system_string = buffer.read<std::string>();
		channel_name = buffer.read<std::string>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ON_RECEIVE_ROOM_INVITATION_H_
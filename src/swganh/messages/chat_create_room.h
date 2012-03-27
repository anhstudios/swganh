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

#ifndef SWGANH_MESSAGES_CHAT_CREATE_ROOM_H_
#define SWGANH_MESSAGES_CHAT_CREATE_ROOM_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ChatCreateRoom : public swganh::messages::BaseSwgMessage<ChatCreateRoom> {
	static uint16_t opcount() { return 7; }
	static uint32_t opcode() { return 0x35366BED; }

	uint8_t public_flag; // 0 = private, 1 = public
	uint8_t moderation_flag; // 0 = unmoderated, 1 = moderated
	uint16_t unknown;
	std::string channel_path; // path to the channel, e.g. "swg/<server>/tatooine/<channel_name>"
	std::string channel_name;
	uint32_t attempts_counter;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(public_flag);
		buffer.write(moderation_flag);
		buffer.write(unknown);
		buffer.write(channel_path);
		buffer.write(channel_name);
		buffer.write(attempts_counter);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		public_flag = buffer.read<uint8_t>();
		moderation_flag = buffer.read<uint8_t>();
		unknown = buffer.read<uint16_t>();
		channel_path = buffer.read<std::string>();
		channel_name = buffer.read<std::string>();
		attempts_counter = buffer.read<uint32_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_CREATE_ROOM_H_
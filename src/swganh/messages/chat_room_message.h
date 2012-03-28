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

#ifndef SWGANH_MESSAGES_CHAT_ROOM_MESSAGE_H_
#define SWGANH_MESSAGES_CHAT_ROOM_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ChatRoomMessage : public swganh::messages::BaseSwgMessage<ChatRoomMessage> {
	static uint16_t opcount() { return 5; }
	static uint32_t opcode() { return 0xCD4CE444; }

	std::string game_name; // arbitrary: "SWG"
	std::string server_name; // galaxy name
	std::string sender_character_name;
	uint32_t channel_id;
	std::wstring message;
	std::wstring out_of_band; // apparently chat room messages usually don't send OutOfBand packages, but implementing just in case

	ChatRoomMessage()
		: game_name("SWG")
	{}

	void AddProsePackage(const anh::ByteBuffer* prose_package)
    {
        prose_package_ = prose_package;
    }

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(game_name);
		buffer.write(server_name);
		buffer.write(sender_character_name);
		buffer.write(channel_id);
		buffer.write(message);
		buffer.write(out_of_band);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		game_name = buffer.read<std::string>();
		server_name = buffer.read<std::string>();
		sender_character_name = buffer.read<std::string>();
		channel_id = buffer.read<uint32_t>();
		message = buffer.read<std::wstring>();
		out_of_band = buffer.read<std::wstring>();
	}

private:
	const anh::ByteBuffer* prose_package_;
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_ROOM_MESSAGE_H_
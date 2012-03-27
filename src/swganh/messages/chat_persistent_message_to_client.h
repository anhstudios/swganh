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

#ifndef SWGANH_MESSAGES_CHAT_PERSISTENT_MESSAGE_TO_CLIENT_H_
#define SWGANH_MESSAGES_CHAT_PERSISTENT_MESSAGE_TO_CLIENT_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ChatPersistentMessageToClient : public swganh::messages::BaseSwgMessage<ChatPersistentMessageToClient> {
	static uint16_t opcount() { return 2; }
	static uint32_t opcode() { return 0x08485E17; }

	std::string sender_character_name;
	std::string game_name; // arbitrary: "SWG"
	std::string server_name; // galaxy name
	uint32_t mail_message_id;
	uint8_t request_type_flag; // 01 = listing message subjects in the mail box, 00 = displaying the message body for the message requested in ChatRequestPersistentMessage
	std::wstring mail_message_body;
	std::wstring mail_message_subject;
	uint32_t null_spacer;
	uint8_t status; // N = New, R = Read, U = Unread
	uint32_t timestamp;
	uint32_t unknown;

	ChatPersistentMessageToClient()
		: game_name("SWG")
	{}

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(sender_character_name);
		buffer.write(game_name);
		buffer.write(server_name);
		buffer.write(mail_message_id);
		buffer.write(request_type_flag);
		if (request_type_flag == 1)
		{
			buffer.write(null_spacer);
			buffer.write(mail_message_subject);
		}
		else if (request_type_flag == 0)
		{
			buffer.write(mail_message_body);
			buffer.write(mail_message_subject);
		}
		buffer.write(status);
		buffer.write(timestamp);
		buffer.write(unknown);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		sender_character_name = buffer.read<std::string>();
		game_name = buffer.read<std::string>();
		server_name = buffer.read<std::string>();
		mail_message_id = buffer.read<uint32_t>();
		request_type_flag = buffer.read<uint8_t>();
		if (request_type_flag == 1)
		{
			null_spacer = buffer.read<uint32_t>();
			mail_message_subject = buffer.read<std::wstring>();
		}
		else if (request_type_flag == 0)
		{
			mail_message_body = buffer.read<std::wstring>();
			mail_message_subject = buffer.read<std::wstring>();
		}
		status = buffer.read<uint8_t>();
		timestamp = buffer.read<uint32_t>();
		unknown = buffer.read<uint32_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_PERSISTENT_MESSAGE_TO_CLIENT_H_
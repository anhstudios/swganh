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

#ifndef SWGANH_MESSAGES_CHAT_PERSISTENT_MESSAGE_TO_SERVER_H_
#define SWGANH_MESSAGES_CHAT_PERSISTENT_MESSAGE_TO_SERVER_H_

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ChatPersistentMessageToServer : public swganh::messages::BaseSwgMessage<ChatPersistentMessageToServer> {
	static uint16_t opcount() { return 6; }
	static uint32_t opcode() { return 0x25A29FA6; }

	std::wstring mail_message_body;
	uint32_t chat_parameter_size; // uncertain
	uint16_t unknown1;
	uint8_t chat_parameter_type; // 0x04 = waypoint chat parameter
	uint32_t containment_type; // uncertain
	uint32_t unknown2;
	glm::vec3 waypoint_coordinates;
	uint64_t waypoint_object_id;
	uint32_t waypoint_planet_crc;
	std::wstring waypoint_name;
	uint64_t cell_id;
	uint8_t color;
	uint8_t active_flag;
	uint32_t sequence_number;
	std::wstring mail_message_subject;
	uint32_t spacer;
	std::string recipient_name;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(mail_message_body);
		buffer.write(chat_parameter_size);
		buffer.write(unknown1);
		buffer.write(chat_parameter_type);
		buffer.write(containment_type);
		buffer.write(unknown2);
		buffer.write(waypoint_coordinates.x);
		buffer.write(waypoint_coordinates.z);
		buffer.write(waypoint_coordinates.y);
		buffer.write(waypoint_object_id);
		buffer.write(waypoint_planet_crc);
		buffer.write(waypoint_name);
		buffer.write(cell_id);
		buffer.write(color);
		buffer.write(active_flag);
		buffer.write(sequence_number);
		buffer.write(mail_message_subject);
		buffer.write(spacer);
		buffer.write(recipient_name);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		mail_message_body = buffer.read<std::wstring>();
		chat_parameter_size = buffer.read<uint32_t>();
		unknown1 = buffer.read<uint16_t>();
		chat_parameter_type = buffer.read<uint8_t>();
		containment_type = buffer.read<uint32_t>();
		unknown2 = buffer.read<uint32_t>();
		waypoint_coordinates.x = buffer.read<float>();
		waypoint_coordinates.z = buffer.read<float>();
		waypoint_coordinates.y = buffer.read<float>();
		waypoint_object_id = buffer.read<uint64_t>();
		waypoint_planet_crc = buffer.read<uint32_t>();
		waypoint_name = buffer.read<std::wstring>();
		cell_id = buffer.read<uint64_t>();
		color = buffer.read<uint8_t>();
		active_flag = buffer.read<uint8_t>();
		sequence_number = buffer.read<uint32_t>();
		mail_message_subject = buffer.read<std::wstring>();
		spacer = buffer.read<uint32_t>();
		recipient_name = buffer.read<std::string>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CHAT_PERSISTENT_MESSAGE_TO_SERVER_H_
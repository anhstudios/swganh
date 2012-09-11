// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include "swganh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatPersistentMessageToServer : public BaseSwgMessage
    {
    	uint16_t Opcount() const { return 6; }
    	uint32_t Opcode() const { return 0x25A29FA6; }

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

    	void OnSerialize(swganh::ByteBuffer& buffer) const
    	{
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

    	void OnDeserialize(swganh::ByteBuffer& buffer)
    	{
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

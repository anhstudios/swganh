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

#ifndef SWGANH_MESSAGES_ATTRIBUTE_LIST_MESSAGE_H_
#define SWGANH_MESSAGES_ATTRIBUTE_LIST_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct Attribute {
	std::string name;
	std::wstring value;
};

struct AttributeListMessage : public swganh::messages::BaseSwgMessage<AttributeListMessage> {
	static uint16_t opcount() { return 3; }
	static uint32_t opcode() { return 0xF3F12F2A; }

	uint64_t object_id;
	std::list<Attribute> attributes;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(object_id);
		buffer.write<uint32_t>(attributes.size());
		std::for_each(attributes.begin(), attributes.end(), [&buffer] (Attribute attribute){
			buffer.write<std::string>(attribute.name);
			buffer.write<std::wstring>(attribute.value);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
        object_id = buffer.read<uint64_t>();
		uint32_t attribute_count = buffer.read<uint32_t>();
		for(uint32_t i = 0; i < attribute_count; i++) {
			Attribute attribute;
			attribute.name = buffer.read<std::string>();
			attribute.value = buffer.read<std::wstring>();
			attributes.push_back(attribute);
		}
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ATTRIBUTE_LIST_MESSAGE_H_
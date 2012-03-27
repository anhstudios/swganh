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

#ifndef SWGANH_MESSAGES_RESOURCE_LIST_FOR_SURVEY_MESSAGE_H_
#define SWGANH_MESSAGES_RESOURCE_LIST_FOR_SURVEY_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ResourceInfo {
	std::string name;
	uint64_t object_id;
	std::string type;
};

struct ResourceListForSurveyMessage : public swganh::messages::BaseSwgMessage<ResourceListForSurveyMessage> {
	static uint16_t opcount() { return 4; }
	static uint32_t opcode() { return 0x8A64B1D5; }
	
	std::list<ResourceInfo> resources;
	std::string resources_type; // type of all resources in the list
	uint64_t surveyor_object_id;
	
	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(resources.size());
		std::for_each(resources.begin(), resources.end(), [&buffer] (ResourceInfo resource) {
			buffer.write(resource.name);
			buffer.write(resource.object_id);
			buffer.write(resource.type);
		});
		buffer.write(resources_type);
		buffer.write(surveyor_object_id);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		uint32_t resources_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < resources_count; i++) {
			ResourceInfo resource;
			resource.name = buffer.read<std::string>();
			resource.object_id = buffer.read<uint64_t>();
			resource.type = buffer.read<std::string>();
			resources.push_back(resource);
		}
		resources_type = buffer.read<std::string>();
		surveyor_object_id = buffer.read<uint64_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_RESOURCE_LIST_FOR_SURVEY_MESSAGE_H_
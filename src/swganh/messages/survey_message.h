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

#ifndef SWGANH_MESSAGES_SURVEY_MESSAGE_H_
#define SWGANH_MESSAGES_SURVEY_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct SurveyResource {
	float x;
	float z; // should be 0, because the z-axis is irrelevant for resources
	float y;
	float percentage;

	SurveyResource()
		: z(0)
	{}
};

struct SurveyMessage : public swganh::messages::BaseSwgMessage<SurveyMessage> {
	static uint16_t opcount() { return 2; }
	static uint32_t opcode() { return 0x877F79AC; }
	
	std::list<SurveyResource> resources;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(resources.size());
		std::for_each(resources.begin(), resources.end(), [&buffer] (SurveyResource resource) {
			buffer.write(resource.x);
			buffer.write(resource.z);
			buffer.write(resource.y);
			buffer.write(resource.percentage);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		uint32_t resources_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < resources_count; i++) {
			SurveyResource resource;
			resource.x = buffer.read<float>();
			resource.z = buffer.read<float>();
			resource.y = buffer.read<float>();
			resource.percentage = buffer.read<float>();
			resources.push_back(resource);
		}
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_SURVEY_MESSAGE_H_
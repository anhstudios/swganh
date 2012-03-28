// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SURVEY_MESSAGE_H_
#define SWGANH_MESSAGES_SURVEY_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct SurveyResource
    {
    	float x;
    	float z; // should be 0, because the z-axis is irrelevant for resources
    	float y;
    	float percentage;

    	SurveyResource()
    		: z(0)
    	{}
    };

    struct SurveyMessage : public BaseSwgMessage<SurveyMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x877F79AC; }
    	
    	std::list<SurveyResource> resources;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(resources.size());
    		std::for_each(resources.begin(), resources.end(), [&buffer] (SurveyResource resource) {
    			buffer.write(resource.x);
    			buffer.write(resource.z);
    			buffer.write(resource.y);
    			buffer.write(resource.percentage);
    		});
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
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

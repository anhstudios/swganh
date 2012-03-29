// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CREATE_CLIENT_PATH_MESSAGE_H_
#define SWGANH_MESSAGES_CREATE_CLIENT_PATH_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <glm/glm.hpp>
#include <list>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct CreateClientPathMessage : public BaseSwgMessage<CreateClientPathMessage>
    {
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0x71957628; }
    	
    	std::list<glm::vec3> path_coordinates;
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(path_coordinates.size());
    		std::for_each(path_coordinates.begin(), path_coordinates.end(), [&buffer] (glm::vec3 coordinate){
    			buffer.write(coordinate.x);
    			buffer.write(coordinate.z);
    			buffer.write(coordinate.y);
    		});
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		uint32_t coordinate_count = buffer.read<uint32_t>();
    		for (uint32_t i = 0; i < coordinate_count; i++) {
    			glm::vec3 coordinate;
    			coordinate.x = buffer.read<float>();
    			coordinate.z = buffer.read<float>();
    			coordinate.y = buffer.read<float>();
    			path_coordinates.push_back(coordinate);
    		}
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CREATE_CLIENT_PATH_MESSAGE_H_

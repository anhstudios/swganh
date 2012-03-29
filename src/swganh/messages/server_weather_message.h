// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SERVER_WEATHER_MESSAGE_H_
#define SWGANH_MESSAGES_SERVER_WEATHER_MESSAGE_H_

#include <cstdint>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ServerWeatherMessage : public BaseSwgMessage<ServerWeatherMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x486356EA; }

    	uint32_t weather_id;
    	glm::vec3 cloud_vector;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(weather_id);
    		buffer.write(cloud_vector.x);
    		buffer.write(cloud_vector.z);
    		buffer.write(cloud_vector.y);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		weather_id = buffer.read<uint32_t>();
    		cloud_vector.x = buffer.read<float>();
    		cloud_vector.z = buffer.read<float>();
    		cloud_vector.y = buffer.read<float>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_SERVER_WEATHER_MESSAGE_H_

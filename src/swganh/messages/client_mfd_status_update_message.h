// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CLIENT_MFD_STATUS_UPDATE_MESSAGE_H_
#define SWGANH_MESSAGES_CLIENT_MFD_STATUS_UPDATE_MESSAGE_H_

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ClientMfdStatusUpdateMessage : public BaseSwgMessage<ClientMfdStatusUpdateMessage>
    {
    	static uint16_t Opcount() { return 6; }
    	static uint32_t Opcode() { return 0x2D2D6EE1; }

    	std::string planet_name;
    	uint64_t object_id;
    	glm::vec3 object_location;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(planet_name);
    		buffer.write(object_id);
    		buffer.write(object_location.x);
    		buffer.write(object_location.y);
    		buffer.write(object_location.z);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		planet_name = buffer.read<std::string>();
    		object_id = buffer.read<uint64_t>();
    		object_location.x = buffer.read<float>();
    		object_location.y = buffer.read<float>();
    		object_location.z = buffer.read<float>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CLIENT_MFD_STATUS_UPDATE_MESSAGE_H_

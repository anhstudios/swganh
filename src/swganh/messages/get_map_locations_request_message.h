// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_GET_MAP_LOCATIONS_REQUEST_MESSAGE_H_
#define SWGANH_MESSAGES_GET_MAP_LOCATIONS_REQUEST_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct GetMapLocationsRequestMessage : public BaseSwgMessage<GetMapLocationsRequestMessage>
    {
    	static uint16_t Opcount() { return 5; }
    	static uint32_t Opcode() { return 0x1A7AB839; }
    	
    	std::string planet_name;
    	uint32_t unknown1;
    	uint32_t unknown2;
    	uint32_t unknown3;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(planet_name);
    		buffer.write(unknown1);
    		buffer.write(unknown2);
    		buffer.write(unknown3);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		planet_name = buffer.read<std::string>();
    		unknown1 = buffer.read<uint32_t>();
    		unknown2 = buffer.read<uint32_t>();
    		unknown3 = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GET_MAP_LOCATIONS_REQUEST_MESSAGE_H_

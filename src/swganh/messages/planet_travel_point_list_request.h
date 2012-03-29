// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_PLANET_TRAVEL_POINT_LIST_REQUEST_H_
#define SWGANH_MESSAGES_PLANET_TRAVEL_POINT_LIST_REQUEST_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct PlanetTravelPointListRequest : public BaseSwgMessage<PlanetTravelPointListRequest>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x96405D4D; }
    	
    	uint64_t travel_terminal_id;
    	std::string planet_name;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(travel_terminal_id);
    		buffer.write(planet_name);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		travel_terminal_id = buffer.read<uint64_t>();
    		planet_name = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLANET_TRAVEL_POINT_LIST_REQUEST_H_

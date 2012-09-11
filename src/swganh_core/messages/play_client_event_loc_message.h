// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include <glm/glm.hpp>
#include "swganh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct PlayClientEventLocMessage : public BaseSwgMessage
    {
    	uint16_t Opcount() const { return 9; }
    	uint32_t Opcode() const { return 0x0A4E222C; }

    	std::string client_effect_file; // uncertain, because PlayClientEffectLocMessage is used for clienteffect files already
    	std::string planet_name; // e.g. "naboo"
    	glm::vec3 location_coordinates;
    	uint64_t unknown1;
    	uint32_t unknown2;

    	void OnSerialize(swganh::ByteBuffer& buffer) const
    	{
    		buffer.write(client_effect_file);
    		buffer.write(planet_name);
    		buffer.write(location_coordinates.x);
    		buffer.write(location_coordinates.z);
    		buffer.write(location_coordinates.y);
    		buffer.write(unknown1);
    		buffer.write(unknown2);
    	}

    	void OnDeserialize(swganh::ByteBuffer& buffer)
    	{
    		client_effect_file = buffer.read<std::string>();
    		planet_name = buffer.read<std::string>();
    		location_coordinates.x = buffer.read<float>();
    		location_coordinates.z = buffer.read<float>();
    		location_coordinates.y = buffer.read<float>();
    		unknown1 = buffer.read<uint64_t>();
    		unknown2 = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

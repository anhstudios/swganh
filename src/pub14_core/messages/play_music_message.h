// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "swganh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct PlayMusicMessage : public BaseSwgMessage
    {
    	uint16_t Opcount() const { return 5; }
    	uint32_t Opcode() const { return 0x04270D8A; }
    	
    	std::string music_file; // e.g. "sound/music_id_tent_naboo_loop.snd"
    	uint64_t unknown1;
    	uint32_t unknown2;
    	uint8_t unknown3;
    	
    	void OnSerialize(swganh::ByteBuffer& buffer) const
    	{
    		buffer.write(music_file);
    		buffer.write(unknown1);
    		buffer.write(unknown2);
    		buffer.write(unknown3);
    	}

    	void OnDeserialize(swganh::ByteBuffer& buffer)
    	{
    		music_file = buffer.read<std::string>();
    		unknown1 = buffer.read<uint64_t>();
    		unknown2 = buffer.read<uint32_t>();
    		unknown3 = buffer.read<uint8_t>();
    	}
    };

}} // namespace swganh::messages

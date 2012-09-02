// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct BeginTradeMessage : public BaseSwgMessage<BeginTradeMessage>
    {
    	static uint16_t Opcount() { return 1; }
    	static uint32_t Opcode() { return 0x325932D8; }
    	
    	uint64_t target_id;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(target_id);
    	}
    	
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		target_id = buffer.read<uint64_t>();
    	}
    };

}} // namespace swganh::messages

// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ChatOnConnectAvatar : public BaseSwgMessage<ChatOnConnectAvatar>
    {
    	static uint16_t Opcount() { return 1; }
    	static uint32_t Opcode() { return 0xD72FE9BE; }
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const {}
    	
    	void OnDeserialize(anh::ByteBuffer buffer) {}
    };

}} // namespace swganh::messages

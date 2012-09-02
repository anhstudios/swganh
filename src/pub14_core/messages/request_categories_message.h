// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct RequestCategoriesMessage : public BaseSwgMessage<RequestCategoriesMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0xF898E25F; }
    	
    	std::string abbreviated_language_locale; // e.g. "en" for English
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(abbreviated_language_locale);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		abbreviated_language_locale = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

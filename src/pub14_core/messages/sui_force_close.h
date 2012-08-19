// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SUI_FORCE_CLOSE_H_
#define SWGANH_MESSAGES_SUI_FORCE_CLOSE_H_

#include <cstdint>
#include <glm/glm.hpp>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct SUIForceClose : public BaseSwgMessage<SUIForceClose>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x990B5DE0; }

		int32_t window_id;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(window_id);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		window_id = buffer.read<int32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_SUI_FORCE_CLOSE_H_

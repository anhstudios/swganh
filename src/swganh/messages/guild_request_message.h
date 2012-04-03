// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_GUILD_REQUEST_MESSAGE_H_
#define SWGANH_MESSAGES_GUILD_REQUEST_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct GuildRequestMessage : public BaseSwgMessage<GuildRequestMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x81EB4EF7; }
    	
    	uint64_t object_id;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(object_id);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		object_id = buffer.read<uint64_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GUILD_REQUEST_MESSAGE_H_

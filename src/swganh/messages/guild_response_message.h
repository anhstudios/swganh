// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_GUILD_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_GUILD_RESPONSE_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct GuildResponseMessage : public BaseSwgMessage<GuildResponseMessage>
    {
    	static uint16_t Opcount() { return 1; }
    	static uint32_t Opcode() { return 0x32263F20; }
    	
    	uint64_t guild_id;
    	std::string guild_name;
    	uint16_t unknown;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(guild_id);
    		buffer.write(guild_name);
    		buffer.write(unknown);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		guild_id = buffer.read<uint64_t>();
    		guild_name = buffer.read<std::string>();
    		unknown = buffer.read<uint16_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GUILD_RESPONSE_MESSAGE_H_

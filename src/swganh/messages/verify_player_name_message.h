// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_VERIFY_PLAYER_NAME_MESSAGE_H_
#define SWGANH_MESSAGES_VERIFY_PLAYER_NAME_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct VerifyPlayerNameMessage : public BaseSwgMessage<VerifyPlayerNameMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xBB8CAD45; }
    	
    	std::wstring name;
    	uint32_t attempts_counter;
    	uint32_t unknown;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(name);
    		buffer.write(attempts_counter);
    		buffer.write(unknown);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		name = buffer.read<std::wstring>();
    		attempts_counter = buffer.read<uint32_t>();
    		unknown = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_VERIFY_PLAYER_NAME_MESSAGE_H_

// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_VERIFY_PLAYER_NAME_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_VERIFY_PLAYER_NAME_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct VerifyPlayerNameResponseMessage : public BaseSwgMessage<VerifyPlayerNameResponseMessage>
    {
    	static uint16_t Opcount() { return 9; }
    	static uint32_t Opcode() { return 0xF4C498FD; }
    	
    	uint8_t success_flag; // 0 = name not accepted, 1 = name accepted
    	uint32_t attempts_counter; // corresponds to the VerifyPlayerNameMessage packet
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(success_flag);
    		buffer.write(attempts_counter);
    	}
    	
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		success_flag = buffer.read<uint8_t>();
    		attempts_counter = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_VERIFY_PLAYER_NAME_RESPONSE_MESSAGE_H_

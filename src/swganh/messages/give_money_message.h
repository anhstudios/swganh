// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_GIVE_MONEY_MESSAGE_H_
#define SWGANH_MESSAGES_GIVE_MONEY_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct GiveMoneyMessage : public BaseSwgMessage<GiveMoneyMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0xD1527EE8; }
    	
    	uint32_t credit_amount;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(credit_amount);
    	}
    	
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		credit_amount = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GIVE_MONEY_MESSAGE_H_

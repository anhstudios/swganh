// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_PLAYER_MONEY_RESPONSE_H_
#define SWGANH_MESSAGES_PLAYER_MONEY_RESPONSE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct PlayerMoneyResponse : public BaseSwgMessage<PlayerMoneyResponse>
    {
    	static uint16_t Opcount() { return 1; }
    	static uint32_t Opcode() { return 0x367E737E; }

    	uint32_t inventory_credit_amount;
    	uint32_t bank_credit_amount;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(inventory_credit_amount);
    		buffer.write(bank_credit_amount);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		inventory_credit_amount = buffer.read<uint32_t>();
    		bank_credit_amount = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLAYER_MONEY_RESPONSE_H_

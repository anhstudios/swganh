// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BEGIN_TRADE_MESSAGE_H_
#define SWGANH_MESSAGES_BEGIN_TRADE_MESSAGE_H_

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

#endif // SWGANH_MESSAGES_BEGIN_TRADE_MESSAGE_H_
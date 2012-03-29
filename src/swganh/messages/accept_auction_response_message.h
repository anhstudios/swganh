// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_ACCEPT_AUCTION_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_ACCEPT_AUCTION_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct AcceptAuctionResponseMessage : public BaseSwgMessage<AcceptAuctionResponseMessage> 
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xC58A446E; }

    	uint64_t item_id;
    	uint32_t error;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(item_id);
    		buffer.write(error);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		item_id = buffer.read<uint64_t>();
    		error = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ACCEPT_AUCTION_RESPONSE_MESSAGE_H_

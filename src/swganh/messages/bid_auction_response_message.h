// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BID_AUCTION_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_BID_AUCTION_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct BidAuctionResponseMessage : public BaseSwgMessage<BidAuctionResponseMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xC58A446E; }

    	uint64_t item_id;
    	uint32_t status_flag; // 0 = Succeeded, 1 = Auctioneer is Invalid, 2 = Invalid Item, 9 = Not Enough Credits

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(item_id);
    		buffer.write(status_flag);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		item_id = buffer.read<uint64_t>();
    		status_flag = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_BID_AUCTION_RESPONSE_MESSAGE_H_

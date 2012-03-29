// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BID_AUCTION_MESSAGE_H_
#define SWGANH_MESSAGES_BID_AUCTION_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct BidAuctionMessage : public BaseSwgMessage<BidAuctionMessage>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0x91125453; }

    	uint64_t item_id;
    	uint32_t bid_price;
    	uint32_t price_proxy;

    	void OnSerialize(anh::ByteBuffer& buffer) const 
    	{
    		buffer.write(item_id);
    		buffer.write(bid_price);
    		buffer.write(price_proxy);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer) 
    	{
    		item_id = buffer.read<uint64_t>();
    		bid_price = buffer.read<uint32_t>();
    		price_proxy = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_BID_AUCTION_MESSAGE_H_

// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CREATE_AUCTION_MESSAGE_H_
#define SWGANH_MESSAGES_CREATE_AUCTION_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct CreateAuctionMessage : public BaseSwgMessage<CreateAuctionMessage>
    {
    	static uint16_t Opcount() { return 7; }
    	static uint32_t Opcode() { return 0xAD47021D; }

    	uint64_t item_id;
    	uint64_t bazaar_terminal_id;
    	uint32_t price;
    	uint32_t auction_duration_seconds;
    	std::wstring item_description;
    	uint8_t premium_auction_flag; // 0 = non-premium, 1 = premium
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(item_id);
    		buffer.write(bazaar_terminal_id);
    		buffer.write(price);
    		buffer.write(auction_duration_seconds);
    		buffer.write(item_description);
    		buffer.write(premium_auction_flag);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		item_id = buffer.read<uint64_t>();
    		bazaar_terminal_id = buffer.read<uint64_t>();
    		price = buffer.read<uint32_t>();
    		auction_duration_seconds = buffer.read<uint32_t>();
    		item_description = buffer.read<std::wstring>();
    		premium_auction_flag = buffer.read<uint8_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CREATE_AUCTION_MESSAGE_H_

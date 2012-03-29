// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_RETRIEVE_AUCTION_ITEM_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_RETRIEVE_AUCTION_ITEM_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct RetrieveAuctionItemResponseMessage : public BaseSwgMessage<RetrieveAuctionItemResponseMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x9499EF8C; }
    	
    	uint64_t item_id;
    	uint32_t success_flag; // 00 = success, 01 = You are not allowed to retrieve that item, 12 = Inventory full
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(item_id);
    		buffer.write(success_flag);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		item_id = buffer.read<uint64_t>();
    		success_flag = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_RETRIEVE_AUCTION_ITEM_RESPONSE_MESSAGE_H_

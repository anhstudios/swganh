// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_RETRIEVE_AUCTION_ITEM_MESSAGE_H_
#define SWGANH_MESSAGES_RETRIEVE_AUCTION_ITEM_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct RetrieveAuctionItemMessage : public BaseSwgMessage<RetrieveAuctionItemMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x12B0D449; }
    	
    	uint64_t item_id;
    	uint64_t bazaar_terminal_id;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(item_id);
    		buffer.write(bazaar_terminal_id);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		item_id = buffer.read<uint64_t>();
    		bazaar_terminal_id = buffer.read<uint64_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_RETRIEVE_AUCTION_ITEM_MESSAGE_H_

// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_ITEM_SOLD_MESSAGE_H_
#define SWGANH_MESSAGES_ITEM_SOLD_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ItemSoldMessage : public BaseSwgMessage<ItemSoldMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x0E61CC92; }

    	uint64_t item_id;
    	uint32_t status_flag; // See wiki.opengalaxies.org/ItemSoldMessage for status flags

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

#endif // SWGANH_MESSAGES_ITEM_SOLD_MESSAGE_H_

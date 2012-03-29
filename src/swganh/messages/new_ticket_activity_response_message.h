// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_NEW_TICKET_ACTIVITY_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_NEW_TICKET_ACTIVITY_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct NewTicketActivityResponseMessage : public BaseSwgMessage<NewTicketActivityResponseMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x6EA42D80; }
    	
    	uint8_t update_status; // 0 = not updated; 1 = updated
    	uint64_t ticket_id;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(update_status);
    		buffer.write(ticket_id);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		update_status = buffer.read<uint8_t>();
    		ticket_id = buffer.read<uint64_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_NEW_TICKET_ACTIVITY_RESPONSE_MESSAGE_H_

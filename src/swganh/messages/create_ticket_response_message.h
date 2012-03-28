// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CREATE_TICKET_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_CREATE_TICKET_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct CreateTicketResponseMessage : public BaseSwgMessage<CreateTicketResponseMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x550A407A; }
    
    	uint32_t message_flag; // 0 = success, 1 = failure
    	uint32_t ticket_id;
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(message_flag);
    		buffer.write(ticket_id);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		message_flag = buffer.read<uint32_t>();
    		ticket_id = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CREATE_TICKET_RESPONSE_MESSAGE_H_
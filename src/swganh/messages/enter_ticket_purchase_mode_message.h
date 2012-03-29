// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_ENTER_TICKET_PURCHASE_MODE_MESSAGE_H_
#define SWGANH_MESSAGES_ENTER_TICKET_PURCHASE_MODE_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct EnterTicketPurchaseModeMessage : public BaseSwgMessage<EnterTicketPurchaseModeMessage>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0x904DAE1A; }
    
    	std::string planet_name; // planet on which the terminal is located
    	std::string city_name; // city in which the terminal is located
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(planet_name);
    		buffer.write(city_name);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		planet_name = buffer.read<std::string>();
    		city_name = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ENTER_TICKET_PURCHASE_MODE_MESSAGE_H_

// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CON_GENERIC_MESSAGE_H_
#define SWGANH_MESSAGES_CON_GENERIC_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ConGenericMessage : public BaseSwgMessage<ConGenericMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x8C5FC76; }

    	std::string message;
    	uint32_t string_spacer;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(message);
    		buffer.write(string_spacer);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		message = buffer.read<std::string>();
    		string_spacer = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CON_GENERIC_MESSAGE_H_

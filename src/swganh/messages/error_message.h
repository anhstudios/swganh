// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_ERROR_MESSAGE_H_
#define SWGANH_MESSAGES_ERROR_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ErrorMessage : public BaseSwgMessage<ErrorMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xB5ABF91A; }

    	std::string type;
    	std::string message;
    	bool force_fatal;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(type);
    		buffer.write(message);
    		buffer.write(force_fatal);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		type = buffer.read<std::string>();
    		message = buffer.read<std::string>();
    		force_fatal = buffer.read<bool>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ERROR_MESSAGE_H_

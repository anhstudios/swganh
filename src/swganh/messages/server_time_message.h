// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SERVER_TIME_MESSAGE_H_
#define SWGANH_MESSAGES_SERVER_TIME_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ServerTimeMessage : public BaseSwgMessage<ServerTimeMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x2EBC3BD9; }

    	uint64_t galactic_time; // number of seconds from server start to present galactic time

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(galactic_time);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		galactic_time = buffer.read<uint64_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_SERVER_TIME_MESSAGE_H_

// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_PARAMETERS_MESSAGE_H_
#define SWGANH_MESSAGES_PARAMETERS_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ParametersMessage : public BaseSwgMessage<ParametersMessage>
    {
    	static uint16_t Opcount() { return 2; }
    	static uint32_t Opcode() { return 0x487652DA; }
    	
    	uint32_t parameter_flag; // default: 900 = decimal, 384 = hex, 1110000100 = binary
    
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(parameter_flag);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		parameter_flag = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PARAMETERS_MESSAGE_H_

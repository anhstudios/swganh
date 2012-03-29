// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_OPENED_CONTAINER_MESSAGE_H_
#define SWGANH_MESSAGES_OPENED_CONTAINER_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct OpenedContainerMessage : public BaseSwgMessage<OpenedContainerMessage>
    {
    	static uint16_t Opcount() { return 4; }
    	static uint32_t Opcode() { return 0x2E11E4AB; }
    	
    	uint32_t unknown1;
    	uint64_t container_object_id;
    	std::string container_slot;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(unknown1);
    		buffer.write(container_object_id);
    		buffer.write(container_slot);
    	}
    
    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		unknown1 = buffer.read<uint32_t>();
    		container_object_id = buffer.read<uint64_t>();
    		container_slot = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_OPENED_CONTAINER_MESSAGE_H_
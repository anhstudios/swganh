// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_UPDATE_POSTURE_MESSAGE_H_
#define SWGANH_MESSAGES_UPDATE_POSTURE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
namespace controllers {
    
    struct UpdatePostureMessage : public BaseSwgMessage<UpdatePostureMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0x0BDE6B41; }

        int8_t posture_id;
        uint64_t object_id;
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const 
        {
            buffer.write(posture_id);
            buffer.write(object_id);
        }
    	
    	void OnDeserialize(anh::ByteBuffer buffer)
        {
            posture_id = buffer.read<int8_t>();
            object_id = buffer.read<uint64_t>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_UPDATE_POSTURE_MESSAGE_H_

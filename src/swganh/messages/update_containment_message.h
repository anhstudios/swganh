// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_UPDATE_CONTAINMENT_MESSAGE_H_
#define SWGANH_MESSAGES_UPDATE_CONTAINMENT_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct UpdateContainmentMessage : public BaseSwgMessage<UpdateContainmentMessage>
    {
        static uint16_t Opcount() { return 4; }
        static uint32_t Opcode() { return 0x56CBDE9E; }
        
        uint64_t object_id;
        uint64_t container_id;
        int32_t containment_type;
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(object_id);
            buffer.write(container_id);
            buffer.write(containment_type);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
            object_id = buffer.read<uint64_t>();
            container_id = buffer.read<uint64_t>();
            containment_type = buffer.read<int32_t>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_UPDATE_CONTAINMENT_MESSAGE_H_

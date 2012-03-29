// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BASE_DELTAS_MESSAGE_H_
#define SWGANH_MESSAGES_BASE_DELTAS_MESSAGE_H_

#include <cstdint>
#include <iostream>

#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    template<typename T>
    struct BaseDeltasMessage : public BaseSwgMessage<T>
    {    
        uint64_t object_id;
        uint32_t object_type;
        uint8_t view_type;
        uint16_t update_count;
        uint16_t update_type;
        anh::ByteBuffer data;
    
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(object_id);
            buffer.write(object_type);
            buffer.write(view_type);
            buffer.write<uint32_t>(data.size() + 4);
            buffer.write<uint16_t>(update_count);
            buffer.write<uint16_t>(update_type);
            buffer.write(data.data(), data.size());   
        }
    
        void OnDeserialize(anh::ByteBuffer buffer)
        {        
            object_id = buffer.read<uint32_t>();
            object_type = buffer.read<uint32_t>();  
            view_type = buffer.read<uint32_t>(); 
            uint32_t data_size = buffer.read<uint32_t>() - 4;
            update_type = buffer.read<uint16_t>();
            update_count = buffer.read<uint16_t>();
            data = anh::ByteBuffer(buffer.data() + buffer.read_position(), data_size);
        }
    };
    
}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_BASE_DELTAS_MESSAGE_H_

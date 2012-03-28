// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BASE_BASELINES_MESSAGE_H_
#define SWGANH_MESSAGES_BASE_BASELINES_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    template<typename T>
    struct BaseBaselinesMessage : public BaseSwgMessage<T>
    {    
        uint64_t object_id;
        uint32_t object_type;
        uint16_t object_opcount;
        uint8_t view_type;    
        anh::ByteBuffer data;
    
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(object_id);
            buffer.write(object_type);
            buffer.write(view_type);
    
            buffer.write<uint32_t>(data.size() + 2);
            buffer.write<uint16_t>(object_opcount);
            buffer.write(data.data(), data.size());        
        }
    
        void OnDeserialize(anh::ByteBuffer buffer)
        {        
            object_id = buffer.read<uint32_t>();
            object_type = buffer.read<uint32_t>();  
            view_type = buffer.read<uint32_t>(); 
            object_opcount = buffer.read<uint16_t>();
    
            uint32_t data_size = buffer.read<uint32_t>();
            data = anh::ByteBuffer(buffer.data() + buffer.read_position(), data_size);
        }
    };
    
}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_BASE_BASELINES_MESSAGE_H_
